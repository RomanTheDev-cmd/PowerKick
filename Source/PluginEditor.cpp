#include "PluginEditor.h"
#include "KickRenderer.h"

using namespace pk::ui;

namespace
{
juce::Font uiFont (float size, bool bold = false)
{
    return juce::Font (juce::FontOptions (size, bold ? juce::Font::bold : juce::Font::plain));
}

void fillPanel (juce::Graphics& g, juce::Rectangle<float> r, float radius = 10.0f)
{
    g.setGradientFill (juce::ColourGradient (colours::panelTop, r.getX(), r.getY(), colours::panelBot, r.getX(), r.getBottom(), false));
    g.fillRoundedRectangle (r, radius);
    // Hairline highlight on the top edge only: light from above, no glow.
    g.setGradientFill (juce::ColourGradient (juce::Colours::white.withAlpha (0.10f), r.getX(), r.getY(),
                                             juce::Colours::white.withAlpha (0.0f), r.getX(), r.getY() + r.getHeight() * 0.5f, false));
    g.drawRoundedRectangle (r.reduced (0.5f), radius, 1.0f);
}

juce::String choiceText (juce::AudioProcessorValueTreeState& s, const juce::String& pid)
{
    if (auto* p = dynamic_cast<juce::AudioParameterChoice*> (s.getParameter (pid)))
        return p->getCurrentChoiceName().toUpperCase();
    return {};
}

void setChoice (juce::AudioProcessorValueTreeState& s, const juce::String& pid, int index)
{
    if (auto* p = dynamic_cast<juce::AudioParameterChoice*> (s.getParameter (pid)))
    {
        p->beginChangeGesture();
        p->setValueNotifyingHost (p->convertTo0to1 ((float) index));
        p->endChangeGesture();
    }
}

void setFloat (juce::AudioProcessorValueTreeState& s, const juce::String& pid, float plain)
{
    if (auto* p = s.getParameter (pid))
    {
        p->beginChangeGesture();
        p->setValueNotifyingHost (p->convertTo0to1 (plain));
        p->endChangeGesture();
    }
}
}

//==============================================================================
LookAndFeel::LookAndFeel()
{
    setColour (juce::PopupMenu::backgroundColourId, colours::panelBot);
    setColour (juce::PopupMenu::textColourId, colours::text);
    setColour (juce::PopupMenu::highlightedBackgroundColourId, colours::accent.withAlpha (0.9f));
    setColour (juce::PopupMenu::highlightedTextColourId, juce::Colour (0xff15161a));
    setColour (juce::PopupMenu::headerTextColourId, colours::dim);
    setColour (juce::TooltipWindow::backgroundColourId, colours::panelTop);
    setColour (juce::TooltipWindow::textColourId, colours::text);
    setColour (juce::TooltipWindow::outlineColourId, colours::line);
    setColour (juce::AlertWindow::backgroundColourId, colours::panelBot);
    setColour (juce::AlertWindow::textColourId, colours::text);
    setColour (juce::AlertWindow::outlineColourId, colours::line);
    setColour (juce::TextEditor::backgroundColourId, colours::bgBottom);
    setColour (juce::TextEditor::textColourId, colours::text);
    setColour (juce::TextEditor::outlineColourId, colours::line);
    setColour (juce::TextEditor::focusedOutlineColourId, colours::accent);
    setColour (juce::TextButton::buttonColourId, colours::panelTop);
    setColour (juce::TextButton::textColourOffId, colours::text);
    setColour (juce::Slider::textBoxTextColourId, colours::text);
}

void LookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h, float pos, float start, float end,
                                    juce::Slider& s)
{
    const auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (4.0f);
    const float radius = std::min (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const auto c = bounds.getCentre();
    const float track = std::max (2.5f, radius * 0.09f);
    const float angle = start + pos * (end - start);
    const bool enabled = s.isEnabled();

    // Face: soft vertical gradient, lighter at the top.
    const float face = radius - track * 2.2f;
    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff484b53), c.x, c.y - face, juce::Colour (0xff202227), c.x, c.y + face, false));
    g.fillEllipse (c.x - face, c.y - face, face * 2.0f, face * 2.0f);
    g.setColour (juce::Colours::white.withAlpha (0.07f));
    g.drawEllipse (c.x - face, c.y - face, face * 2.0f, face * 2.0f, 1.0f);

    juce::Path bg, val;
    const float r = radius - track * 0.5f;
    bg.addCentredArc (c.x, c.y, r, r, 0.0f, start, end, true);
    g.setColour (juce::Colours::white.withAlpha (0.09f));
    g.strokePath (bg, juce::PathStrokeType (track, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Bipolar parameters (TUNE) draw from the centre.
    const bool bipolar = s.getMinimum() < 0.0 && s.getMaximum() > 0.0 && s.getProperties().contains ("bipolar");
    const float from = bipolar ? (start + end) * 0.5f : start;
    if (std::abs (angle - from) > 0.001f)
    {
        val.addCentredArc (c.x, c.y, r, r, 0.0f, std::min (from, angle), std::max (from, angle), true);
        if (enabled)
            g.setGradientFill (juce::ColourGradient (colours::accentLo, c.x - radius, c.y + radius, colours::accent, c.x + radius, c.y - radius, false));
        else
            g.setColour (colours::faint);
        g.strokePath (val, juce::PathStrokeType (track, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    // Pointer: a short line on the face.
    const auto tip = c.getPointOnCircumference (face * 0.78f, angle);
    const auto base = c.getPointOnCircumference (face * 0.38f, angle);
    g.setColour (enabled ? colours::text : colours::faint);
    g.drawLine ({ base, tip }, std::max (2.0f, radius * 0.06f));
}

void LookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& b, const juce::Colour&, bool over, bool down)
{
    auto r = b.getLocalBounds().toFloat().reduced (0.5f);
    const bool on = b.getToggleState();
    const float radius = r.getHeight() * 0.5f;
    if (on)
    {
        g.setGradientFill (juce::ColourGradient (colours::accent.withAlpha (down ? 0.8f : 1.0f), r.getX(), r.getY(),
                                                 juce::Colour (0xffb9bcc2), r.getX(), r.getBottom(), false));
        g.fillRoundedRectangle (r, radius);
        return;
    }
    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff3a3d45).brighter (over ? 0.12f : 0.0f), r.getX(), r.getY(),
                                             juce::Colour (0xff25272c), r.getX(), r.getBottom(), false));
    g.fillRoundedRectangle (r, radius);
    g.setColour (juce::Colours::white.withAlpha (over ? 0.16f : 0.08f));
    g.drawRoundedRectangle (r, radius, 1.0f);
}

