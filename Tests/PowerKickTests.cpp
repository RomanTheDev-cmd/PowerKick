// Headless checks of the PowerKick engine, presets and WAV export.
// Run: powerkick_tests [--report]   (--report prints per-preset levels)

#include "DSP/KickEngine.h"
#include "KickRenderer.h"
#include "Presets.h"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <set>

using namespace pk;

namespace
{
int failures = 0, checks = 0;

void expect (bool ok, const juce::String& what)
{
    ++checks;
    if (! ok)
    {
        ++failures;
        std::cout << "FAIL: " << what << "\n";
    }
}

struct Stats { double peak = 0, rms = 0; bool finite = true; };

Stats stats (const juce::AudioBuffer<float>& b, int start = 0, int len = -1)
{
    Stats s;
    if (len < 0) len = b.getNumSamples() - start;
    double sum = 0;
    for (int c = 0; c < b.getNumChannels(); ++c)
        for (int i = start; i < start + len; ++i)
        {
            const double x = b.getSample (c, i);
            if (! std::isfinite (x)) s.finite = false;
            s.peak = std::max (s.peak, std::abs (x));
            sum += x * x;
        }
    s.rms = std::sqrt (sum / std::max (1, len * b.getNumChannels()));
    return s;
}

double rmsOf (const std::vector<float>& v)
{
    double sum = 0;
    for (float x : v) sum += (double) x * x;
    return std::sqrt (sum / (double) std::max<size_t> (1, v.size()));
}

double db (double g) { return 20.0 * std::log10 (std::max (g, 1.0e-12)); }

// Energy of x in [f1, f2] via a 4th-order band filter (two LR4 splits).
// 48 dB/oct edges: for "is there sub in the side signal", where wide highs would leak
// through a 24 dB/oct measuring filter.
double bandRmsSteep (const float* x, int n, double sr, double f1, double f2)
{
    dsp::LR4 lo1, lo2, hi1, hi2;
    lo1.set (f1, sr); lo2.set (f1, sr); hi1.set (f2, sr); hi2.set (f2, sr);
    double sum = 0;
    for (int i = 0; i < n; ++i)
    {
        double a, b, c, d, e, g, h, k;
        lo1.split (x[i], a, b); lo2.split (b, c, d); hi1.split (d, e, g); hi2.split (e, h, k);
        sum += h * h;
    }
    return std::sqrt (sum / std::max (1, n));
}

// Energy in [f1, f2] from a Hann-windowed DFT (Goertzel per bin): no leakage from strong
// content outside the band, unlike a filter. RMS-equivalent amplitude.
double bandRmsDft (const float* x, int n, double sr, double f1, double f2)
{
    std::vector<double> w ((size_t) n);
    double wsum = 0;
    for (int i = 0; i < n; ++i) { w[(size_t) i] = 0.5 - 0.5 * std::cos (2.0 * 3.14159265358979 * i / (n - 1)); wsum += w[(size_t) i] * w[(size_t) i]; }
    double e = 0;
    for (int k = (int) std::ceil (f1 * n / sr); k <= (int) (f2 * n / sr); ++k)
    {
        const double c = 2.0 * std::cos (2.0 * 3.14159265358979 * k / n);
        double s1 = 0, s2 = 0;
        for (int i = 0; i < n; ++i) { const double s0 = x[i] * w[(size_t) i] + c * s1 - s2; s2 = s1; s1 = s0; }
        e += s1 * s1 + s2 * s2 - c * s1 * s2;
    }
    return std::sqrt (2.0 * e / (wsum * n));
}

double bandRms (const float* x, int n, double sr, double f1, double f2)
{
    dsp::LR4 lo, hi;
    lo.set (f1, sr);
    hi.set (f2, sr);
    double sum = 0;
    for (int i = 0; i < n; ++i)
    {
        double a, b, c, d;
        lo.split (x[i], a, b);
        hi.split (b, c, d);
        sum += c * c;
    }
    return std::sqrt (sum / std::max (1, n));
}

KickParams preset (const char* name)
{
    for (const auto& p : factoryPresets())
        if (p.name == name)
            return paramsFromSnapshot (p.values);
    expect (false, juce::String ("missing preset ") + name);
    return {};
}

// Streams one hit through the engine with a chosen host buffer size.
juce::AudioBuffer<float> stream (const KickParams& p, double sr, int hostBlock, int seconds = 1)
{
    dsp::KickEngine e;
    e.prepare (sr, hostBlock);
    const int total = (int) (sr * seconds);
    juce::AudioBuffer<float> out (2, total + hostBlock);
    juce::MidiBuffer on, none;
    on.addEvent (juce::MidiMessage::noteOn (1, 60, 1.0f), 0);
    for (int pos = 0; pos < total; pos += hostBlock)
        e.process (out.getWritePointer (0, pos), out.getWritePointer (1, pos), hostBlock, pos == 0 ? on : none, 0, p,
                   140.0, false);
    out.setSize (2, total, true);
    return out;
}

void testPresetLibrary()
{
    const auto& f = factoryPresets();
    expect (f.size() == 300, "300 factory presets, got " + juce::String ((int) f.size()));
    std::map<juce::String, int> count;
    for (const auto& p : f) ++count[p.category];
    expect (count["Techno"] == 30 && count["Hard Techno"] == 110 && count["Schranz"] == 100 && count["Industrial"] == 56
                && count["Rave / Hard Dance"] == 4,
            "preset category split 30/110/100/56/4");
    std::set<juce::String> names;
    for (const auto& p : f) names.insert (p.name);
    expect (names.size() == f.size(), "factory preset names are unique");
    int grouped = 0;
    for (const auto& p : f) grouped += p.group.isNotEmpty() ? 1 : 0;
    expect (grouped == (int) f.size(), "every factory preset has a browser group");
    int wide = 0;
    for (const auto& p : f) wide += p.values.at (id::width) >= 50.0f ? 1 : 0;
    expect (wide >= 5, "several presets demonstrate the stereo engine");
}

void testPresetsRender (bool report)
{
    for (const auto& pr : factoryPresets())
    {
        const auto p = paramsFromSnapshot (pr.values);
        const auto audio = renderKick (p, 48000.0, 140.0, true);
        const auto s = stats (audio);
        expect (s.finite, pr.name + ": finite output");
        expect (s.peak <= 1.0, pr.name + ": peak <= 0 dBFS (" + juce::String (db (s.peak), 2) + ")");
        expect (s.peak > 0.25, pr.name + ": audible (" + juce::String (db (s.peak), 1) + " dB)");
        // Sub must be mono: side signal below 100 Hz at least 50 dB under the mid.
        const int n = audio.getNumSamples();
        std::vector<float> mid ((size_t) n), side ((size_t) n);
        for (int i = 0; i < n; ++i)
        {
            mid[(size_t) i] = 0.5f * (audio.getSample (0, i) + audio.getSample (1, i));
            side[(size_t) i] = 0.5f * (audio.getSample (0, i) - audio.getSample (1, i));
        }
        const double subMid = bandRms (mid.data(), n, 48000.0, 20.0, 100.0);
        const double subSide = bandRmsDft (side.data(), std::min (n, 48000), 48000.0, 20.0, 100.0)
                               * std::sqrt ((double) std::min (n, 48000) / n);
        expect (db (subSide) - db (subMid) < -50.0, pr.name + ": sub stays mono (" + juce::String (db (subSide) - db (subMid), 1) + " dB)");
        if (report)
        {
            // Level of the first 150 ms: the hit itself, which is what A/B listening compares.
            const auto hit = stats (audio, 0, std::min (n, 7200));
            std::cout << juce::String (pr.name).paddedRight (' ', 18) << " peak " << juce::String (db (s.peak), 1)
                      << "  hitRMS " << juce::String (db (hit.rms), 1) << "  len " << juce::String (n / 48.0, 0)
                      << " ms  side/mid " << juce::String (db (rmsOf (side)) - db (rmsOf (mid)), 1)
                      << " dB\n";
        }
    }
}

void testWidthZeroIsTrueMono()
{
    auto p = preset ("Steel Press");
    p.width = 0.0f;
    p.delayMs = 15.0f; // Delay alone must not create stereo
    const auto a = stream (p, 48000.0, 256);
    double maxDiff = 0;
    for (int i = 0; i < a.getNumSamples(); ++i)
        maxDiff = std::max (maxDiff, (double) std::abs (a.getSample (0, i) - a.getSample (1, i)));
    expect (maxDiff == 0.0, "WIDTH 0 gives bit-identical L/R (diff " + juce::String (maxDiff) + ")");
}

void testMonoSumKeepsKick()
{
    // Mono sum of the widest setting vs. width 0: sub and body unchanged, highs within 2.5 dB.
    for (float delay : { 0.0f, 1.0f, 5.0f, 15.0f })
    {
        auto p = preset ("Wide Iron Sub");
        p.width = 0.0f;
        const auto ref = stream (p, 48000.0, 512);
        p.width = 100.0f;
        p.delayMs = delay;
        const auto wide = stream (p, 48000.0, 512);
        const int n = ref.getNumSamples();
        std::vector<float> mRef ((size_t) n), mWide ((size_t) n);
        for (int i = 0; i < n; ++i)
        {
            mRef[(size_t) i] = 0.5f * (ref.getSample (0, i) + ref.getSample (1, i));
            mWide[(size_t) i] = 0.5f * (wide.getSample (0, i) + wide.getSample (1, i));
        }
        const double bands[][2] = { { 30, 120 }, { 120, 1000 }, { 1000, 3000 }, { 3000, 8000 }, { 8000, 16000 } };
        for (auto& b : bands)
        {
            const double d = db (bandRms (mWide.data(), n, 48000, b[0], b[1])) - db (bandRms (mRef.data(), n, 48000, b[0], b[1]));
            const double limit = b[0] < 1000 ? 0.3 : 2.5;
            expect (std::abs (d) <= limit, "mono sum " + juce::String (b[0]) + "-" + juce::String (b[1]) + " Hz at delay "
                                               + juce::String (delay) + " ms changes " + juce::String (d, 2) + " dB");
        }
        // Per-channel loudness must not grow when widening (no loudness trick).
        const double chRef = stats (ref).rms, chWide = stats (wide).rms;
        expect (db (chWide) - db (chRef) < 0.5, "widening is not a loudness boost (" + juce::String (db (chWide) - db (chRef), 2) + " dB)");
        // And the stereo engine must actually do something.
        double side = 0;
        for (int i = 0; i < n; ++i) side += std::pow (wide.getSample (0, i) - wide.getSample (1, i), 2.0f);
        expect (side > 0.0, "WIDTH 100 produces a side signal");
    }
}

void testSampleRatesAndBlocks()
{
    for (double sr : { 44100.0, 48000.0, 88200.0, 96000.0, 192000.0 })
        for (int block : { 16, 64, 173, 512, 2048 })
        {
            const auto a = stream (preset ("Schranz Metal"), sr, block);
            const auto s = stats (a);
            expect (s.finite && s.peak <= 1.0 && s.peak > 0.25,
                    "sr " + juce::String (sr) + " block " + juce::String (block) + " peak " + juce::String (db (s.peak), 2));
        }
    // Same sound regardless of host buffer size.
    const auto a = stream (preset ("Hard Techno Core"), 48000.0, 64), b = stream (preset ("Hard Techno Core"), 48000.0, 512);
    double diff = 0;
    for (int i = 0; i < a.getNumSamples(); ++i) diff = std::max (diff, (double) std::abs (a.getSample (0, i) - b.getSample (0, i)));
    expect (diff < 0.02, "buffer-size independent output (max diff " + juce::String (diff, 5) + ")");
}

// Largest sample-to-sample jump relative to the signal's own largest jump.
double maxStep (const juce::AudioBuffer<float>& a, int ch, int from, int to)
{
    double m = 0;
    for (int i = std::max (1, from); i < to; ++i) m = std::max (m, (double) std::abs (a.getSample (ch, i) - a.getSample (ch, i - 1)));
    return m;
}

void testNoClicksOnParameterJumps()
{
    // Long low kick (smooth waveform); jump stereo, output, drive and character mid-tail.
    auto p = preset ("Deep Drop");
    p.click = 0.0f;
    const double sr = 48000.0;
    dsp::KickEngine e;
    e.prepare (sr, 256);
    juce::AudioBuffer<float> out (2, (int) sr + 256); // whole blocks must fit
    juce::MidiBuffer on, none;
    on.addEvent (juce::MidiMessage::noteOn (1, 60, 1.0f), 0);
    for (int pos = 0; pos < (int) sr; pos += 256)
    {
        if (pos == 256 * 60) // ~320 ms, inside the tail
        {
            p.width = 100.0f; p.delayMs = 12.0f; p.outputDb = 6.0f; p.drive = 0.9f; p.character = 1.0f;
            p.charMode = CharMode::raw;
        }
        e.process (out.getWritePointer (0, pos), out.getWritePointer (1, pos), 256, pos == 0 ? on : none, 0, p, 140.0, false);
    }
    const double before = maxStep (out, 0, 256 * 40, 256 * 60);
    const double at = maxStep (out, 0, 256 * 60, 256 * 62);
    // Drive legitimately sharpens the waveform; compare with the settled new sound too.
    const double after = maxStep (out, 0, 256 * 64, 256 * 80);
    expect (at < std::max (before, after) * 1.5 + 0.002, "no click on parameter jump (step " + juce::String (at, 4) + " vs " + juce::String (std::max (before, after), 4) + ")");

    // Retrigger in the middle of a loud hit: no discontinuity beyond normal waveform steps.
    auto q = preset ("Deep Drop");
    q.click = 0.0f;
    dsp::KickEngine e2;
    e2.prepare (sr, 256);
    juce::AudioBuffer<float> o2 (2, (int) sr / 2 + 256);
    for (int pos = 0; pos < (int) sr / 2; pos += 256)
    {
        juce::MidiBuffer m;
        if (pos == 0 || pos == 256 * 20) m.addEvent (juce::MidiMessage::noteOn (1, 60, 1.0f), 37);
        e2.process (o2.getWritePointer (0, pos), o2.getWritePointer (1, pos), 256, m, 0, q, 140.0, false);
    }
    const double normal = maxStep (o2, 0, 0, 256 * 20); // includes the first attack
    const double retrig = maxStep (o2, 0, 256 * 20, 256 * 21);
    expect (retrig < normal * 1.6, "retrigger is click-free (" + juce::String (retrig, 4) + " vs " + juce::String (normal, 4) + ")");
}

void testCharacterAudible()
{
    // Each mode at full CHARACTER must move the spectrum clearly (> 2 dB in some band) vs. CHARACTER 0.
    auto base = preset ("Classic Punch");
    base.character = 0.0f;
    const auto ref = renderKick (base, 48000.0, 140.0, false);
    const double bands[][2] = { { 30, 90 }, { 90, 250 }, { 250, 800 }, { 800, 2500 }, { 2500, 8000 } };
    for (int m = 0; m < 5; ++m)
    {
        auto p = base;
        p.charMode = (CharMode) m;
        p.character = 1.0f;
        const auto a = renderKick (p, 48000.0, 140.0, false);
        const int n = std::min (a.getNumSamples(), ref.getNumSamples());
        double maxDelta = 0;
        for (auto& b : bands)
            maxDelta = std::max (maxDelta, std::abs (db (bandRms (a.getReadPointer (0), n, 48000, b[0], b[1]))
                                                     - db (bandRms (ref.getReadPointer (0), n, 48000, b[0], b[1]))));
        expect (maxDelta > 2.0, "character mode " + juce::String (m) + " audible (" + juce::String (maxDelta, 1) + " dB)");
    }
}

void testSettledPitch()
{
    // After the sweep the body must sit on TUNE at any sample rate (pitchFall off: pure settle check).
    for (double sr : { 22050.0, 24000.0, 44100.0, 96000.0 })
    {
        auto p = preset ("Classic Punch");
        p.pitchFall = 0.0f;
        const double expected = baseFrequency (p.tune);
        const auto a = renderKick (p, sr, 140.0, false);
        const float* x = a.getReadPointer (0);
        int first = -1, last = -1, count = 0;
        for (int i = (int) (0.15 * sr); i < (int) (0.25 * sr) && i < a.getNumSamples(); ++i)
            if (x[i - 1] < 0.0f && x[i] >= 0.0f)
            {
                if (first < 0) first = i;
                last = i;
                ++count;
            }
        const double f = count > 1 ? (count - 1) * sr / (last - first) : 0.0;
        expect (std::abs (f - expected) < 1.0, "settled pitch (" + juce::String (expected, 2) + " Hz) at " + juce::String (sr) + " Hz is " + juce::String (f, 2) + " Hz");
    }
}

// Level (dB) of a 40 ms RMS window of channel 0, centred on t (>= 2 periods of a ~50 Hz body).
double frameDb (const juce::AudioBuffer<float>& a, double t, double sr)
{
    const int from = std::max (0, (int) ((t - 0.02) * sr)), to = std::min (a.getNumSamples(), (int) ((t + 0.02) * sr));
    double s = 0;
    for (int i = from; i < to; ++i) s += (double) a.getSample (0, i) * a.getSample (0, i);
    return db (std::sqrt (s / std::max (1, to - from)));
}

void testGatedBody()
{
    // Measured hard-techno kicks hold their body (tail only 1-2 dB under the hit) and then stop.
    auto p = preset ("Hard Techno Core");
    p.body = 1.0f; p.rumble = 0.0f; p.drive = 0.0f; p.click = 0.0f; p.character = 0.0f; // no clipping anywhere
    p.outputDb = -18.0f; // stay clear of the output safety limiter: measure the envelope itself
    const auto a = stream (p, 48000.0, 256);
    const double D = decayMs (p.decay) * 0.001;
    const double early = frameDb (a, 0.2 * D, 48000), late = frameDb (a, 0.75 * D, 48000);
    expect (early - late <= 2.5, "BODY 100% holds the body (drop " + juce::String (early - late, 1) + " dB at 75% of decay)");
    // The end must not click: the last 10 ms may not step harder than the body does.
    const int endI = (int) (D * 48000);
    expect (maxStep (a, 0, endI - 480, endI + 480) <= maxStep (a, 0, (int) (0.3 * D * 48000), (int) (0.5 * D * 48000)) * 1.2,
            "gated end is click-free");
}

void testBodyTone()
{
    auto p = preset ("Classic Schranz");
    // The original driven-schranz scenario: hard drive with clip and HARD character.
    p.rumble = 0.0f; p.drive = 0.8f; p.driveMode = DriveMode::clip; p.charMode = CharMode::hard; p.character = 0.7f; p.click = 0.55f;
    p.bodyTone = 1.0f;
    const auto open = renderKick (p, 48000.0, 140.0, false);
    p.bodyTone = 0.0f;
    const auto dark = renderKick (p, 48000.0, 140.0, false);
    auto seg = [] (const juce::AudioBuffer<float>& a, double t0, double t1, double f1, double f2)
    { return db (bandRms (a.getReadPointer (0, (int) (t0 * 48000)), (int) ((t1 - t0) * 48000), 48000, f1, f2)); };
    const double midDrop = seg (open, 0.045, 0.14, 700, 3000) - seg (dark, 0.045, 0.14, 700, 3000);
    const double lowDiff = std::abs (seg (open, 0.045, 0.14, 30, 120) - seg (dark, 0.045, 0.14, 30, 120));
    const double attackDiff = std::abs (seg (open, 0.0, 0.006, 700, 8000) - seg (dark, 0.0, 0.006, 700, 8000));
    // Measured kicks scoop the body mids but keep the distortion "fizz" on top.
    const double highDiff = std::abs (seg (open, 0.045, 0.14, 4000, 12000) - seg (dark, 0.045, 0.14, 4000, 12000));
    expect (highDiff <= 3.0, "bodyTone keeps the body's top end (" + juce::String (highDiff, 1) + " dB)");
    expect (midDrop >= 8.0, "bodyTone 0 scoops the body mids (" + juce::String (midDrop, 1) + " dB)");
    expect (lowDiff <= 1.5, "bodyTone keeps the low body (" + juce::String (lowDiff, 2) + " dB)");
    expect (attackDiff <= 1.5, "bodyTone keeps the attack (" + juce::String (attackDiff, 2) + " dB)");
}

double pitchAt (const juce::AudioBuffer<float>& a, double t, double sr)
{
    // Zero-crossing frequency over a 60 ms window centred on t.
    int first = -1, last = -1, count = 0;
    for (int i = std::max (1, (int) ((t - 0.03) * sr)); i < std::min (a.getNumSamples(), (int) ((t + 0.03) * sr)); ++i)
        if (a.getSample (0, i - 1) < 0.0f && a.getSample (0, i) >= 0.0f) { if (first < 0) first = i; last = i; ++count; }
    return count > 1 ? (count - 1) * sr / (last - first) : 0.0;
}

void testPitchFall()
{
    // Measured kicks keep falling in pitch through the tail (2-4 semitones from 100 to 250 ms).
    auto p = preset ("Deep Drop");
    p.click = 0.0f; p.rumble = 0.0f; p.drive = 0.0f; p.character = 0.0f; p.decay = 0.45f; p.body = 0.9f; p.outputDb = -12.0f;
    double st[2];
    for (int k = 0; k < 2; ++k)
    {
        p.pitchFall = k == 0 ? 0.0f : 12.0f;
        const auto a = renderKick (p, 48000.0, 140.0, false);
        st[k] = 12.0 * std::log2 (pitchAt (a, 0.10, 48000) / pitchAt (a, 0.25, 48000));
    }
    expect (st[1] - st[0] >= 2.0, "pitchFall 12 adds a 2+ st fall from 100 to 250 ms (" + juce::String (st[1] - st[0], 2) + " st)");
}

void testDriveSweepIsSmooth()
{
    // Same class of bug Codex found in PowerBass: a few-point peak estimate of a non-monotonic
    // shaper can land on its zeros and make the level jump. Level must follow DRIVE smoothly.
    auto base = preset ("Classic Punch");
    base.rumble = 0.0f; base.outputDb = -24.0f; base.character = 0.0f;
    for (int m = 0; m < 3; ++m)
    {
        double prev = 0, worst = 0, at = 0;
        for (int i = 0; i <= 100; ++i)
        {
            auto p = base; p.driveMode = (DriveMode) m; p.drive = 0.01f * (float) i;
            const auto a = renderKick (p, 48000.0, 140.0, false);
            const double level = db (stats (a, 0, std::min (a.getNumSamples(), 7200)).rms);
            if (i > 0 && std::abs (level - prev) > worst) { worst = std::abs (level - prev); at = p.drive; }
            prev = level;
        }
        expect (worst <= 2.0, "drive mode " + juce::String (m) + ": level follows DRIVE smoothly (max step "
                                  + juce::String (worst, 2) + " dB at " + juce::String (at, 2) + ")");
    }
}

double sideToMidDb (const juce::AudioBuffer<float>& a, double f1, double f2)
{
    std::vector<float> m ((size_t) a.getNumSamples()), sd ((size_t) a.getNumSamples());
    for (int i = 0; i < a.getNumSamples(); ++i)
    {
        m[(size_t) i] = 0.5f * (a.getSample (0, i) + a.getSample (1, i));
        sd[(size_t) i] = 0.5f * (a.getSample (0, i) - a.getSample (1, i));
    }
    return db (bandRms (sd.data(), (int) sd.size(), 48000, f1, f2)) - db (bandRms (m.data(), (int) m.size(), 48000, f1, f2));
}

void testStereoControlsAreAudible()
{
    // Roman: "WIDTH and DELAY seem to do nothing". WIDTH must give a clearly audible image on a
    // normal kick, DELAY must add a real time offset; the mono sum stays untouched (other tests).
    auto p = preset ("Hard Techno Core");
    p.rumble = 0.0f;
    p.width = 100.0f; p.delayMs = 0.0f; p.splitHz = 500.0f; // the WIDTH knob itself, at the default SPLIT
    const auto wide = renderKick (p, 48000.0, 140.0, true);
    const double whole = sideToMidDb (wide, 20, 16000);
    expect (whole >= -20.0, "WIDTH 100% is clearly audible on a kick (side " + juce::String (whole, 1) + " dB re mid)");
    p.delayMs = 5.0f;
    const auto delayed = renderKick (p, 48000.0, 140.0, true);
    const double gain = sideToMidDb (delayed, 500, 16000) - sideToMidDb (wide, 500, 16000);
    expect (gain >= 3.0, "DELAY 5 ms audibly adds width to the highs (+" + juce::String (gain, 1) + " dB side)");
}

void testStereoSplit()
{
    // SPLIT sets where widening starts (Roman's request): lower SPLIT, wider low-mids.
    auto p = preset ("Hard Techno Core");
    p.rumble = 0.0f; p.width = 100.0f; p.delayMs = 3.0f;
    p.splitHz = 150.0f; // the lowest SPLIT
    const auto lowest = renderKick (p, 48000.0, 140.0, true);
    p.splitHz = 300.0f;
    const auto low = renderKick (p, 48000.0, 140.0, true);
    p.splitHz = 1500.0f;
    const auto high = renderKick (p, 48000.0, 140.0, true);
    const double diff = sideToMidDb (low, 400, 800) - sideToMidDb (high, 400, 800);
    expect (diff >= 10.0, "SPLIT 300 Hz widens 400-800 Hz far more than SPLIT 1.5 kHz (" + juce::String (diff, 1) + " dB)");
    // Side below 90 Hz (48 dB/oct measurement) relative to the whole mono kick.
    std::vector<float> m ((size_t) lowest.getNumSamples()), sd ((size_t) lowest.getNumSamples());
    for (int i = 0; i < lowest.getNumSamples(); ++i)
    {
        m[(size_t) i] = 0.5f * (lowest.getSample (0, i) + lowest.getSample (1, i));
        sd[(size_t) i] = 0.5f * (lowest.getSample (0, i) - lowest.getSample (1, i));
    }
    const double sub = db (bandRmsSteep (sd.data(), (int) sd.size(), 48000, 20, 90)) - db (bandRms (m.data(), (int) m.size(), 48000, 20, 16000));
    expect (sub < -50.0, "sub stays mono with a low SPLIT (" + juce::String (sub, 1) + " dB)");
}

void testVelocity()
{
    auto p = preset ("Classic Punch");
    const auto loud = renderKick (p, 48000.0, 140.0, false, 1.0f);
    const auto soft = renderKick (p, 48000.0, 140.0, false, 0.4f);
    expect (stats (soft).peak < stats (loud).peak * 0.9, "velocity changes level");
}

void testLegacyPresetMigration()
{
    // A preset saved before bodyTone / pitchFall / splitFreq existed must sound as it did:
    // missing new parameters take their neutral value, not today's defaults.
    const juce::String xml = "<PowerKickPreset format=\"1\" name=\"Old\" category=\"User\">"
                             "<Param id=\"tune\" value=\"2\"/><Param id=\"drive\" value=\"0.4\"/></PowerKickPreset>";
    Preset p;
    juce::String err;
    expect (presetFromXml (xml, p, err), "old preset parses");
    expect (p.values.count (id::bodyTone) && std::abs (p.values[id::bodyTone] - 1.0f) < 1e-6f, "old preset: bodyTone neutral (1)");
    expect (p.values.count (id::pitchFall) && std::abs (p.values[id::pitchFall]) < 1e-6f, "old preset: pitchFall neutral (0)");
    expect (p.values.count (id::splitFreq) && std::abs (p.values[id::splitFreq] - 1000.0f) < 1e-3f, "old preset: SPLIT at the old fixed 1 kHz");
}

void testPresetRoundTripAndWav()
{
    Snapshot s = factoryPresets()[21].values;
    s[id::width] = 73.5f;
    s[id::delay] = 4.25f;
    const auto xml = presetToXml (juce::String::fromUTF8 ("Ünïcode Test"), "User", s);
    Preset back;
    juce::String err;
    expect (presetFromXml (xml, back, err), "preset XML parses: " + err);
    expect (back.values.size() == s.size(), "preset keeps every parameter");
    expect (std::abs (back.values[id::width] - 73.5f) < 1e-4f && std::abs (back.values[id::delay] - 4.25f) < 1e-4f,
            "preset stores Stereo Width and Delay");
    expect (back.name == juce::String::fromUTF8 ("Ünïcode Test"), "preset name round-trips");
    Preset bad;
    expect (! presetFromXml ("<garbage/>", bad, err), "rejects a foreign file");
    expect (! presetFromXml ("not xml at all", bad, err), "rejects a corrupt file");

    auto dir = juce::File::getSpecialLocation (juce::File::tempDirectory).getChildFile ("powerkick-test-" + juce::String (juce::Random::getSystemRandom().nextInt()));
    {
        PresetLibrary lib (dir);
        expect (lib.saveUser ("My Kick", s, err), "save user preset: " + err);
        int idx = lib.indexOf ("My Kick");
        expect (idx >= 26, "user preset listed after factory");
        expect (lib.rename (idx, "Renamed", err), "rename: " + err);
        idx = lib.indexOf ("Renamed");
        expect (idx >= 0 && lib.indexOf ("My Kick") < 0, "renamed preset listed");
        auto exported = dir.getChildFile ("exported.pkpreset");
        expect (lib.exportTo (exported, "Renamed", s, err), "export: " + err);
        juce::String imported;
        expect (lib.importFile (exported, imported, err), "import: " + err);
        expect (imported == "Renamed 2", "import avoids name clash (" + imported + ")");
        expect (! lib.remove (0, err), "factory presets cannot be deleted");
        expect (lib.remove (lib.indexOf ("Renamed"), err), "delete user preset: " + err);
        expect (lib.indexOf ("Renamed") < 0, "deleted preset gone");
    }
    // WAV export: 24-bit, channels as requested.
    for (bool st : { true, false })
    {
        auto audio = renderKick (paramsFromSnapshot (s), 44100.0, 140.0, st);
        auto wavFile = dir.getChildFile (st ? "st.wav" : "mono.wav");
        expect (writeWav24 (wavFile, audio, 44100.0, err), "write wav: " + err);
        juce::WavAudioFormat fmt;
        std::unique_ptr<juce::AudioFormatReader> r (fmt.createReaderFor (wavFile.createInputStream().release(), true));
        expect (r != nullptr && r->bitsPerSample == 24 && (int) r->numChannels == (st ? 2 : 1) && r->sampleRate == 44100.0,
                "WAV is 24-bit with the requested channels");
    }
    dir.deleteRecursively();
}
}

