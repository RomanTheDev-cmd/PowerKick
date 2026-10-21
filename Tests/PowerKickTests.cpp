// Headless checks of the PowerKick engine, presets and WAV export.
// Run: powerkick_tests [--report]   (--report prints per-preset levels)

#include "DSP/KickEngine.h"
#include "KickRenderer.h"
#include "Presets.h"
#include <iostream>

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
    expect (f.size() == 26, "26 factory presets, got " + juce::String ((int) f.size()));
    std::map<juce::String, int> count;
    for (const auto& p : f) ++count[p.category];
    expect (count["Techno"] == 6 && count["Hard Techno"] == 6 && count["Schranz"] == 6 && count["Industrial"] == 4
                && count["Rave / Hard Dance"] == 4,
            "preset category split 6/6/6/4/4");
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
        const double subSide = bandRms (side.data(), n, 48000.0, 20.0, 100.0);
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
            const auto a = stream (preset ("Concrete Hammer"), sr, block);
            const auto s = stats (a);
            expect (s.finite && s.peak <= 1.0 && s.peak > 0.25,
                    "sr " + juce::String (sr) + " block " + juce::String (block) + " peak " + juce::String (db (s.peak), 2));
        }
    // Same sound regardless of host buffer size.
    const auto a = stream (preset ("Hard Floor"), 48000.0, 64), b = stream (preset ("Hard Floor"), 48000.0, 512);
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
    auto p = preset ("Long Sub Techno");
    p.click = 0.0f;
    const double sr = 48000.0;
    dsp::KickEngine e;
    e.prepare (sr, 256);
    juce::AudioBuffer<float> out (2, (int) sr);
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
    auto q = preset ("Long Sub Techno");
    q.click = 0.0f;
    dsp::KickEngine e2;
    e2.prepare (sr, 256);
    juce::AudioBuffer<float> o2 (2, (int) sr / 2);
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
    auto base = preset ("Warehouse Punch");
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
    // After the sweep the body must sit on TUNE (A1 - 1 st = 51.9 Hz) at any sample rate.
    for (double sr : { 22050.0, 24000.0, 44100.0, 96000.0 })
    {
        auto p = preset ("Warehouse Punch");
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
        expect (std::abs (f - 51.91) < 1.0, "settled pitch at " + juce::String (sr) + " Hz is " + juce::String (f, 2) + " Hz");
    }
}

void testVelocity()
{
    auto p = preset ("Warehouse Punch");
    const auto loud = renderKick (p, 48000.0, 140.0, false, 1.0f);
    const auto soft = renderKick (p, 48000.0, 140.0, false, 0.4f);
    expect (stats (soft).peak < stats (loud).peak * 0.9, "velocity changes level");
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
    juce::ScopedJuceInitialiser_GUI init;
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
    testSettledPitch();
    testVelocity();
    testPresetRoundTripAndWav();
    std::cout << (failures == 0 ? "PASS " : "FAILED ") << (checks - failures) << "/" << checks << " checks\n";
    return failures == 0 ? 0 : 1;
}