void LookAndFeel::drawButtonText (juce::Graphics& g, juce::TextButton& b, bool, bool)
{
    g.setFont (getTextButtonFont (b, b.getHeight()));
    g.setColour (b.getToggleState() ? juce::Colour (0xff17181c) : (b.isEnabled() ? colours::text : colours::faint));
    g.drawFittedText (b.getButtonText(), b.getLocalBounds().reduced (6, 0), juce::Justification::centred, 1);
}

void LookAndFeel::drawPopupMenuBackground (juce::Graphics& g, int w, int h)
{
    g.setGradientFill (juce::ColourGradient (colours::panelTop, 0, 0, colours::panelBot, 0, (float) h, false));
    g.fillAll();
    g.setColour (colours::line);
    g.drawRect (0, 0, w, h);
}

juce::Font LookAndFeel::getTextButtonFont (juce::TextButton&, int h) { return uiFont (std::min (13.0f, h * 0.48f), true).withExtraKerningFactor (0.06f); }
juce::Font LookAndFeel::getPopupMenuFont() { return uiFont (14.0f); }

//==============================================================================
Knob::Knob (juce::AudioProcessorValueTreeState& s, const juce::String& paramId, const juce::String& t,
            const juce::String& tooltip, bool sm)
    : state (s), title (t), small (sm)
{
    slider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    slider.setRotaryParameters (juce::MathConstants<float>::pi * 1.25f, juce::MathConstants<float>::pi * 2.75f, true);
    slider.setMouseDragSensitivity (220);
    slider.setVelocityModeParameters (0.4, 1, 0.0, false, juce::ModifierKeys::shiftModifier); // shift = fine
    slider.setTooltip (tooltip);
    slider.onValueChange = [this] { repaint(); };
    addAndMakeVisible (slider);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (s, paramId, slider);
    if (auto* p = s.getParameter (paramId))
        slider.setDoubleClickReturnValue (true, (double) dynamic_cast<juce::RangedAudioParameter*> (p)->convertFrom0to1 (p->getDefaultValue()));
    setTooltip (tooltip);
}

void Knob::setSelector (const juce::String& choiceParamId) { selectorId = choiceParamId; }

void Knob::resized()
{
    auto r = getLocalBounds();
    r.removeFromTop (small ? 16 : 20);
    auto bottom = r.removeFromBottom (selectorId.isNotEmpty() ? 34 : 18);
    selectorArea = selectorId.isNotEmpty() ? bottom.removeFromBottom (16) : juce::Rectangle<int>();
    const int d = std::min (r.getWidth(), r.getHeight());
    slider.setBounds (r.withSizeKeepingCentre (d, d));
}

void Knob::paint (juce::Graphics& g)
{
    const float alpha = slider.isEnabled() ? 1.0f : 0.4f;
    auto r = getLocalBounds();
    g.setColour (colours::dim.withMultipliedAlpha (alpha));
    g.setFont (uiFont (small ? 11.0f : 12.0f, true).withExtraKerningFactor (0.14f));
    g.drawText (title, r.removeFromTop (small ? 16 : 20), juce::Justification::centred);

    auto bottom = r.removeFromBottom (selectorId.isNotEmpty() ? 34 : 18);
    g.setColour (colours::text.withMultipliedAlpha (alpha));
    g.setFont (uiFont (small ? 12.0f : 13.0f));
    g.drawText (slider.getTextFromValue (slider.getValue()), bottom.removeFromTop (18), juce::Justification::centred);

    if (selectorId.isNotEmpty())
    {
        const auto text = choiceText (state, selectorId) + juce::String::fromUTF8 (" \xe2\x96\xbe");
        g.setFont (uiFont (10.5f, true).withExtraKerningFactor (0.1f));
        g.setColour (colours::text.withMultipliedAlpha (alpha * 0.85f));
        g.drawText (text, selectorArea, juce::Justification::centred);
    }
}

void Knob::mouseUp (const juce::MouseEvent& e)
{
    if (selectorId.isEmpty() || ! selectorArea.contains (e.getPosition()))
        return;
    auto* p = dynamic_cast<juce::AudioParameterChoice*> (state.getParameter (selectorId));
    if (p == nullptr)
        return;
    juce::PopupMenu m;
    for (int i = 0; i < p->choices.size(); ++i)
        m.addItem (i + 1, p->choices[i], true, i == p->getIndex());
    m.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (this).withTargetScreenArea (localAreaToGlobal (selectorArea)),
                     [this, id = selectorId] (int r)
                     {
                         if (r > 0) { setChoice (state, id, r - 1); repaint(); }
                     });
}

//==============================================================================
Segment::Segment (juce::AudioProcessorValueTreeState& s, const juce::String& pid, const juce::StringArray& labels,
                  const juce::String& tooltip)
    : state (s), paramId (pid)
{
    for (int i = 0; i < labels.size(); ++i)
    {
        auto* b = buttons.add (new juce::TextButton (labels[i]));
        b->setTooltip (tooltip);
        b->onClick = [this, i]
        {
            if (auto* p = dynamic_cast<juce::RangedAudioParameter*> (state.getParameter (paramId)))
            {
                p->beginChangeGesture();
                p->setValueNotifyingHost (p->convertTo0to1 ((float) i));
                p->endChangeGesture();
            }
        };
        addAndMakeVisible (b);
    }
    state.addParameterListener (paramId, this);
    handleAsyncUpdate();
}

Segment::~Segment() { state.removeParameterListener (paramId, this); }

