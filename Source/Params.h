#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace pk
{
// Parameter IDs. Visible controls first, then hidden "voicing" parameters that
// presets and automation use but the main interface does not show.
namespace id
{
    inline constexpr const char* tune      = "tune";
    inline constexpr const char* decay     = "decay";
    inline constexpr const char* punch     = "punch";
    inline constexpr const char* body      = "body";
    inline constexpr const char* click     = "click";
    inline constexpr const char* drive     = "drive";
    inline constexpr const char* character = "character";
    inline constexpr const char* rumble    = "rumble";
    inline constexpr const char* output    = "output";
    inline constexpr const char* width     = "width";
    inline constexpr const char* delay     = "delay";
    inline constexpr const char* driveMode = "driveMode";
    inline constexpr const char* charMode  = "charMode";

    inline constexpr const char* pitchDepth = "pitchDepth";
    inline constexpr const char* pitchTime  = "pitchTime";
    inline constexpr const char* clickType  = "clickType";
    inline constexpr const char* clickTone  = "clickTone";
    inline constexpr const char* rumbleTime = "rumbleTime";
    inline constexpr const char* rumbleSync = "rumbleSync";
    inline constexpr const char* velSens    = "velSens";
    inline constexpr const char* keyTrack   = "keyTrack";
}

enum class DriveMode { saturate, clip, industrial };
enum class CharMode  { deep, punchy, dark, hard, raw };
enum class ClickType { soft, sharp, metal };

// Plain snapshot of every sound parameter in real units.
struct KickParams
{
    float tune = 0.0f;          // semitones, -12..+12 around A1 (55 Hz)
    float decay = 0.45f;        // 0..1 (mapped to 60..1500 ms)
    float punch = 0.6f;         // 0..1
    float body = 0.5f;          // 0..1
    float click = 0.35f;        // 0..1
    float drive = 0.3f;         // 0..1
    float character = 0.5f;     // 0..1
    float rumble = 0.0f;        // 0..1
    float outputDb = -3.0f;     // -24..+6 dB
    float width = 0.0f;         // 0..100 %
    float delayMs = 0.0f;       // 0..15 ms
    DriveMode driveMode = DriveMode::saturate;
    CharMode charMode = CharMode::punchy;

    float pitchDepth = 30.0f;   // semitones of pitch sweep
    float pitchTime = 30.0f;    // ms, sweep time constant
    ClickType clickType = ClickType::sharp;
    float clickTone = 0.5f;     // 0..1
    float rumbleTime = 0.5f;    // 0..1
    int rumbleSync = 0;         // 0 = 1/16, 1 = 1/8, 2 = 3/16
    float velSens = 0.6f;       // 0..1
    bool keyTrack = false;
};

juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
KickParams readParams (const juce::AudioProcessorValueTreeState& state);

// Every parameter ID, in a stable order (used by presets, A/B and randomize).
const juce::StringArray& allParamIds();

float decayMs (float decayNorm) noexcept;
juce::String noteNameForTune (float semitones);
float baseFrequency (float tuneSemitones) noexcept;
}
