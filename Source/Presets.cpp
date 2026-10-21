#include "Presets.h"

namespace pk
{
namespace
{
// Order: tune, decay, punch, body, click, drive, character, rumble, output, width, delay,
// driveMode (0 sat, 1 clip, 2 industrial), charMode (0 deep, 1 punchy, 2 dark, 3 hard, 4 raw),
// pitchDepth, pitchTime, clickType (0 soft, 1 sharp, 2 metal), clickTone, rumbleTime, rumbleSync, bodyTone, pitchFall, splitFreq
struct Row { const char* name; const char* category; float v[22]; const char* group = ""; };

const Row rows[] = {
    // Techno, Hard Techno, Schranz and two Industrial presets are fitted to typical reference kicks
    // (Tools/fit_kicks.py: k-medoids per category, synthesis parameters matched to the medoid).
    { "Classic Punch",       "Techno",        { -3.6f, 0.423f, 0.777f, 0.446f, 0.000f, 0.475f, 0.800f, 0.133f, -3.0f, 0.0f, 0.0f, 1, 1, 39.3f, 32.5f, 0, 0.302f, 0.052f, 0, 0.900f, 0.8f, 500 } }, // fit 8.5 to OBI Punchy Kick 18.wav (cluster of 6)
    { "Ticking Body",        "Techno",        { -0.4f, 0.374f, 0.709f, 0.860f, 0.825f, 0.451f, 0.400f, 0.000f, -3.0f, 0.0f, 0.0f, 0, 1, 33.7f, 52.8f, 2, 0.784f, 0.119f, 0, 0.700f, 0.1f, 500 } }, // fit 9.4 to OBI Punchy Kick 1.wav (cluster of 1)
    { "Falling Thump",       "Techno",        { -3.8f, 0.378f, 0.903f, 0.644f, 1.000f, 0.600f, 0.400f, 0.100f, -1.0f, 0.0f, 0.0f, 0, 3, 29.7f, 33.5f, 0, 0.773f, 0.000f, 0, 0.000f, 9.0f, 500 } }, // fit 11.9 to OBI Punchy Kick 4.wav (cluster of 9)
    { "Raw Thud",            "Techno",        { 0.9f, 0.409f, 0.426f, 0.165f, 1.000f, 0.805f, 0.337f, 0.000f, -3.1f, 0.0f, 0.0f, 0, 4, 41.1f, 41.1f, 0, 0.380f, 0.704f, 0, 0.000f, 0.0f, 500 } }, // fit 12.6 to SVEKick12S.wav (cluster of 7)
    { "Pitch Drop",          "Techno",        { -4.9f, 0.436f, 0.911f, 0.300f, 0.600f, 0.649f, 0.900f, 0.011f, -3.9f, 0.0f, 0.0f, 1, 1, 20.1f, 50.4f, 2, 0.045f, 0.200f, 0, 0.900f, 6.9f, 500 } }, // fit 12.9 to SVEKick07S.wav (cluster of 3)
    { "Short Knock",         "Techno",        { -1.8f, 0.292f, 0.926f, 0.800f, 1.000f, 0.800f, 0.600f, 0.013f, -3.4f, 0.0f, 0.0f, 0, 1, 18.6f, 48.6f, 2, 0.036f, 0.000f, 0, 0.000f, 7.2f, 500 } }, // fit 11.4 to SVEKick04S.wav (cluster of 9)
    { "Long Clip",           "Hard Techno",   { -1.6f, 0.509f, 0.813f, 0.075f, 0.700f, 0.700f, 0.400f, 0.000f, -3.7f, 0.0f, 0.0f, 1, 1, 24.6f, 41.3f, 1, 1.000f, 0.993f, 0, 0.400f, 3.2f, 500 } }, // fit 10.8 to SVEKick04C.wav (cluster of 55)
    { "Hard Techno Core",    "Hard Techno",   { -0.7f, 0.421f, 0.471f, 0.742f, 0.141f, 0.599f, 0.300f, 0.001f, -2.5f, 40.0f, 3.0f, 1, 1, 39.8f, 54.7f, 2, 1.000f, 0.500f, 0, 0.500f, 1.4f, 500 } }, // fit 9.8 to HST1 Rumble Kick Shot 25.wav (cluster of 60)
    { "Deep Drop",           "Hard Techno",   { -2.0f, 0.461f, 0.888f, 0.727f, 0.900f, 0.253f, 0.300f, 0.000f, -1.9f, 0.0f, 0.0f, 1, 0, 34.9f, 26.8f, 1, 1.000f, 0.000f, 0, 0.700f, 8.2f, 500 } }, // fit 9.9 to OBI Hard Rumble Kick 21.wav (cluster of 24)
    { "Rumble Tail",         "Hard Techno",   { -0.4f, 0.306f, 0.998f, 0.318f, 1.000f, 0.506f, 0.934f, 0.656f, -0.9f, 0.0f, 0.0f, 1, 0, 38.5f, 17.9f, 0, 1.000f, 0.000f, 0, 1.000f, 0.0f, 500 } }, // fit 15.4 to HST1 Rumble Kick Shot 31.wav (cluster of 17)
    { "Sub Pressure",        "Hard Techno",   { 1.3f, 0.469f, 0.213f, 0.325f, 0.500f, 0.168f, 1.000f, 0.010f, -3.3f, 0.0f, 0.0f, 0, 0, 7.0f, 4.0f, 0, 0.400f, 0.700f, 0, 0.900f, 0.0f, 500 } }, // fit 17.1 to SVE2 Sub 04.wav (cluster of 9)
    { "Falling Clipper",     "Hard Techno",   { -2.7f, 0.425f, 0.012f, 0.783f, 1.000f, 0.261f, 0.089f, 0.000f, -3.2f, 0.0f, 0.0f, 1, 1, 47.3f, 29.9f, 1, 1.000f, 0.896f, 0, 0.490f, 14.0f, 500 } }, // fit 10.5 to OBI Hard Rumble Kick 24.wav (cluster of 53)
    { "Schranz Metal",       "Schranz",       { 2.9f, 0.414f, 0.957f, 0.631f, 0.755f, 0.800f, 0.800f, 0.000f, -2.4f, 30.0f, 3.0f, 0, 1, 39.0f, 28.9f, 2, 0.738f, 0.131f, 0, 0.400f, 0.9f, 500 } }, // fit 15.8 to MS Signature Kick Shot 30.wav (cluster of 24)
    { "Schranz Pressure",    "Schranz",       { -0.8f, 0.419f, 0.916f, 0.834f, 0.500f, 0.778f, 0.300f, 0.001f, -1.1f, 41.1f, 3.0f, 0, 1, 28.7f, 60.1f, 0, 0.800f, 1.000f, 0, 0.200f, 2.4f, 500 } }, // fit 12.8 to SSP Signature Schranz Kick 92.wav (cluster of 22)
    { "Wide Hammer",         "Schranz",       { 2.3f, 0.414f, 0.736f, 1.000f, 0.977f, 0.900f, 0.000f, 0.000f, -4.0f, 100.0f, 0.0f, 1, 2, 34.6f, 44.4f, 2, 0.300f, 0.623f, 0, 0.400f, 3.4f, 500 } }, // fit 10.0 to ITHTS Signature Schranz Kick 4.wav (cluster of 12)
    { "Schranz Wide Tick",   "Schranz",       { 2.0f, 0.407f, 0.928f, 0.945f, 0.860f, 0.400f, 0.600f, 0.000f, -2.6f, 60.0f, 3.0f, 1, 1, 37.6f, 26.9f, 2, 0.800f, 0.410f, 0, 0.500f, 5.2f, 500 } }, // fit 11.3 to SSP Signature Schranz Kick 90.wav (cluster of 50)
    { "Schranz Air",         "Schranz",       { 2.3f, 0.427f, 0.999f, 0.700f, 0.619f, 0.400f, 0.600f, 0.000f, -2.9f, 100.0f, 3.0f, 0, 1, 47.2f, 25.2f, 2, 0.532f, 0.372f, 0, 0.700f, 3.3f, 500 } }, // fit 13.5 to SSP Signature Schranz Kick 38.wav (cluster of 52)
    { "Classic Schranz",     "Schranz",       { 0.4f, 0.358f, 0.528f, 0.440f, 0.600f, 0.610f, 0.700f, 0.377f, -3.8f, 0.0f, 0.0f, 1, 1, 41.6f, 31.0f, 2, 0.629f, 0.000f, 0, 0.900f, 0.2f, 500 } }, // fit 13.0 to ARK Kick Drum Shot 19.wav (cluster of 42)
    { "Distorted Factory",   "Industrial",    { 1.0f, 0.322f, 0.930f, 0.928f, 1.000f, 0.900f, 0.900f, 0.200f, -2.5f, 35.1f, 3.0f, 1, 1, 28.4f, 27.4f, 1, 0.000f, 0.000f, 0, 0.650f, 0.3f, 500 } }, // fit 16.5 to ITHTS Distorted Kick Shot 6.wav (cluster of 27)
    { "Industrial Rumble",   "Industrial",    { 0.7f, 0.424f, 0.799f, 0.401f, 0.700f, 0.544f, 0.446f, 0.700f, -1.9f, 0.0f, 0.0f, 1, 3, 29.9f, 35.6f, 1, 0.500f, 0.000f, 0, 0.731f, 0.5f, 500 } }, // fit 13.0 to OBI Hard Industrial Kick 16.wav (cluster of 57)
    // Hand-made (no close reference cluster):
    { "Steel Press",      "Industrial",  { 1, .40f, .80f, .65f, .60f, .75f, .70f, .35f, -0.3f, 80, 10, 2, 3, 38, 22, 2, .90f, .45f, 0, 1, 0, 500 } },
    { "Wide Iron Sub",    "Industrial",  { -2, .65f, .65f, .70f, .50f, .70f, .75f, .40f, 0.7f, 100, 14, 2, 4, 28, 34, 2, .55f, .55f, 1, 1, 0, 500 } },
    // Hand-made: the reference folder has no rave / hard-dance kicks.
    { "Gabba Lite",       "Rave / Hard Dance", { 4, .45f, .90f, .75f, .55f, .90f, .70f, 0, -2.4f, 0, 0, 1, 3, 46, 30, 1, .60f, .50f, 0, 1, 0, 500 } },
    { "Hardstyle Tok",    "Rave / Hard Dance", { 3, .50f, .85f, .70f, .50f, .75f, .70f, .10f, -1.2f, 25, 2, 0, 1, 48, 45, 1, .70f, .50f, 0, 1, 0, 500 } },
    { "Euphoric Punch",   "Rave / Hard Dance", { 4, .40f, .90f, .60f, .55f, .55f, .60f, 0, -1.7f, 35, 3, 0, 1, 42, 30, 0, .60f, .50f, 0, 1, 0, 500 } },
    { "Rawstyle Screech", "Rave / Hard Dance", { 3, .48f, .85f, .75f, .60f, .95f, .85f, .15f, 1.1f, 45, 4, 2, 4, 46, 40, 2, .80f, .50f, 0, 1, 0, 500 } },
};

const char* const rowIds[22] = { id::tune, id::decay, id::punch, id::body, id::click, id::drive, id::character,
                                 id::rumble, id::output, id::width, id::delay, id::driveMode, id::charMode,
                                 id::pitchDepth, id::pitchTime, id::clickType, id::clickTone, id::rumbleTime,
                                 id::rumbleSync, id::bodyTone, id::pitchFall, id::splitFreq };

juce::String sanitiseName (juce::String name)
{
    name = name.trim().removeCharacters ("\\/:*?\"<>|");
    return name.isEmpty() ? juce::String ("Untitled") : name.substring (0, 64);
}
}

const std::vector<Preset>& factoryPresets()
{
    static const std::vector<Preset> list = []
    {
        std::vector<Preset> v;
        for (const auto& r : rows)
        {
            Preset p { r.name, r.category, {}, {} };
            p.group = r.group;
            for (int i = 0; i < 22; ++i)
                p.values[rowIds[i]] = r.v[i];
            p.values[id::velSens] = 0.6f;
            p.values[id::keyTrack] = 0.0f;
            v.push_back (std::move (p));
        }
        return v;
    }();
    return list;
}

const Snapshot& legacyNeutral()
{
    static const Snapshot neutral { { id::bodyTone, 1.0f }, { id::pitchFall, 0.0f }, { id::splitFreq, 1000.0f } };
    return neutral;
}

Snapshot captureSnapshot (const juce::AudioProcessorValueTreeState& state)
{
    Snapshot s;
    for (const auto& pid : allParamIds())
        s[pid] = state.getRawParameterValue (pid)->load();
    return s;
}

void applySnapshot (juce::AudioProcessorValueTreeState& state, const Snapshot& values)
{
    for (const auto& pid : allParamIds())
    {
        auto* param = dynamic_cast<juce::RangedAudioParameter*> (state.getParameter (pid));
        if (param == nullptr)
            continue;
        const auto found = values.find (pid);
        const float norm = found != values.end() ? param->convertTo0to1 (found->second) : param->getDefaultValue();
        param->beginChangeGesture();
        param->setValueNotifyingHost (norm);
        param->endChangeGesture();
    }
}

KickParams paramsFromSnapshot (const Snapshot& s)
{
    KickParams p;
    auto get = [&s] (const char* pid, float def) { auto it = s.find (pid); return it != s.end() ? it->second : def; };
    p.tune = get (id::tune, p.tune);
    p.decay = get (id::decay, p.decay);
    p.punch = get (id::punch, p.punch);
    p.body = get (id::body, p.body);
    p.click = get (id::click, p.click);
    p.drive = get (id::drive, p.drive);
    p.character = get (id::character, p.character);
    p.rumble = get (id::rumble, p.rumble);
    p.outputDb = get (id::output, p.outputDb);
    p.width = get (id::width, p.width);
    p.delayMs = get (id::delay, p.delayMs);
    p.driveMode = (DriveMode) juce::roundToInt (get (id::driveMode, (float) p.driveMode));
    p.charMode = (CharMode) juce::roundToInt (get (id::charMode, (float) p.charMode));
    p.pitchDepth = get (id::pitchDepth, p.pitchDepth);
    p.pitchTime = get (id::pitchTime, p.pitchTime);
    p.clickType = (ClickType) juce::roundToInt (get (id::clickType, (float) p.clickType));
    p.clickTone = get (id::clickTone, p.clickTone);
    p.rumbleTime = get (id::rumbleTime, p.rumbleTime);
    p.rumbleSync = juce::roundToInt (get (id::rumbleSync, (float) p.rumbleSync));
    p.velSens = get (id::velSens, p.velSens);
    p.keyTrack = get (id::keyTrack, 0.0f) > 0.5f;
    p.bodyTone = get (id::bodyTone, p.bodyTone);
    p.pitchFall = get (id::pitchFall, p.pitchFall);
    p.splitHz = get (id::splitFreq, p.splitHz);
    return p;
}

juce::String presetToXml (const juce::String& name, const juce::String& category, const Snapshot& values)
{
    juce::XmlElement root ("PowerKickPreset");
    root.setAttribute ("format", 1);
    root.setAttribute ("name", name);
    root.setAttribute ("category", category);
    for (const auto& [pid, value] : values)
    {
        auto* e = root.createNewChildElement ("Param");
        e->setAttribute ("id", pid);
        e->setAttribute ("value", (double) value);
    }
    return root.toString();
}

bool presetFromXml (const juce::String& xml, Preset& out, juce::String& error)
{
    auto root = juce::parseXML (xml);
    if (root == nullptr || ! root->hasTagName ("PowerKickPreset"))
    {
        error = "Not a PowerKick preset";
        return false;
    }
    if (root->getIntAttribute ("format", 0) > 1)
    {
        error = "Preset was made by a newer PowerKick version";
        return false;
    }
    out.name = sanitiseName (root->getStringAttribute ("name"));
    out.category = root->getStringAttribute ("category", "User");
    out.values.clear();
    const auto& known = allParamIds();
    for (auto* e : root->getChildWithTagNameIterator ("Param"))
    {
        const auto pid = e->getStringAttribute ("id");
        const auto value = (float) e->getDoubleAttribute ("value", std::numeric_limits<double>::quiet_NaN());
        if (known.contains (pid) && std::isfinite (value))
            out.values[pid] = value;
    }
    if (! out.values.empty())
        for (const auto& [pid, value] : legacyNeutral())
            if (out.values.find (pid) == out.values.end())
                out.values[pid] = value;
    if (out.values.empty())
    {
        error = "Preset contains no PowerKick parameters";
        return false;
    }
    return true;
}

//==============================================================================
PresetLibrary::PresetLibrary (juce::File dir) : userDir (std::move (dir)) { rescan(); }

juce::File PresetLibrary::defaultUserDirectory()
{
    return juce::File::getSpecialLocation (juce::File::userDocumentsDirectory).getChildFile ("PowerKick/Presets");
}

juce::File PresetLibrary::fileFor (const juce::String& name) const
{
    return userDir.getChildFile (sanitiseName (name) + extension);
}

void PresetLibrary::rescan()
{
    presets = factoryPresets();
    juce::Array<juce::File> files = userDir.findChildFiles (juce::File::findFiles, false, juce::String ("*") + extension);
    files.sort();
    for (const auto& f : files)
    {
        Preset p;
        juce::String err;
        if (presetFromXml (f.loadFileAsString(), p, err))
        {
            p.name = f.getFileNameWithoutExtension();
            p.category = "User";
            p.file = f;
            presets.push_back (std::move (p));
        }
    }
}

int PresetLibrary::indexOf (const juce::String& name) const
{
    for (size_t i = 0; i < presets.size(); ++i)
        if (presets[i].name == name)
            return (int) i;
    return -1;
}

bool PresetLibrary::saveUser (const juce::String& name, const Snapshot& values, juce::String& error)
{
    if (! userDir.createDirectory())
    {
        error = "Cannot create " + userDir.getFullPathName();
        return false;
    }
    const auto file = fileFor (name);
    if (! file.replaceWithText (presetToXml (sanitiseName (name), "User", values)))
    {
        error = "Cannot write " + file.getFullPathName();
        return false;
    }
    rescan();
    return true;
}

bool PresetLibrary::rename (int index, const juce::String& newName, juce::String& error)
{
    if (index < 0 || index >= (int) presets.size() || presets[(size_t) index].isFactory())
    {
        error = "Factory presets cannot be renamed";
        return false;
    }
    const auto target = fileFor (newName);
    if (target.existsAsFile())
    {
        error = "A preset with this name already exists";
        return false;
    }
    const auto values = presets[(size_t) index].values;
    const auto old = presets[(size_t) index].file;
    if (! target.replaceWithText (presetToXml (sanitiseName (newName), "User", values)) || ! old.deleteFile())
    {
        error = "Rename failed";
        return false;
    }
    rescan();
    return true;
}

bool PresetLibrary::remove (int index, juce::String& error)
{
    if (index < 0 || index >= (int) presets.size() || presets[(size_t) index].isFactory())
    {
        error = "Factory presets cannot be deleted";
        return false;
    }
    // Moved to the trash rather than destroyed, so a mistaken delete can be undone.
    const auto file = presets[(size_t) index].file;
    // Only ever to the Trash: if that is impossible the file stays (never a silent hard delete).
    if (! file.moveToTrash())
    {
        error = "Could not move the preset to the Trash; nothing was deleted";
        return false;
    }
    rescan();
    return true;
}

bool PresetLibrary::importFile (const juce::File& source, juce::String& importedName, juce::String& error)
{
    Preset p;
    if (! presetFromXml (source.loadFileAsString(), p, error))
        return false;
    auto name = p.name.isNotEmpty() ? p.name : source.getFileNameWithoutExtension();
    for (int n = 2; fileFor (name).existsAsFile() || indexOf (name) >= 0; ++n)
        name = p.name + " " + juce::String (n);
    importedName = name;
    return saveUser (name, p.values, error);
}

bool PresetLibrary::exportTo (const juce::File& target, const juce::String& name, const Snapshot& values,
                              juce::String& error) const
{
    if (! target.replaceWithText (presetToXml (sanitiseName (name), "User", values)))
    {
        error = "Cannot write " + target.getFullPathName();
        return false;
    }
    return true;
}
}