void Segment::resized()
{
    auto r = getLocalBounds();
    const int w = r.getWidth() / std::max (1, buttons.size());
    for (auto* b : buttons)
        b->setBounds (r.removeFromLeft (w).reduced (2, 0));
}

void Segment::handleAsyncUpdate()
{
    const int index = juce::roundToInt (state.getRawParameterValue (paramId)->load());
    for (int i = 0; i < buttons.size(); ++i)
        buttons[i]->setToggleState (i == index, juce::dontSendNotification);
}

//==============================================================================
AdvancedPanel::AdvancedPanel (juce::AudioProcessorValueTreeState& s)
    : state (s),
      pitchDepth (s, pk::id::pitchDepth, "DEPTH", "How far above TUNE the body starts (semitones). Less = less laser, more kick", true),
      pitchTime (s, pk::id::pitchTime, "TIME", "How fast the pitch drops to TUNE. Long times sound like a zap or a tom", true),
      pitchCurve (s, pk::id::pitchCurve, "CURVE", "Shape of the drop: 0 = fast exponential, 1 = rounder and slower", true),
      pitchFall (s, pk::id::pitchFall, "FALL", "Slow extra fall through the tail, as measured on hard-techno kicks", true),
      keyRoot (s, pk::id::keyRoot, "KEY ROOT", "With key tracking on: the MIDI note that plays the kick at TUNE", true),
      clickTone (s, pk::id::clickTone, "TONE", "Colour of the click: darker <-> brighter", true),
      rumbleTime (s, pk::id::rumbleTime, "TIME", "Length and darkness of the rumble tail", true),
      bodyTone (s, pk::id::bodyTone, "TONE", "Mid scoop of the body after the attack: 0 = deep scoop, 100 = none", true),
      subLevel (s, pk::id::subLevel, "SUB", "Clean sine under the driven body, same pitch, never distorted", true),
      startPhase (s, pk::id::startPhase, "PHASE", "Start of the body waveform (0-90 degrees): more push on the first cycle", true),
      driveEnv (s, pk::id::driveEnv, "ATTACK", "Extra drive on the attack (up to +24 dB), fading over about 30 ms", true),
      velDrive (s, pk::id::velDrive, "VELOCITY", "Softer notes drive less", true),
      modDrive (s, pk::id::modDrive, "MOD", "Mod wheel adds drive (up to +24 dB)", true),
      velSens (s, pk::id::velSens, "VELOCITY", "How much note velocity changes the level and the click", true),
      beatLength (s, pk::id::beatLength, { "OFF", "1/4", "1/8", "1/16" }, "The kick ends within this note value at the current tempo"),
      rumbleMode (s, pk::id::rumbleMode, { "CLASSIC", "REVERB" }, "Rumble engine: tuned comb tail or a diffuse dark reverb"),
      choke (s, pk::id::choke, { "OFF", "ON" }, "A new hit cuts the previous one in 1 ms instead of 4 ms (tight rolls)"),
      oversample (s, pk::id::oversample, { "2x", "4x", "8x" }, "Oversampling of the drive: higher = cleaner top, more CPU")
{
    for (auto* c : std::initializer_list<juce::Component*> { &pitchDepth, &pitchTime, &pitchCurve, &pitchFall, &keyRoot, &clickTone,
                                                            &rumbleTime, &bodyTone, &subLevel, &startPhase, &driveEnv, &velDrive,
                                                            &modDrive, &velSens, &beatLength, &rumbleMode, &choke, &oversample,
                                                            &closeButton })
        addAndMakeVisible (c);
    closeButton.setTooltip ("Back to the main controls");
    closeButton.onClick = [this] { if (onClose) onClose(); };
}

void AdvancedPanel::refresh()
{
    keyRoot.slider.setEnabled (state.getRawParameterValue (pk::id::keyTrack)->load() > 0.5f);
}

void AdvancedPanel::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat();
    g.setColour (juce::Colours::black.withAlpha (0.35f));
    g.fillRoundedRectangle (r.translated (0, 4), 14.0f);
    g.setGradientFill (juce::ColourGradient (colours::panelTop, 0, r.getY(), colours::panelBot, 0, r.getBottom(), false));
    g.fillRoundedRectangle (r, 14.0f);
    g.setColour (colours::accent.withAlpha (0.35f));
    g.drawRoundedRectangle (r.reduced (0.5f), 14.0f, 1.0f);
    const float s = getWidth() / 872.0f;
    g.setFont (uiFont (11.0f * s, true).withExtraKerningFactor (0.18f));
    for (const auto& l : labels)
    {
        g.setColour (l.text == "ADVANCED" ? colours::text : colours::dim);
        g.drawText (l.text, l.area, juce::Justification::centredLeft);
        if (l.text != "ADVANCED" && l.area.getX() < getWidth() * 0.72f) // group captions get a hairline
        {
            const int tw = juce::GlyphArrangement::getStringWidthInt (g.getCurrentFont(), l.text) + juce::roundToInt (8 * s);
            g.setColour (colours::line);
            g.fillRect (juce::Rectangle<int> (l.area.getX() + tw, l.area.getCentreY(), std::max (0, l.area.getWidth() - tw), 1));
        }
    }
    // Hairline between the knob grid and the switches.
    g.setColour (colours::line);
    g.fillRect (juce::Rectangle<float> ((float) getWidth() * 0.735f, 40.0f * s, 1.0f, (float) getHeight() - 56.0f * s));
}

