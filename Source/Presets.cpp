#include "Presets.h"

namespace pk
{
namespace
{
// Order: tune, decay, punch, body, click, drive, character, rumble, output, width, delay,
// driveMode (0 sat, 1 clip, 2 industrial), charMode (0 deep, 1 punchy, 2 dark, 3 hard, 4 raw),
// pitchDepth, pitchTime, clickType (0 soft, 1 sharp, 2 metal), clickTone, rumbleTime, rumbleSync
struct Row { const char* name; const char* category; float v[19]; };

const Row rows[] = {
    { "Warehouse Punch",  "Techno",      { -1, .38f, .70f, .55f, .35f, .25f, .50f, 0, -2.6f, 0, 0, 0, 1, 30, 28, 1, .50f, .50f, 0 } },
    { "Deep Rumble 132",  "Techno",      { -2, .55f, .50f, .60f, .20f, .20f, .60f, .55f, -3.7f, 0, 0, 0, 0, 26, 35, 0, .40f, .55f, 0 } },
    { "Berlin Tight",     "Techno",      { 0, .25f, .75f, .45f, .45f, .20f, .45f, 0, -0.8f, 0, 0, 0, 1, 34, 22, 1, .60f, .50f, 0 } },
    { "Long Sub Techno",  "Techno",      { -3, .75f, .45f, .70f, .25f, .15f, .55f, .25f, -4.0f, 0, 0, 0, 0, 24, 40, 0, .30f, .60f, 1 } },
    { "Hypnotic Knock",   "Techno",      { 0, .35f, .65f, .50f, .30f, .35f, .50f, .35f, -2.4f, 15, 1.5f, 0, 1, 28, 25, 1, .45f, .40f, 0 } },
    { "Dark Room",        "Techno",      { -2, .50f, .55f, .55f, .25f, .30f, .60f, .40f, -4.0f, 0, 0, 0, 2, 30, 32, 0, .35f, .50f, 0 } },

    { "Hard Floor",       "Hard Techno", { 1, .42f, .80f, .60f, .50f, .55f, .55f, .30f, -1.6f, 0, 0, 1, 3, 36, 26, 1, .55f, .45f, 0 } },
    { "Pounder 150",      "Hard Techno", { 2, .35f, .85f, .55f, .55f, .60f, .50f, .20f, -0.9f, 20, 2, 0, 3, 38, 22, 1, .60f, .40f, 0 } },
    { "Distorted Tail",   "Hard Techno", { 0, .60f, .70f, .65f, .40f, .65f, .60f, .50f, -2.7f, 0, 0, 0, 2, 32, 30, 1, .50f, .55f, 0 } },
    { "Peak Time",        "Hard Techno", { 1, .40f, .90f, .60f, .60f, .50f, .60f, .25f, -2.4f, 30, 3, 1, 1, 40, 20, 1, .65f, .40f, 0 } },
    { "Stomp Wide",       "Hard Techno", { 2, .45f, .75f, .60f, .55f, .60f, .55f, .30f, -1.5f, 55, 6, 1, 3, 36, 24, 2, .50f, .45f, 0 } },
    { "Acid Bunker",      "Hard Techno", { -1, .50f, .70f, .65f, .35f, .70f, .65f, .45f, -3.7f, 10, 1, 2, 2, 30, 30, 0, .40f, .55f, 1 } },

    { "Schranz Machine",  "Schranz",     { 2, .30f, .85f, .70f, .55f, .80f, .70f, .35f, -0.4f, 0, 0, 1, 3, 40, 18, 1, .70f, .35f, 0 } },
    { "Loop Grinder",     "Schranz",     { 2, .32f, .80f, .75f, .50f, .85f, .75f, .40f, 1.1f, 25, 2.5f, 2, 4, 38, 20, 2, .60f, .35f, 0 } },
    { "Sawblade",         "Schranz",     { 3, .28f, .90f, .65f, .70f, .80f, .70f, .25f, 1.4f, 40, 4, 1, 4, 44, 16, 2, .80f, .30f, 0 } },
    { "Tribal Pressure",  "Schranz",     { 1, .36f, .85f, .70f, .45f, .75f, .60f, .50f, -0.5f, 15, 1.5f, 0, 3, 36, 22, 1, .55f, .40f, 0 } },
    { "Overdriven Loop",  "Schranz",     { 1, .40f, .80f, .80f, .50f, .90f, .80f, .45f, 1.9f, 0, 0, 2, 3, 34, 24, 1, .60f, .40f, 0 } },
    { "Concrete Hammer",  "Schranz",     { 3, .26f, .95f, .70f, .65f, .85f, .75f, .20f, 1.9f, 60, 5, 1, 4, 46, 15, 2, .75f, .30f, 0 } },

    { "Rust Factory",     "Industrial",  { -1, .55f, .70f, .70f, .45f, .80f, .80f, .55f, 2.7f, 70, 8, 2, 4, 32, 30, 2, .30f, .60f, 1 } },
    { "Steel Press",      "Industrial",  { 1, .40f, .80f, .65f, .60f, .75f, .70f, .35f, 0.7f, 80, 10, 2, 3, 38, 22, 2, .90f, .45f, 0 } },
    { "Noise Bunker",     "Industrial",  { -1, .50f, .70f, .70f, .55f, .85f, .85f, .50f, -0.4f, 60, 12, 2, 2, 30, 28, 1, .40f, .55f, 2 } },
    { "Wide Iron Sub",    "Industrial",  { -2, .65f, .65f, .70f, .50f, .70f, .75f, .40f, 2.0f, 100, 14, 2, 4, 28, 34, 2, .55f, .55f, 1 } },

    { "Gabba Lite",       "Rave / Hard Dance", { 4, .45f, .90f, .75f, .55f, .90f, .70f, 0, 0.6f, 0, 0, 1, 3, 46, 30, 1, .60f, .50f, 0 } },
    { "Hardstyle Tok",    "Rave / Hard Dance", { 3, .50f, .85f, .70f, .50f, .75f, .70f, .10f, -0.8f, 25, 2, 0, 1, 48, 45, 1, .70f, .50f, 0 } },
    { "Euphoric Punch",   "Rave / Hard Dance", { 4, .40f, .90f, .60f, .55f, .55f, .60f, 0, -1.8f, 35, 3, 0, 1, 42, 30, 0, .60f, .50f, 0 } },
    { "Rawstyle Screech", "Rave / Hard Dance", { 3, .48f, .85f, .75f, .60f, .95f, .85f, .15f, 3.9f, 45, 4, 2, 4, 46, 40, 2, .80f, .50f, 0 } },
};

const char* const rowIds[19] = { id::tune, id::decay, id::punch, id::body, id::click, id::drive, id::character,
                                 id::rumble, id::output, id::width, id::delay, id::driveMode, id::charMode,
                                 id::pitchDepth, id::pitchTime, id::clickType, id::clickTone, id::rumbleTime,
                                 id::rumbleSync };

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
            for (int i = 0; i < 19; ++i)
                p.values[rowIds[i]] = r.v[i];
            p.values[id::velSens] = 0.6f;
            p.values[id::keyTrack] = 0.0f;
            v.push_back (std::move (p));
        }
        return v;
    }();
    return list;
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
    if (! file.moveToTrash() && ! file.deleteFile())
    {
        error = "Delete failed";
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