int main (int argc, char** argv)
{
    // No GUI initialisation: the tests are pure DSP and file I/O (and run under Wine).
    if (argc > 1 && juce::String (argv[1]) == "--dump-presets")
    {
        // name|category|id=value ... for Tools/fit_kicks.py
        for (const auto& pr : factoryPresets())
        {
            std::cout << pr.name << "|" << pr.category << "|";
            for (const auto& [k, v] : pr.values) std::cout << k << "=" << v << " ";
            std::cout << "\n";
        }
        return 0;
    }
    if (argc > 1 && juce::String (argv[1]) == "--serve")
    {
        // Render server for Tools/fit_kicks.py. Each stdin line: "paramId=value ... _len=0.6 _sr=48000".
        // Unlisted parameters use defaults. Writes len*sr stereo frames of float32 (interleaved) to stdout.
        std::string line;
        std::vector<float> out;
        while (std::getline (std::cin, line))
        {
            Snapshot s;
            double len = 0.6, sr = 48000.0;
            std::istringstream ss (line);
            std::string tok;
            while (ss >> tok)
            {
                const auto eq = tok.find ('=');
                if (eq == std::string::npos) continue;
                const auto key = juce::String (tok.substr (0, eq));
                const float value = std::stof (tok.substr (eq + 1));
                if (key == "_len") len = value;
                else if (key == "_sr") sr = value;
                else s[key] = value;
            }
            const auto audio = stream (paramsFromSnapshot (s), sr, 512, 1);
            const int frames = std::min (audio.getNumSamples(), (int) (len * sr));
            out.resize ((size_t) frames * 2);
            for (int i = 0; i < frames; ++i)
            {
                out[(size_t) i * 2] = audio.getSample (0, i);
                out[(size_t) i * 2 + 1] = audio.getSample (1, i);
            }
            std::fwrite (out.data(), sizeof (float), out.size(), stdout);
            std::fflush (stdout);
        }
        return 0;
    }
    const bool report = argc > 1 && juce::String (argv[1]) == "--report";
    if (argc > 2 && juce::String (argv[1]) == "--export")
    {
        // Renders every factory preset to 24-bit stereo WAV for listening.
        juce::File dir (juce::String::fromUTF8 (argv[2]));
        dir.createDirectory();
        int n = 1;
        for (const auto& pr : factoryPresets())
        {
            juce::String err;
            const auto audio = renderKick (paramsFromSnapshot (pr.values), 48000.0, 140.0, true);
            const auto name = juce::String (n++).paddedLeft ('0', 2) + " " + pr.category.replace ("/", "-") + " - " + pr.name + ".wav";
            if (! writeWav24 (dir.getChildFile (name), audio, 48000.0, err))
                std::cout << err << "\n";
        }
        return 0;
    }
    testPresetLibrary();
    testPresetsRender (report);
    testWidthZeroIsTrueMono();
    testMonoSumKeepsKick();
    testSampleRatesAndBlocks();
    testNoClicksOnParameterJumps();
    testCharacterAudible();
    testPitchFall();
    testDriveSweepIsSmooth();
    testStereoControlsAreAudible();
    testStereoSplit();
    testSettledPitch();
    testGatedBody();
    testBodyTone();
    testVelocity();
    testLegacyPresetMigration();
    testPresetRoundTripAndWav();
    std::cout << (failures == 0 ? "PASS " : "FAILED ") << (checks - failures) << "/" << checks << " checks\n";
    return failures == 0 ? 0 : 1;
}