void AdvancedPanel::resized()
{
    const float s = getWidth() / 872.0f;
    auto S = [s] (float v) { return juce::roundToInt (v * s); };
    labels.clear();
    auto r = getLocalBounds().reduced (S (16), S (10));
    auto top = r.removeFromTop (S (24));
    closeButton.setBounds (top.removeFromRight (S (72)).withSizeKeepingCentre (S (72), S (22)));
    labels.push_back ({ "ADVANCED", top });

    auto grid = r.removeFromLeft (juce::roundToInt (r.getWidth() * 0.735f) - S (16));
    r.removeFromLeft (S (28));
    // Two rows of seven knobs under group captions.
    struct Group { const char* caption; std::vector<Knob*> knobs; };
    const std::vector<Group> rowsA { { "PITCH", { &pitchDepth, &pitchTime, &pitchCurve, &pitchFall, &keyRoot } },
                                     { "CLICK / RUMBLE", { &clickTone, &rumbleTime } } };
    const std::vector<Group> rowsB { { "BODY", { &bodyTone, &subLevel, &startPhase } },
                                     { "DRIVE", { &driveEnv, &velDrive, &modDrive } },
                                     { "LEVEL", { &velSens } } };
    const int rowH = grid.getHeight() / 2;
    const int colW = grid.getWidth() / 7;
    for (const auto* row : { &rowsA, &rowsB })
    {
        auto band = grid.removeFromTop (rowH);
        auto captions = band.removeFromTop (S (18));
        for (const auto& gr : *row)
        {
            const int w = colW * (int) gr.knobs.size();
            labels.push_back ({ gr.caption, captions.removeFromLeft (w).reduced (S (6), 0) });
            auto cells = band.removeFromLeft (w);
            for (auto* k : gr.knobs)
                k->setBounds (cells.removeFromLeft (colW).reduced (S (3), S (2)));
        }
    }

    // Switches, each with its caption above.
    const char* captions[4] = { "LENGTH (BEATS)", "RUMBLE MODE", "CHOKE", "OVERSAMPLING" };
    Segment* segs[4] = { &beatLength, &rumbleMode, &choke, &oversample };
    const int slot = r.getHeight() / 4;
    for (int i = 0; i < 4; ++i)
    {
        auto cell = r.removeFromTop (slot).reduced (0, S (6));
        labels.push_back ({ captions[i], cell.removeFromTop (S (16)) });
        segs[i]->setBounds (cell.withSizeKeepingCentre (cell.getWidth(), std::min (cell.getHeight(), S (26))).withY (cell.getY() + S (2)));
    }
}

//==============================================================================
WaveformView::WaveformView (PowerKickProcessor& p) : proc (p)
{
    setTooltip ("Drag this waveform into your DAW to drop the kick as a 24-bit WAV");
    setMouseCursor (juce::MouseCursor::DraggingHandCursor);
}

void WaveformView::setAudio (const juce::AudioBuffer<float>& mono, double sampleRate)
{
    const int columns = 600;
    minV.assign (columns, 0.0f);
    maxV.assign (columns, 0.0f);
    const int n = mono.getNumSamples();
    lengthMs = 1000.0 * n / sampleRate;
    // Fixed 1.2 s window so lengths are comparable between presets.
    const double window = std::max (0.5 * sampleRate, n * 1.08);
    for (int c = 0; c < columns; ++c)
    {
        const int a = (int) (c * window / columns), b = std::min (n, (int) ((c + 1) * window / columns));
        if (a >= n) break;
        const auto range = juce::FloatVectorOperations::findMinAndMax (mono.getReadPointer (0, a), std::max (1, b - a));
        minV[(size_t) c] = range.getStart();
        maxV[(size_t) c] = range.getEnd();
    }
    repaint();
}

void WaveformView::decay()
{
    if (flashLevel > 0.0f)
    {
        flashLevel = std::max (0.0f, flashLevel - 0.08f);
        repaint();
    }
}

void WaveformView::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat();
    fillPanel (g, r, 12.0f);
    auto plot = r.reduced (18.0f, 16.0f);
    const float mid = plot.getCentreY();

    g.setColour (juce::Colours::white.withAlpha (0.06f));
    g.drawHorizontalLine ((int) mid, plot.getX(), plot.getRight());
    // 100 ms grid over the displayed window.
    const double windowMs = std::max (500.0, lengthMs * 1.08);
    for (double t = 100.0; t < windowMs; t += 100.0)
    {
        const float x = plot.getX() + plot.getWidth() * (float) (t / windowMs);
        g.drawVerticalLine ((int) x, plot.getY() + 4, plot.getBottom() - 4);
    }

    if (! maxV.empty())
    {
        juce::Path p;
        const auto cols = (int) maxV.size();
        const float half = plot.getHeight() * 0.48f;
        p.startNewSubPath (plot.getX(), mid);
        for (int c = 0; c < cols; ++c)
            p.lineTo (plot.getX() + plot.getWidth() * c / (cols - 1.0f), mid - maxV[(size_t) c] * half);
        for (int c = cols - 1; c >= 0; --c)
            p.lineTo (plot.getX() + plot.getWidth() * c / (cols - 1.0f), mid - minV[(size_t) c] * half);
        p.closeSubPath();
        // Warm white at the centre fading into graphite at the peaks.
        juce::ColourGradient fill (juce::Colour (0xff6f727a), 0, mid - half, juce::Colour (0xff6f727a), 0, mid + half, false);
        fill.addColour (0.5, colours::accent);
        g.setGradientFill (fill);
        g.fillPath (p);
        if (flashLevel > 0.0f)
        {
            g.setColour (juce::Colours::white.withAlpha (0.25f * flashLevel));
            g.fillPath (p);
        }
    }

    g.setFont (uiFont (11.0f, true).withExtraKerningFactor (0.1f));
    g.setColour (colours::dim);
    g.drawText (juce::String (juce::roundToInt (lengthMs)) + " MS", r.reduced (14.0f, 10.0f).removeFromTop (14.0f),
                juce::Justification::topRight);
    if (hover)
    {
        g.setColour (colours::text.withAlpha (0.8f));
        g.drawText (juce::String::fromUTF8 ("DRAG TO DAW \xe2\x86\x97"), r.reduced (14.0f, 10.0f).removeFromTop (14.0f),
                    juce::Justification::topLeft);
    }
}

void WaveformView::mouseDrag (const juce::MouseEvent& e)
{
    if (dragStarted || e.getDistanceFromDragStart() < 6)
        return;
    dragStarted = true;
    const auto params = pk::readParams (proc.state);
    const double sr = proc.getCurrentSampleRate();
    const auto audio = pk::renderKick (params, sr, proc.getHostBpm(), true);
    auto dir = juce::File::getSpecialLocation (juce::File::tempDirectory).getChildFile ("PowerKick Drag");
    dir.createDirectory();
    auto file = dir.getChildFile (juce::File::createLegalFileName ("PowerKick - " + proc.getPresetName()) + ".wav");
    juce::String err;
    if (pk::writeWav24 (file, audio, sr, err))
        juce::DragAndDropContainer::performExternalDragDropOfFiles ({ file.getFullPathName() }, false, this);
}

//==============================================================================
void Meter::setLevels (float l, float r)
{
    const float in[2] = { l, r };
    bool changed = false;
    for (int c = 0; c < 2; ++c)
    {
        const float v = std::max (in[c], levels[c] * 0.82f);
        changed |= std::abs (v - levels[c]) > 0.001f;
        levels[c] = v;
        if (in[c] >= held[c]) { held[c] = in[c]; holdFrames[c] = 45; }
        else if (--holdFrames[c] <= 0) held[c] = std::max (0.0f, held[c] - 0.02f);
    }
    if (changed || held[0] > 0.0f || held[1] > 0.0f)
        repaint();
}

void Meter::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat();
    const float bw = (r.getWidth() - 4.0f) / 2.0f;
    auto toY = [&r] (float lin)
    {
        const float dbv = juce::jlimit (-48.0f, 0.0f, juce::Decibels::gainToDecibels (lin, -48.0f));
        return r.getBottom() - r.getHeight() * (dbv + 48.0f) / 48.0f;
    };
    for (int c = 0; c < 2; ++c)
    {
        auto bar = juce::Rectangle<float> (r.getX() + c * (bw + 4.0f), r.getY(), bw, r.getHeight());
        g.setColour (juce::Colours::white.withAlpha (0.06f));
        g.fillRoundedRectangle (bar, 2.0f);
        const float top = toY (levels[c]);
        juce::ColourGradient grad (colours::accent, 0, r.getY(), colours::accentLo.withAlpha (0.8f), 0, r.getBottom(), false);
        g.setGradientFill (grad);
        g.fillRoundedRectangle (bar.withTop (top), 2.0f);
        const float hy = toY (held[c]);
        g.setColour (held[c] >= 0.999f ? colours::hot : colours::text);
        g.fillRect (bar.getX(), hy - 1.0f, bar.getWidth(), 2.0f);
    }
}

//==============================================================================
PowerKickEditor::PowerKickEditor (PowerKickProcessor& p)
    : AudioProcessorEditor (p), proc (p), waveform (p),
      tune (p.state, pk::id::tune, "TUNE", "Root note of the kick body (the pitch after the sweep)"),
      decay (p.state, pk::id::decay, "DECAY", "Length of the kick"),
      punch (p.state, pk::id::punch, "PUNCH", "Strength of the initial low hit and pitch drop"),
      body (p.state, pk::id::body, "BODY", "Density: how long the body holds and how full it sounds"),
      click (p.state, pk::id::click, "CLICK", "Level of the top click; pick its type below"),
      drive (p.state, pk::id::drive, "DRIVE", "Overdrive amount; the low end is protected. Pick the algorithm below"),
      character (p.state, pk::id::character, "CHARACTER", "Intensity of the selected colour: Deep, Punchy, Dark, Hard, Raw"),
      rumble (p.state, pk::id::rumble, "RUMBLE", "Tempo-synced rumble tail, ducked under every hit"),
      output (p.state, pk::id::output, "OUTPUT", "Output level"),
      width (p.state, pk::id::width, "WIDTH", "Stereo width above SPLIT; the sub always stays mono", true),
      delay (p.state, pk::id::delay, "DELAY", "Time offset between left and right in the widened part (needs WIDTH above 0)", true),
      split (p.state, pk::id::splitFreq, "SPLIT", "Widening starts above this frequency; the sub below 120 Hz always stays mono", true),
      advanced (p.state)
{
    setLookAndFeel (&lnf);
    tune.slider.getProperties().set ("bipolar", true);
    output.slider.getProperties().set ("bipolar", true);
    click.setSelector (pk::id::clickType);
    drive.setSelector (pk::id::driveMode);
    rumble.setSelector (pk::id::rumbleSync);

    for (auto* k : { &tune, &decay, &punch, &body, &click, &drive, &character, &rumble, &output, &width, &delay, &split })
        addAndMakeVisible (*k);
    for (auto* b : { &prevButton, &nextButton, &presetButton, &aButton, &bButton, &copyButton, &menuButton, &previewButton,
                     &exportButton, &variationButton, &undoButton, &advancedButton })
        addAndMakeVisible (*b);
    addAndMakeVisible (waveform);
    addAndMakeVisible (meter);

    static const char* modeNames[5] = { "DEEP", "PUNCHY", "DARK", "HARD", "RAW" };
    static const char* modeTips[5] = { "Deep and massive", "Punchy and accented", "Dark and heavy", "Hard, harmonically saturated",
                                       "Raw and industrial" };
    for (int i = 0; i < 5; ++i)
    {
        auto& b = charButtons[(size_t) i];
        b.setButtonText (modeNames[i]);
        b.setTooltip (modeTips[i]);
        b.setClickingTogglesState (false);
        b.onClick = [this, i] { setChoice (proc.state, pk::id::charMode, i); };
        addAndMakeVisible (b);
    }
    addChildComponent (advanced); // last: lies on top of the knob area when open

    prevButton.setTooltip ("Previous preset");
    nextButton.setTooltip ("Next preset");
    presetButton.setTooltip ("Browse presets");
    aButton.setTooltip ("Compare: setting A");
    bButton.setTooltip ("Compare: setting B");
    copyButton.setTooltip ("Copy A into B");
    menuButton.setTooltip ("Preset file actions, Mono Check and options");
    previewButton.setTooltip ("Play the kick");
    exportButton.setTooltip ("Save the kick as a 24-bit WAV");
    variationButton.setTooltip ("New timbre variation at the same key and loudness");
    undoButton.setTooltip ("Undo the last variation");
    advancedButton.setTooltip ("Pitch envelope, body, drive dynamics, length and engine options");
    advancedButton.setClickingTogglesState (true);
    advancedButton.onClick = [this] { advanced.setVisible (advancedButton.getToggleState()); advanced.refresh(); };
    advanced.onClose = [this] { advancedButton.setToggleState (false, juce::sendNotification); };

    prevButton.onClick = [this] { stepPreset (-1); };
    nextButton.onClick = [this] { stepPreset (1); };
    presetButton.onClick = [this] { showPresetMenu(); };
    menuButton.onClick = [this] { showMainMenu(); };
    aButton.onClick = [this] { proc.switchSlot (false); refreshHeader(); };
    bButton.onClick = [this] { proc.switchSlot (true); refreshHeader(); };
    copyButton.onClick = [this] { proc.copyAToB(); refreshHeader(); };
    previewButton.onClick = [this] { proc.triggerRequested = true; };
    exportButton.onClick = [this] { showExportMenu(); };
    variationButton.onClick = [this] { proc.makeVariation(); refreshHeader(); };
    undoButton.onClick = [this] { proc.undoVariation(); refreshHeader(); };

    for (const auto& pid : pk::allParamIds())
        proc.state.addParameterListener (pid, this);

    setResizable (true, true);
    setResizeLimits (736, 528, 1380, 990);
    getConstrainer()->setFixedAspectRatio (920.0 / 660.0);
    setSize (920, 660);
    refreshHeader();
    startTimerHz (30);
}

PowerKickEditor::~PowerKickEditor()
{
    for (const auto& pid : pk::allParamIds())
        proc.state.removeParameterListener (pid, this);
    setLookAndFeel (nullptr);
}

void PowerKickEditor::paint (juce::Graphics& g)
{
    const auto r = getLocalBounds().toFloat();
    g.setGradientFill (juce::ColourGradient (colours::bgTop, 0, 0, colours::bgBottom, 0, r.getHeight(), false));
    g.fillAll();
    // A faint warm light from the upper left keeps the dark from feeling flat.
    g.setGradientFill (juce::ColourGradient (juce::Colour (0x14fff1e6), r.getWidth() * 0.15f, 0.0f,
                                             juce::Colour (0x00fff1e6), r.getWidth() * 0.55f, r.getHeight() * 0.6f, true));
    g.fillAll();

    const float s = getWidth() / 920.0f;
    // Wordmark
    auto header = getLocalBounds().removeFromTop (juce::roundToInt (60 * s)).reduced (juce::roundToInt (24 * s), 0);
    g.setFont (uiFont (19.0f * s, true).withExtraKerningFactor (0.08f));
    g.setColour (colours::text);
    g.drawText ("POWER", header, juce::Justification::centredLeft);
    const int powerW = juce::GlyphArrangement::getStringWidthInt (uiFont (19.0f * s, true).withExtraKerningFactor (0.08f), "POWER");
    g.setColour (colours::accentLo);
    g.drawText ("KICK", header.withTrimmedLeft (powerW + 2), juce::Justification::centredLeft);

    g.setColour (colours::line);
    g.drawHorizontalLine (juce::roundToInt (60 * s), 0.0f, (float) getWidth());

    if (proc.monoCheck.load())
    {
        g.setColour (colours::accent);
        g.fillRoundedRectangle (monoBadge.toFloat(), monoBadge.getHeight() * 0.5f);
        g.setColour (juce::Colour (0xff17181c));
        g.setFont (uiFont (11.0f * s, true).withExtraKerningFactor (0.1f));
        g.drawText ("MONO CHECK", monoBadge, juce::Justification::centred);
    }

    // Section captions
    g.setFont (uiFont (10.5f * s, true).withExtraKerningFactor (0.18f));
    g.setColour (colours::faint);
    g.drawText ("CHARACTER", charButtons[0].getBounds().withWidth (juce::roundToInt (90 * s)).translated (juce::roundToInt (-100 * s), 0),
                juce::Justification::centredRight);
    g.drawText ("STEREO", footerArea.withWidth (juce::roundToInt (60 * s)), juce::Justification::centredLeft);
}

void PowerKickEditor::resized()
{
    const float s = getWidth() / 920.0f;
    auto S = [s] (float v) { return juce::roundToInt (v * s); };
    auto r = getLocalBounds();

    // Header
    auto header = r.removeFromTop (S (60)).reduced (S (24), S (14));
    menuButton.setBounds (header.removeFromRight (S (40)));
    header.removeFromRight (S (14));
    copyButton.setBounds (header.removeFromRight (S (50)));
    header.removeFromRight (S (6));
    bButton.setBounds (header.removeFromRight (S (34)));
    header.removeFromRight (S (4));
    aButton.setBounds (header.removeFromRight (S (34)));
    monoBadge = header.removeFromRight (S (112)).reduced (S (8), S (2));
    auto centre = getLocalBounds().removeFromTop (S (60)).withSizeKeepingCentre (S (330), S (32));
    prevButton.setBounds (centre.removeFromLeft (S (32)));
    nextButton.setBounds (centre.removeFromRight (S (32)));
    presetButton.setBounds (centre.reduced (S (6), 0));

    r.reduce (S (24), S (14));
    // Display: waveform with transport, output column on the right.
    auto display = r.removeFromTop (S (180));
    auto outCol = display.removeFromRight (S (120));
    display.removeFromRight (S (16));
    auto transport = display.removeFromBottom (S (34));
    display.removeFromBottom (S (8));
    waveform.setBounds (display);
    previewButton.setBounds (transport.removeFromLeft (S (120)));
    exportButton.setBounds (transport.removeFromRight (S (130)));
    output.setBounds (outCol.removeFromTop (S (128)));
    meter.setBounds (outCol.withSizeKeepingCentre (S (22), outCol.getHeight()).reduced (0, S (2)));

    r.removeFromTop (S (12));
    advanced.setBounds (r.withHeight (S (302)));
    // Knobs: two rows of four.
    auto rows = r.removeFromTop (S (262));
    const int rowH = rows.getHeight() / 2;
    auto row1 = rows.removeFromTop (rowH), row2 = rows;
    const int colW = row1.getWidth() / 4;
    Knob* top[4] = { &tune, &decay, &punch, &body };
    Knob* bot[4] = { &click, &drive, &character, &rumble };
    for (int i = 0; i < 4; ++i)
    {
        top[i]->setBounds (row1.removeFromLeft (colW).reduced (S (10), S (2)));
        bot[i]->setBounds (row2.removeFromLeft (colW).reduced (S (10), S (2)));
    }

    // Character mode segment, centred.
    auto modes = r.removeFromTop (S (40)).withSizeKeepingCentre (S (440), S (28));
    const int mw = modes.getWidth() / 5;
    for (auto& b : charButtons)
        b.setBounds (modes.removeFromLeft (mw).reduced (S (3), 0));

    // Footer: stereo on the left, variation on the right.
    footerArea = r.withTrimmedTop (S (6));
    auto f = footerArea;
    f.removeFromLeft (S (64));
    width.setBounds (f.removeFromLeft (S (84)));
    delay.setBounds (f.removeFromLeft (S (84)));
    split.setBounds (f.removeFromLeft (S (84)));
    auto var = f.removeFromRight (S (330)).withSizeKeepingCentre (S (330), S (30));
    undoButton.setBounds (var.removeFromRight (S (72)));
    var.removeFromRight (S (8));
    variationButton.setBounds (var.removeFromRight (S (140)));
    var.removeFromRight (S (8));
    advancedButton.setBounds (var);
}

void PowerKickEditor::timerCallback()
{
    meter.setLevels (proc.meterL.load(), proc.meterR.load());
    waveform.decay();
    if (const int h = proc.hitCounter.load(); h != lastHit)
    {
        lastHit = h;
        waveform.flash();
    }

    const int charIndex = juce::roundToInt (proc.state.getRawParameterValue (pk::id::charMode)->load());
    for (int i = 0; i < 5; ++i)
        charButtons[(size_t) i].setToggleState (i == charIndex, juce::dontSendNotification);
    const bool stereoOn = proc.state.getRawParameterValue (pk::id::width)->load() > 0.0f;
    delay.slider.setEnabled (stereoOn);
    split.slider.setEnabled (stereoOn);
    undoButton.setEnabled (! proc.variationUndo.empty());
    if (advanced.isVisible())
        advanced.refresh();

    // Re-render the preview waveform once the knobs settle.
    if (soundDirty)
    {
        dirtyAge = 0;
        soundDirty = false;
        repaint (monoBadge);
        for (auto* k : { &click, &drive, &rumble }) k->repaint();
    }
    if (dirtyAge >= 0 && ++dirtyAge > 3)
    {
        dirtyAge = -1;
        auto params = pk::readParams (proc.state);
        waveform.setAudio (pk::renderKick (params, 24000.0, proc.getHostBpm(), false), 24000.0);
    }
}

//==============================================================================
void PowerKickEditor::refreshHeader()
{
    presetButton.setButtonText (proc.getPresetName());
    aButton.setToggleState (! proc.slotBActive, juce::dontSendNotification);
    bButton.setToggleState (proc.slotBActive, juce::dontSendNotification);
    copyButton.setEnabled (! proc.slotBActive);
    repaint();
}

void PowerKickEditor::stepPreset (int delta)
{
    const auto& all = proc.library.all();
    if (all.empty()) return;
    int i = proc.library.indexOf (proc.getPresetName());
    i = (i < 0 ? 0 : i + delta + (int) all.size()) % (int) all.size();
    proc.loadPreset (i);
    refreshHeader();
}

void PowerKickEditor::showPresetMenu()
{
    proc.library.rescan();
    const auto& all = proc.library.all();
    juce::PopupMenu menu;
    juce::StringArray cats;
    for (const auto& p : all)
        cats.addIfNotAlreadyThere (p.category);
    for (const auto& c : cats)
    {
        // Category -> optional sub-group -> preset (300 presets stay browsable).
        juce::StringArray groups;
        for (const auto& p : all)
            if (p.category == c) groups.addIfNotAlreadyThere (p.group);
        juce::PopupMenu sub;
        const bool nested = groups.size() > 1;
        for (const auto& g : groups)
        {
            juce::PopupMenu items;
            bool ticked = false;
            for (size_t i = 0; i < all.size(); ++i)
                if (all[i].category == c && all[i].group == g)
                {
                    const bool current = all[i].name == proc.getPresetName();
                    ticked = ticked || current;
                    (nested && g.isNotEmpty() ? items : sub).addItem ((int) i + 1, all[i].name, true, current);
                }
            if (nested && g.isNotEmpty())
                sub.addSubMenu (g, items, true, nullptr, ticked);
        }
        menu.addSubMenu (c, sub);
    }
    menu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (&presetButton),
                        [this] (int r) { if (r > 0) { proc.loadPreset (r - 1); refreshHeader(); } });
}

void PowerKickEditor::showMainMenu()
{
    const int current = proc.library.indexOf (proc.getPresetName());
    const bool isUser = current >= 0 && ! proc.library.all()[(size_t) current].isFactory();
    const bool keyTrack = proc.state.getRawParameterValue (pk::id::keyTrack)->load() > 0.5f;

    juce::PopupMenu menu, bpm, vel;
    menu.addSectionHeader ("PRESET");
    menu.addItem (1, "Save as...");
    menu.addItem (2, "Rename...", isUser);
    menu.addItem (3, "Delete...", isUser);
    menu.addItem (4, "Import...");
    menu.addItem (5, "Export...");
    menu.addItem (6, "Show preset folder");
    menu.addSectionHeader ("LISTEN");
    menu.addItem (10, "Mono Check", true, proc.monoCheck.load());
    menu.addSectionHeader ("PLAY");
    menu.addItem (11, "Key tracking (chromatic)", true, keyTrack);
    const float vs = proc.state.getRawParameterValue (pk::id::velSens)->load();
    for (int i = 0; i <= 4; ++i)
        vel.addItem (30 + i, juce::String (i * 25) + " %", true, juce::roundToInt (vs * 4.0f) == i);
    menu.addSubMenu ("Velocity sensitivity", vel);
    for (int i = 0; i < 6; ++i)
    {
        const int b = 120 + i * 10;
        bpm.addItem (20 + i, juce::String (b) + " BPM", true, juce::roundToInt (proc.fallbackBpm) == b);
    }
    menu.addSubMenu ("Tempo when the host has none", bpm);

    menu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (&menuButton), [this, current, keyTrack] (int r)
    {
        juce::String err;
        if (r == 1)
            askForName ("Save preset", proc.getPresetName(), [this] (juce::String name)
            {
                juce::String e;
                if (! proc.library.saveUser (name, pk::captureSnapshot (proc.state), e)) { showError (e); return; }
                proc.setPresetName (name.trim());
                refreshHeader();
            });
        else if (r == 2)
            askForName ("Rename preset", proc.getPresetName(), [this, current] (juce::String name)
            {
                juce::String e;
                if (! proc.library.rename (current, name, e)) { showError (e); return; }
                proc.setPresetName (name.trim());
                refreshHeader();
            });
        else if (r == 3)
            juce::AlertWindow::showOkCancelBox (juce::MessageBoxIconType::QuestionIcon, "Delete preset",
                                                "Move \"" + proc.getPresetName() + "\" to the Trash?", "Delete", "Cancel", this,
                                                juce::ModalCallbackFunction::create ([this, current] (int ok)
                                                {
                                                    juce::String e;
                                                    if (ok == 1 && ! proc.library.remove (current, e)) showError (e);
                                                    refreshHeader();
                                                }));
        else if (r == 4)
        {
            chooser = std::make_unique<juce::FileChooser> ("Import PowerKick preset", juce::File(), "*.pkpreset");
            chooser->launchAsync (juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                                  [this] (const juce::FileChooser& fc)
                                  {
                                      if (fc.getResult() == juce::File()) return;
                                      juce::String name, e;
                                      if (! proc.library.importFile (fc.getResult(), name, e)) { showError (e); return; }
                                      proc.loadPreset (proc.library.indexOf (name));
                                      refreshHeader();
                                  });
        }
        else if (r == 5)
        {
            chooser = std::make_unique<juce::FileChooser> ("Export PowerKick preset",
                                                           juce::File::getSpecialLocation (juce::File::userDesktopDirectory)
                                                               .getChildFile (juce::File::createLegalFileName (proc.getPresetName()) + ".pkpreset"),
                                                           "*.pkpreset");
            chooser->launchAsync (juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::warnAboutOverwriting,
                                  [this] (const juce::FileChooser& fc)
                                  {
                                      if (fc.getResult() == juce::File()) return;
                                      juce::String e;
                                      if (! proc.library.exportTo (fc.getResult().withFileExtension ("pkpreset"), proc.getPresetName(),
                                                                   pk::captureSnapshot (proc.state), e))
                                          showError (e);
                                  });
        }
        else if (r == 6)
        {
            auto dir = pk::PresetLibrary::defaultUserDirectory();
            dir.createDirectory();
            dir.revealToUser();
        }
        else if (r == 10)
        {
            proc.monoCheck = ! proc.monoCheck.load();
            repaint();
        }
        else if (r == 11)
            setFloat (proc.state, pk::id::keyTrack, keyTrack ? 0.0f : 1.0f);
        else if (r >= 20 && r < 26)
            proc.fallbackBpm = 120.0 + (r - 20) * 10.0;
        else if (r >= 30 && r <= 34)
            setFloat (proc.state, pk::id::velSens, (r - 30) * 0.25f);
    });
}

void PowerKickEditor::showExportMenu()
{
    juce::PopupMenu m;
    m.addItem (1, "Stereo WAV, 24-bit...");
    m.addItem (2, "Mono WAV, 24-bit...");
    m.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (&exportButton),
                     [this] (int r) { if (r > 0) exportWav (r == 1); });
}

void PowerKickEditor::exportWav (bool stereo)
{
    const auto suggested = juce::File::getSpecialLocation (juce::File::userDesktopDirectory)
                               .getChildFile (juce::File::createLegalFileName ("PowerKick - " + proc.getPresetName()) + ".wav");
    chooser = std::make_unique<juce::FileChooser> ("Export kick", suggested, "*.wav");
    chooser->launchAsync (juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::warnAboutOverwriting,
                          [this, stereo] (const juce::FileChooser& fc)
                          {
                              auto file = fc.getResult();
                              if (file == juce::File()) return;
                              // Export ignores Mono Check: the channel format is chosen here.
                              const double sr = proc.getCurrentSampleRate();
                              const auto audio = pk::renderKick (pk::readParams (proc.state), sr, proc.getHostBpm(), stereo);
                              juce::String e;
                              if (! pk::writeWav24 (file.withFileExtension ("wav"), audio, sr, e))
                                  showError (e);
                          });
}

void PowerKickEditor::askForName (const juce::String& title, const juce::String& initial, std::function<void (juce::String)> done)
{
    auto* w = new juce::AlertWindow (title, "Preset name", juce::MessageBoxIconType::NoIcon, this);
    w->setLookAndFeel (&lnf);
    w->addTextEditor ("name", initial);
    w->addButton ("OK", 1, juce::KeyPress (juce::KeyPress::returnKey));
    w->addButton ("Cancel", 0, juce::KeyPress (juce::KeyPress::escapeKey));
    w->enterModalState (true, juce::ModalCallbackFunction::create ([w, done] (int r)
    {
        const auto name = w->getTextEditorContents ("name").trim();
        if (r == 1 && name.isNotEmpty())
            done (name);
    }), true);
}

void PowerKickEditor::showError (const juce::String& message)
{
    juce::AlertWindow::showMessageBoxAsync (juce::MessageBoxIconType::WarningIcon, "PowerKick", message, "OK", this);
}
