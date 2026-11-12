#include "Presets.h"
#include <algorithm>

namespace pk
{
namespace
{
// Order: tune, decay, punch, body, click, drive, character, rumble, output, width, delay,
// driveMode (0 sat, 1 clip, 2 industrial), charMode (0 deep, 1 punchy, 2 dark, 3 hard, 4 raw),
// pitchDepth, pitchTime, clickType (0 soft, 1 sharp, 2 metal), clickTone, rumbleTime, rumbleSync, bodyTone, pitchFall, splitFreq,
// weight (1.2; rows with 22 values have none)
struct Row { const char* name; const char* category; float v[23]; const char* group = ""; };

const Row rows[] = {
    // 300 presets. Fitted ones are matched to measured reference kicks (Tools/fit_kicks.py,
    // Tools/build_library.py); hand-made ones have no close reference. Last field: browser group.
    // --- Techno (30) ---
    { "Short Knock",               "Techno",            { -1.5f, 0.305f, 0.996f, 0.697f, 0.900f, 0.600f, 0.800f, 0.013f, -6.1f, 0.0f, 0.0f, 1, 1, 17.0f, 48.2f, 2, 0.227f, 0.034f, 0, 0.000f, 9.8f, 500.0f }, "Short" }, // fit 10.1 to SVEKick04S.wav (kick register)
    { "Driven Clang F#1 II",       "Techno",            { -2.6f, 0.358f, 0.821f, 0.464f, 0.614f, 0.610f, 0.717f, 0.033f, -3.1f, 0.0f, 0.0f, 0, 1, 30.3f, 53.0f, 2, 0.958f, 0.000f, 0, 0.009f, 1.0f, 500.0f, 0.225f }, "Driven" }, // fit 18.5 to OBI Punchy Kick 19.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Click F#1 III)
    { "Driven Click G#1 III",      "Techno",            { -1.3f, 0.305f, 0.800f, 0.576f, 1.000f, 0.645f, 1.000f, 0.030f, -2.2f, 0.0f, 0.0f, 0, 1, 27.9f, 49.0f, 0, 0.900f, 0.000f, 0, 0.100f, 4.8f, 500.0f }, "Short" }, // fit 13.8 to SVEKick11S.wav (kick register)
    { "Clipped Clang A1 II",       "Techno",            { -0.0f, 0.313f, 0.844f, 0.616f, 0.800f, 0.600f, 0.800f, 0.011f, -4.4f, 0.0f, 0.0f, 1, 1, 17.0f, 47.8f, 2, 0.278f, 0.000f, 0, 0.300f, 7.2f, 500.0f }, "Short" }, // fit 12.6 to SVEKick03S.wav (kick register)
    { "Driven Knock D#1",          "Techno",            { -5.8f, 0.287f, 0.274f, 0.567f, 0.600f, 0.648f, 1.000f, 0.048f, -2.5f, 0.0f, 0.0f, 0, 1, 25.8f, 77.9f, 1, 0.000f, 0.434f, 0, 0.024f, 15.7f, 500.0f, 0.096f }, "Short" }, // fit 13.2 to OBI Punchy Kick 13.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Click D#1)
    { "Clipped Clang A1 III",      "Techno",            { -0.1f, 0.299f, 0.626f, 0.600f, 0.800f, 0.700f, 1.000f, 0.017f, -3.9f, 0.0f, 0.0f, 1, 1, 43.8f, 29.5f, 2, 0.731f, 0.100f, 0, 0.600f, 8.8f, 500.0f }, "Short" }, // fit 13.7 to SVEKick14S.wav (kick register)
    { "Driven Click F1",           "Techno",            { -3.7f, 0.269f, 0.581f, 1.000f, 0.800f, 0.544f, 0.437f, 0.000f, -2.1f, 0.0f, 0.0f, 0, 4, 45.8f, 74.0f, 1, 0.600f, 0.366f, 0, 0.000f, 0.0f, 1000.0f }, "Short" }, // fit 13.8 to OBI Punchy Kick 3.wav (kick register)
    { "Warm Clang C#1",            "Techno",            { -8.4f, 0.272f, 0.992f, 0.500f, 1.000f, 0.400f, 0.900f, 0.010f, -4.8f, 0.0f, 0.0f, 0, 3, 23.5f, 74.5f, 2, 0.041f, 0.000f, 0, 0.000f, 0.9f, 500.0f }, "Short" }, // fit 15.2 to OBI Punchy Kick 20.wav (kick register)
    { "Warm Click D#1 II",         "Techno",            { -5.8f, 0.481f, 0.493f, 0.089f, 0.994f, 0.427f, 0.857f, 0.000f, -3.7f, 0.0f, 0.0f, 1, 4, 47.7f, 57.0f, 1, 0.086f, 0.454f, 0, 0.175f, 6.2f, 500.0f, 0.300f }, "Short" }, // fit 15.9 to SVEKick12S.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Clipped Click G#1 V",       "Techno",            { -1.2f, 0.479f, 0.640f, 0.167f, 1.000f, 0.615f, 0.034f, 0.000f, -1.4f, 1.0f, 0.0f, 1, 0, 37.2f, 47.4f, 1, 0.854f, 0.968f, 0, 0.200f, 0.0f, 500.0f, 0.239f }, "Long" }, // fit 15.6 to OBI Punchy Kick 12.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Crushed Clang G1",          "Techno",            { -2.3f, 0.355f, 0.946f, 0.427f, 0.655f, 0.911f, 0.900f, 0.069f, -1.0f, 1.1f, 0.0f, 1, 1, 18.0f, 61.8f, 2, 0.100f, 0.000f, 0, 0.700f, 7.0f, 500.0f, 0.123f }, "Long" }, // fit 14.0 to SVEKick06S.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Thump G1 IV)
    { "Crushed Clang F#1 III",     "Techno",            { -3.4f, 0.455f, 0.721f, 0.229f, 1.000f, 0.766f, 0.834f, 0.084f, -5.5f, 0.0f, 0.0f, 1, 1, 11.5f, 67.4f, 2, 0.000f, 0.000f, 0, 0.639f, 13.5f, 500.0f, 0.152f }, "Long" }, // fit 14.0 to SVEKick10S.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Crushed Punch F#1)
    { "Clean Clang G1 IV",         "Techno",            { -1.8f, 0.798f, 0.948f, 0.000f, 0.700f, 0.250f, 0.800f, 0.000f, -0.3f, 20.0f, 0.0f, 1, 0, 40.5f, 37.4f, 2, 0.466f, 0.740f, 0, 0.500f, 0.9f, 400.0f }, "Long" }, // fit 7.7 to OBI Punchy Kick 10.wav (kick register)
    { "Wide Grinding Rumble A1",   "Techno",            { -0.1f, 0.513f, 0.000f, 0.570f, 1.000f, 0.977f, 0.938f, 0.600f, -0.3f, 60.0f, 0.0f, 2, 2, 43.4f, 31.5f, 0, 0.700f, 0.076f, 0, 0.200f, 8.1f, 250.0f, 0.012f }, "Rumble" }, // fit 18.9 to OBI Punchy Kick 11.wav (rumble tail and power refit 2026-11-12)
    { "Wide Warm Thump F#1",       "Techno",            { -2.9f, 0.431f, 0.749f, 0.564f, 0.500f, 0.473f, 0.500f, 0.000f, 1.0f, 50.0f, 0.0f, 2, 1, 45.5f, 46.5f, 2, 0.515f, 0.969f, 0, 0.061f, 2.7f, 1000.0f, 0.000f }, "Wide" }, // fit 30.4 to OBI Punchy Kick 9.wav (rumble tail and power refit 2026-11-12, was Warm Thump F#1 II)
    { "Raw Thud",                  "Techno",            { -3.3f, 0.387f, 0.685f, 0.142f, 0.833f, 0.704f, 1.000f, 0.057f, -1.6f, 0.0f, 0.0f, 0, 1, 30.4f, 60.3f, 2, 0.055f, 0.000f, 0, 0.960f, 7.4f, 500.0f, 0.014f }, "Long" }, // fit 15.6 to SVEKick12S.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Crushed Clang A#1",         "Techno",            { 0.5f, 0.290f, 0.728f, 0.518f, 0.800f, 0.900f, 1.000f, 0.010f, -3.7f, 0.0f, 0.0f, 1, 1, 43.8f, 28.9f, 2, 0.728f, 0.167f, 0, 0.600f, 7.5f, 500.0f, 0.000f }, "Driven" }, // fit 19.1 to SVEKick08S.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Click A#1 IV)
    { "Crushed Clang F#1",         "Techno",            { -2.9f, 0.368f, 0.764f, 0.263f, 0.866f, 0.885f, 0.774f, 0.065f, -3.7f, 0.0f, 0.0f, 1, 1, 47.9f, 34.3f, 2, 0.449f, 0.252f, 0, 0.000f, 1.0f, 500.0f }, "Crushed" }, // fit 10.9 to OBI Punchy Kick 14.wav (kick register)
    { "Falling Thump",             "Techno",            { -3.5f, 0.376f, 0.997f, 0.632f, 0.975f, 0.610f, 0.383f, 0.100f, -1.6f, 0.0f, 0.0f, 0, 3, 29.8f, 36.9f, 1, 0.937f, 0.000f, 0, 0.000f, 8.7f, 500.0f }, "Driven" }, // fit 11.1 to OBI Punchy Kick 4.wav (kick register)
    { "Pitch Drop",                "Techno",            { -3.0f, 0.431f, 0.943f, 0.240f, 0.600f, 0.700f, 0.800f, 0.044f, -4.2f, 0.0f, 0.0f, 1, 1, 19.1f, 50.0f, 2, 0.083f, 0.049f, 0, 0.900f, 6.7f, 500.0f }, "Driven" }, // fit 12.6 to SVEKick07S.wav (kick register)
    { "Clipped Clang D#1",         "Techno",            { -5.6f, 0.444f, 1.000f, 0.160f, 0.803f, 0.682f, 1.000f, 0.053f, -3.6f, 0.8f, 0.0f, 1, 1, 31.0f, 63.2f, 2, 0.263f, 0.000f, 0, 1.000f, 6.0f, 500.0f, 0.156f }, "Long" }, // fit 17.0 to SVEKick13S.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Click D#1)
    { "Clipped Clang F1 II",       "Techno",            { -3.7f, 0.416f, 1.000f, 0.221f, 0.800f, 0.700f, 0.900f, 0.033f, -3.7f, 0.0f, 0.0f, 1, 1, 22.5f, 58.6f, 2, 0.100f, 0.064f, 0, 1.000f, 7.4f, 500.0f }, "Driven" }, // fit 12.9 to SVEKick09S.wav (kick register)
    { "Driven Clang G#1",          "Techno",            { -0.7f, 0.397f, 0.541f, 0.600f, 0.800f, 0.622f, 0.700f, 0.118f, -0.2f, 0.0f, 0.0f, 0, 0, 47.4f, 26.0f, 2, 0.545f, 0.400f, 0, 0.000f, 5.3f, 1600.0f }, "Crushed" }, // fit 9.8 to OBI Punchy Kick 8.wav (refit with tonality 2026-10-23, was Crushed Clang G#1)
    { "Warm Click A1",             "Techno",            { -0.3f, 0.276f, 0.976f, 0.511f, 0.991f, 0.321f, 0.996f, 0.010f, -3.3f, 0.0f, 0.0f, 0, 3, 21.2f, 55.8f, 1, 0.044f, 0.000f, 0, 0.100f, 6.8f, 500.0f, 0.000f }, "Short" }, // fit 16.2 to SVEKick05S.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Click A1)
    { "Classic Punch",             "Techno",            { -3.5f, 0.443f, 0.617f, 0.369f, 0.500f, 0.457f, 0.813f, 0.072f, -2.0f, 0.0f, 0.0f, 1, 1, 44.4f, 38.4f, 1, 0.700f, 0.157f, 0, 0.854f, 0.0f, 500.0f }, "Clean" }, // fit 8.4 to OBI Punchy Kick 18.wav (kick register)
    { "Ticking Body",              "Techno",            { 0.0f, 0.386f, 0.542f, 0.800f, 0.934f, 0.300f, 0.670f, 0.000f, -2.9f, 1.1f, 0.0f, 1, 1, 37.4f, 56.5f, 2, 0.759f, 0.190f, 0, 0.600f, 2.0f, 1000.0f }, "Clean" }, // fit 13.5 to OBI Punchy Kick 1.wav (refit with tonality 2026-10-23)
    { "Warm Rumble A1 II",         "Techno",            { -0.5f, 0.436f, 0.031f, 0.174f, 0.100f, 0.319f, 0.963f, 0.425f, -3.1f, 1.1f, 0.0f, 1, 1, 40.0f, 63.0f, 2, 0.188f, 0.000f, 0, 0.911f, 6.3f, 1600.0f, 0.177f }, "Clean" }, // fit 27.3 to OBI Punchy Kick 6.wav (rumble tail and power refit 2026-11-12)
    { "Warm Click F1 IV",          "Techno",            { -3.8f, 0.436f, 0.935f, 0.354f, 0.900f, 0.454f, 0.342f, 0.234f, -3.2f, 0.3f, 0.0f, 1, 1, 32.2f, 49.0f, 1, 0.300f, 0.029f, 0, 0.582f, 13.5f, 500.0f, 0.174f }, "Long" }, // fit 18.0 to OBI Punchy Kick 7.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Driven Clang D#1",          "Techno",            { -5.5f, 0.363f, 0.995f, 0.774f, 1.000f, 0.627f, 0.428f, 0.000f, -4.8f, 10.2f, 0.0f, 0, 3, 35.1f, 39.2f, 2, 0.992f, 0.000f, 0, 0.000f, 4.4f, 500.0f, 0.400f }, "Clean" }, // fit 14.0 to OBI Punchy Kick 17.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang D#1)
    { "Warm Thump F1",             "Techno",            { -4.3f, 0.466f, 0.591f, 0.343f, 0.300f, 0.367f, 0.739f, 0.099f, -2.0f, 8.1f, 0.0f, 1, 1, 31.9f, 35.9f, 2, 0.026f, 0.160f, 0, 1.000f, 4.2f, 1600.0f }, "Long" }, // fit 12.1 to OBI Punchy Kick 15.wav (kick register)
    // --- Hard Techno (110) ---
    { "Wide Warm Thump A#1",       "Hard Techno",       { 0.6f, 0.467f, 0.229f, 0.272f, 0.000f, 0.368f, 1.000f, 0.000f, -0.7f, 70.0f, 0.0f, 1, 0, 27.0f, 69.5f, 0, 0.559f, 0.832f, 0, 0.290f, 0.0f, 500.0f, 0.140f }, "Short" }, // fit 22.4 to HST1 Short Kick Shot  19.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Warm Click F#1 VI",         "Hard Techno",       { -3.0f, 0.272f, 0.933f, 0.540f, 1.000f, 0.359f, 0.000f, 0.001f, -3.3f, 6.3f, 0.0f, 1, 4, 21.7f, 38.8f, 1, 0.600f, 1.000f, 0, 1.000f, 21.6f, 1000.0f }, "Short" }, // fit 9.9 to HST1 Short Kick Shot  2.wav (kick register)
    { "Clean Knock C2",            "Hard Techno",       { 2.6f, 0.242f, 0.696f, 0.500f, 0.400f, 0.200f, 0.600f, 0.000f, -4.4f, 19.2f, 3.0f, 1, 3, 24.7f, 23.0f, 2, 0.388f, 0.104f, 0, 0.800f, 22.7f, 1600.0f }, "Short" }, // fit 10.3 to SVE2 Kick 10.1.wav (kick register)
    { "Crushed Knock F1",          "Hard Techno",       { -4.0f, 0.268f, 0.643f, 1.000f, 0.584f, 0.756f, 0.000f, 0.000f, 0.6f, 20.0f, 3.0f, 0, 1, 40.2f, 77.6f, 2, 0.710f, 0.034f, 0, 0.900f, 0.9f, 1600.0f, 0.300f }, "Short" }, // fit 20.8 to SVE2 Kick 7.1.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Clang F1 III)
    { "Warm Clang F1 III",         "Hard Techno",       { -3.8f, 0.279f, 0.558f, 0.700f, 0.600f, 0.384f, 0.343f, 0.000f, -0.9f, 0.0f, 0.0f, 0, 3, 33.0f, 75.5f, 2, 0.400f, 0.447f, 0, 0.100f, 6.6f, 1000.0f }, "Short" }, // fit 13.2 to HST1 Short Kick Shot  12.wav (kick register)
    { "Warm Knock F1",             "Hard Techno",       { -3.8f, 0.283f, 0.724f, 0.725f, 0.508f, 0.445f, 0.412f, 0.001f, -1.1f, 6.0f, 0.0f, 0, 3, 34.2f, 52.2f, 2, 0.900f, 0.192f, 0, 0.100f, 4.9f, 1000.0f, 0.027f }, "Short" }, // fit 16.4 to HST1 Short Kick Shot  11.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Clang F1)
    { "Clean Clang F#1",           "Hard Techno",       { -3.4f, 0.227f, 0.900f, 0.500f, 0.600f, 0.200f, 0.900f, 0.000f, -3.7f, 30.0f, 0.0f, 1, 3, 10.2f, 27.2f, 2, 0.100f, 0.149f, 0, 0.900f, 21.5f, 1000.0f }, "Short" }, // fit 10.4 to HST1 Short Kick Shot  5.wav (kick register)
    { "Driven Knock F1",           "Hard Techno",       { -3.6f, 0.274f, 0.447f, 0.930f, 0.687f, 0.622f, 0.317f, 0.000f, -0.5f, 19.2f, 0.0f, 0, 3, 34.9f, 75.3f, 1, 0.036f, 0.120f, 0, 0.418f, 11.8f, 400.0f, 0.152f }, "Short" }, // fit 14.9 to SVE2 Kick 6.1.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Click F1 II)
    { "Driven Click G1 II",        "Hard Techno",       { -1.8f, 0.297f, 0.033f, 0.480f, 0.900f, 0.600f, 0.400f, 0.000f, -3.6f, 20.9f, 0.0f, 0, 4, 41.5f, 62.8f, 1, 0.900f, 0.479f, 0, 0.000f, 9.7f, 1000.0f }, "Short" }, // fit 10.1 to SVE2 Kick 5.1.wav (kick register)
    { "Driven Clang G1 II",        "Hard Techno",       { -1.6f, 0.294f, 0.007f, 0.571f, 1.000f, 0.640f, 0.300f, 0.000f, -3.6f, 24.6f, 0.0f, 0, 4, 47.9f, 46.4f, 2, 0.800f, 0.338f, 0, 0.000f, 12.1f, 1000.0f, 0.200f }, "Short" }, // fit 12.7 to SVE2 Kick 5.1.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Click G1 III)
    { "Warm Click F1 V",           "Hard Techno",       { -3.9f, 0.345f, 0.526f, 0.634f, 0.800f, 0.318f, 0.900f, 0.244f, -1.4f, 0.0f, 0.0f, 1, 0, 47.6f, 46.2f, 0, 1.000f, 0.008f, 0, 1.000f, 1.9f, 500.0f }, "Short" }, // fit 13.3 to SVEKick02CC.wav (kick register)
    { "Wide Clipped Knock G1",     "Hard Techno",       { -2.1f, 0.268f, 0.505f, 0.900f, 0.100f, 0.616f, 0.404f, 0.042f, 0.8f, 50.0f, 0.0f, 1, 0, 47.5f, 64.7f, 2, 1.000f, 0.100f, 0, 0.817f, 1.1f, 1000.0f, 0.071f }, "Short" }, // fit 18.2 to SVE2 Kick 7.2.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Clipped Clang A1 IV",       "Hard Techno",       { -0.4f, 0.361f, 0.981f, 0.558f, 0.600f, 0.500f, 0.747f, 0.000f, -3.3f, 40.0f, 0.0f, 1, 4, 20.1f, 69.5f, 2, 0.510f, 0.262f, 0, 0.094f, 14.8f, 1000.0f }, "Driven" }, // fit 13.8 to SVE2 Kick 1.1.wav (kick register)
    { "Wide Warm Click F#1 II",    "Hard Techno",       { -3.0f, 0.296f, 0.936f, 0.500f, 0.998f, 0.400f, 0.036f, 0.000f, -3.7f, 60.0f, 0.0f, 1, 3, 37.7f, 22.2f, 1, 0.500f, 0.252f, 0, 1.000f, 6.1f, 1000.0f }, "Short" }, // fit 9.1 to HST1 Short Kick Shot  3.wav (kick register)
    { "Driven Clang F#1",          "Hard Techno",       { -2.9f, 0.259f, 0.357f, 0.837f, 0.900f, 0.552f, 0.700f, 0.000f, -3.9f, 40.0f, 0.0f, 0, 1, 38.7f, 90.9f, 2, 0.500f, 0.176f, 0, 0.700f, 0.2f, 1000.0f }, "Short" }, // fit 18.5 to SVE2 Kick 4.1.wav (kick register)
    { "Long Clip",                 "Hard Techno",       { -1.3f, 0.504f, 0.688f, 0.125f, 0.932f, 0.466f, 0.460f, 0.000f, -3.7f, 0.0f, 0.0f, 1, 4, 23.0f, 40.7f, 1, 1.000f, 1.000f, 0, 0.200f, 5.3f, 500.0f }, "Long" }, // fit 11.5 to SVEKick04C.wav (kick register)
    { "Sub Pressure",              "Hard Techno",       { 1.0f, 0.512f, 0.020f, 0.257f, 0.000f, 0.047f, 0.822f, 0.212f, -2.5f, 0.0f, 0.0f, 1, 0, 7.0f, 13.6f, 0, 0.646f, 0.000f, 0, 0.885f, 0.1f, 500.0f }, "Long" }, // fit 17.3 to SVE2 Sub 04.wav (refit with tonality 2026-10-23)
    { "Clean Drop A#1",            "Hard Techno",       { 0.5f, 0.429f, 0.200f, 0.605f, 0.000f, 0.180f, 1.000f, 0.000f, -3.8f, 2.4f, 0.0f, 1, 1, 27.0f, 31.3f, 0, 0.946f, 0.841f, 0, 0.900f, 13.3f, 500.0f }, "Long" }, // fit 17.6 to HST1 Short Kick Shot  20.wav (refit with tonality 2026-10-23, was Warm Click A#1)
    { "Driven Knock F1 II",        "Hard Techno",       { -3.6f, 0.239f, 0.855f, 0.958f, 0.200f, 0.682f, 1.000f, 0.109f, 0.3f, 1.0f, 0.0f, 0, 0, 30.5f, 70.5f, 2, 1.000f, 0.061f, 0, 0.300f, 4.5f, 600.0f, 0.084f }, "Long" }, // fit 15.3 to SVEKick11CC.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Click F1 II)
    { "Crushed Click G#1",         "Hard Techno",       { -1.0f, 0.342f, 0.060f, 0.400f, 1.000f, 0.811f, 1.000f, 0.155f, -2.3f, 0.0f, 0.0f, 1, 1, 48.0f, 50.6f, 0, 0.800f, 0.042f, 0, 0.463f, 2.5f, 1600.0f, 0.265f }, "Long" }, // fit 16.5 to SVEKick06C.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Click G#1)
    { "Clipped Click G#1 II",      "Hard Techno",       { -0.7f, 0.544f, 0.837f, 0.000f, 0.900f, 0.605f, 0.100f, 0.000f, -1.9f, 0.0f, 0.0f, 1, 3, 26.1f, 39.9f, 1, 1.000f, 0.844f, 0, 0.900f, 4.5f, 500.0f }, "Long" }, // fit 17.2 to SVEKick05CC.wav (kick register)
    { "Clean Rumble G1 III",       "Hard Techno",       { -1.5f, 0.468f, 0.836f, 0.194f, 0.100f, 0.196f, 1.000f, 0.332f, -0.7f, 20.0f, 0.0f, 1, 0, 21.7f, 58.7f, 1, 0.000f, 0.000f, 0, 0.669f, 0.1f, 500.0f, 0.088f }, "Clean" }, // fit 32.5 to HST1 Short Kick Shot  19.wav (rumble tail and power refit 2026-11-12, was Wide Clean Rumble G1)
    { "Grinding Click G#1",        "Hard Techno",       { -0.8f, 0.555f, 0.978f, 0.000f, 0.881f, 0.773f, 0.635f, 0.000f, 1.6f, 0.0f, 0.0f, 2, 1, 30.9f, 46.2f, 0, 0.300f, 0.595f, 0, 0.565f, 5.5f, 500.0f, 0.273f }, "Long" }, // fit 18.4 to SVEKick15C.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Drop G#1 II)
    { "Grinding Boom G1",          "Hard Techno",       { -2.0f, 0.409f, 0.519f, 0.994f, 0.600f, 0.524f, 0.820f, 0.000f, -1.3f, 10.0f, 0.0f, 2, 1, 33.3f, 77.0f, 0, 0.700f, 0.012f, 0, 0.128f, 0.1f, 600.0f, 0.055f }, "Long" }, // fit 22.7 to SVE2 Kick 2.3.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Click G1 II)
    { "Clipped Clang G#1 III",     "Hard Techno",       { -0.5f, 0.521f, 0.990f, 0.078f, 1.000f, 0.636f, 1.000f, 0.000f, -4.0f, 0.3f, 0.0f, 1, 1, 27.7f, 30.1f, 2, 0.063f, 0.796f, 0, 0.800f, 9.6f, 500.0f, 0.000f }, "Long" }, // fit 17.2 to SVEKick13C.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Click G#1 VI)
    { "Clipped Punch B1",          "Hard Techno",       { 1.6f, 0.529f, 0.951f, 0.019f, 0.000f, 0.651f, 1.000f, 0.000f, -1.7f, 0.0f, 0.0f, 1, 1, 28.8f, 48.9f, 0, 1.000f, 0.967f, 0, 0.600f, 0.0f, 1000.0f }, "Long" }, // fit 14.9 to SVEKick12C.wav (kick register)
    { "Warm Thump G#1",            "Hard Techno",       { -1.2f, 0.501f, 0.734f, 0.200f, 0.000f, 0.475f, 1.000f, 0.000f, -3.1f, 0.0f, 0.0f, 1, 1, 28.3f, 40.6f, 1, 0.678f, 0.986f, 0, 0.200f, 4.7f, 500.0f }, "Long" }, // fit 12.5 to SVEKick03C.wav (kick register)
    { "Crushed Thump G1 II",       "Hard Techno",       { -2.0f, 0.524f, 0.645f, 0.050f, 0.200f, 0.791f, 1.000f, 0.045f, -1.5f, 0.0f, 0.0f, 1, 1, 31.2f, 67.6f, 2, 0.000f, 0.000f, 0, 0.600f, 0.0f, 500.0f, 0.357f }, "Long" }, // fit 15.8 to SVEKick09CC.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Click G1)
    { "Clipped Drop G1",           "Hard Techno",       { -1.5f, 0.411f, 0.896f, 0.346f, 0.200f, 0.554f, 0.934f, 0.080f, 0.2f, 0.0f, 0.0f, 1, 2, 47.8f, 7.8f, 2, 0.400f, 0.032f, 0, 0.499f, 16.9f, 500.0f, 0.085f }, "Long" }, // fit 17.9 to SVEKick04CC.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Clipped Click G1 IV",       "Hard Techno",       { -2.0f, 0.497f, 0.953f, 0.049f, 0.800f, 0.663f, 0.829f, 0.000f, -3.2f, 0.0f, 0.0f, 1, 3, 24.7f, 50.7f, 1, 0.353f, 0.993f, 0, 0.143f, 4.8f, 500.0f, 0.141f }, "Long" }, // fit 17.3 to SVEKick09C.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Punch G1)
    { "Wide Clipped Thump B1",     "Hard Techno",       { 1.7f, 0.336f, 0.556f, 0.732f, 0.200f, 0.662f, 0.200f, 0.200f, -2.0f, 70.0f, 0.0f, 1, 1, 47.3f, 40.4f, 1, 0.000f, 0.000f, 0, 0.900f, 0.0f, 1600.0f, 0.022f }, "Rumble" }, // fit 22.1 to SVE2 Kick 9.3.wav (rumble tail and power refit 2026-11-12, was Wide Clipped Rumble B1)
    { "Grinding Drop F#1",         "Hard Techno",       { -2.5f, 0.509f, 0.897f, 0.167f, 0.400f, 0.640f, 0.800f, 0.000f, 0.7f, 0.7f, 0.0f, 2, 4, 6.1f, 111.8f, 1, 1.000f, 0.825f, 0, 0.439f, 10.6f, 500.0f, 0.600f }, "Long" }, // fit 21.8 to SVEKick08C.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Click F#1)
    { "Warm Drop G1",              "Hard Techno",       { -2.1f, 0.399f, 0.927f, 0.191f, 0.600f, 0.321f, 1.000f, 0.192f, 0.9f, 2.7f, 0.0f, 1, 0, 19.5f, 51.6f, 0, 0.600f, 0.000f, 0, 0.762f, 11.7f, 500.0f, 0.094f }, "Long" }, // fit 19.5 to HST1 Short Kick Shot  20.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Drop G1 II)
    { "Crushed Drop G1 II",        "Hard Techno",       { -2.2f, 0.461f, 0.863f, 0.175f, 0.000f, 0.949f, 0.200f, 0.200f, 1.1f, 30.0f, 0.0f, 1, 2, 35.8f, 25.9f, 1, 1.000f, 0.000f, 0, 0.800f, 11.4f, 1000.0f, 0.800f }, "Wide" }, // fit 21.2 to SVE2 Kick 7.3.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Warm Boom G1)
    { "Wide Warm Rumble E1",       "Hard Techno",       { -4.5f, 0.305f, 0.966f, 0.514f, 0.100f, 0.281f, 1.000f, 0.407f, -1.1f, 54.7f, 0.0f, 1, 0, 38.2f, 45.2f, 1, 1.000f, 0.000f, 0, 0.761f, 0.0f, 500.0f, 0.300f }, "Long" }, // fit 42.5 to HST1 Short Kick Shot  19.wav (rumble tail and power refit 2026-11-12)
    { "Wide Clipped Drop F#1",     "Hard Techno",       { -3.5f, 0.521f, 0.897f, 0.012f, 0.677f, 0.620f, 0.474f, 0.000f, -3.7f, 50.0f, 0.0f, 1, 4, 29.4f, 63.0f, 1, 0.000f, 0.427f, 0, 0.523f, 6.0f, 1000.0f }, "Long" }, // fit 15.1 to SVE2 Kick 5.2.wav (kick register)
    { "Rumble Tail",               "Hard Techno",       { -1.6f, 0.294f, 0.932f, 0.401f, 1.000f, 0.384f, 0.938f, 0.500f, -0.5f, 0.4f, 0.0f, 1, 0, 47.1f, 21.2f, 0, 1.000f, 0.000f, 0, 0.830f, 2.6f, 500.0f, 0.027f }, "Rumble" }, // fit 35.4 to HST1 Rumble Kick Shot 31.wav (rumble tail and power refit 2026-11-12)
    { "Clipped Rumble A#1",        "Hard Techno",       { 0.8f, 0.379f, 0.662f, 0.528f, 0.978f, 0.611f, 0.195f, 0.460f, -1.3f, 0.1f, 0.0f, 1, 4, 29.6f, 39.7f, 1, 0.403f, 0.000f, 0, 0.343f, 0.0f, 500.0f, 0.000f }, "Rumble" }, // fit 16.7 to OBI Hard Rumble Kick 11.wav (rumble tail and power refit 2026-11-12)
    { "Warm Rumble B1 III",        "Hard Techno",       { 2.1f, 0.390f, 0.908f, 0.537f, 1.000f, 0.469f, 0.704f, 0.938f, -1.1f, 1.0f, 0.0f, 1, 0, 30.7f, 46.0f, 2, 0.175f, 0.030f, 0, 0.700f, 0.0f, 500.0f, 0.200f }, "Rumble" }, // fit 18.4 to OBI Hard Rumble Kick 2.wav (rumble tail and power refit 2026-11-12)
    { "Warm Rumble D#1",           "Hard Techno",       { -5.9f, 0.326f, 0.888f, 0.400f, 0.864f, 0.400f, 0.830f, 0.400f, 0.1f, 0.9f, 0.0f, 1, 0, 26.3f, 64.3f, 2, 0.016f, 0.000f, 0, 0.948f, 0.5f, 500.0f, 0.182f }, "Clean" }, // fit 39.4 to HST1 Rumble Kick Shot 33.wav (rumble tail and power refit 2026-11-12)
    { "Warm Rumble A1 IV",         "Hard Techno",       { 0.2f, 0.317f, 0.999f, 0.169f, 1.000f, 0.484f, 0.807f, 0.500f, 1.2f, 0.0f, 0.0f, 1, 0, 48.0f, 10.2f, 1, 0.100f, 0.000f, 0, 0.951f, 9.5f, 500.0f, 0.179f }, "Rumble" }, // fit 36.2 to HST1 Rumble Kick Shot 14.wav (rumble tail and power refit 2026-11-12, was Clipped Rumble A1)
    { "Driven Rumble A#1",         "Hard Techno",       { 1.4f, 0.385f, 0.848f, 0.799f, 0.744f, 0.631f, 0.540f, 0.600f, -2.4f, 0.0f, 0.0f, 0, 3, 30.9f, 28.6f, 1, 1.000f, 0.000f, 0, 0.100f, 2.2f, 1600.0f, 0.386f }, "Rumble" }, // fit 18.9 to OBI Hard Rumble Kick 26.wav (rumble tail and power refit 2026-11-12)
    { "Crushed Click B1",          "Hard Techno",       { 2.4f, 0.372f, 0.648f, 0.542f, 0.962f, 0.856f, 1.000f, 0.161f, 0.3f, 0.0f, 0.0f, 0, 0, 33.0f, 23.6f, 1, 0.000f, 0.089f, 0, 0.543f, 1.9f, 500.0f }, "Crushed" }, // fit 14.8 to OBI Hard Rumble Kick 25.wav (kick register)
    { "Driven Click B1 II",        "Hard Techno",       { 2.3f, 0.384f, 0.663f, 0.542f, 1.000f, 0.688f, 0.769f, 0.162f, -4.0f, 0.0f, 0.0f, 0, 4, 36.6f, 34.9f, 1, 0.300f, 0.067f, 0, 0.296f, 0.0f, 500.0f, 0.074f }, "Rumble" }, // fit 22.1 to SVEKick10C.wav (rumble tail and power refit 2026-11-12)
    { "Driven Click E1",           "Hard Techno",       { -4.8f, 0.308f, 0.708f, 1.000f, 0.900f, 0.738f, 1.000f, 0.286f, -1.6f, 0.0f, 0.0f, 0, 1, 31.5f, 55.1f, 1, 0.700f, 0.000f, 0, 0.262f, 0.8f, 600.0f, 0.500f }, "Rumble" }, // fit 22.2 to ITHTS Rumble Kick 9.wav (rumble tail and power refit 2026-11-12)
    { "Wide Crushed Rumble E1",    "Hard Techno",       { -4.7f, 0.406f, 0.832f, 0.389f, 0.800f, 0.753f, 0.243f, 0.300f, 0.3f, 50.0f, 0.0f, 1, 2, 27.3f, 35.1f, 1, 0.800f, 0.000f, 0, 0.900f, 20.0f, 1000.0f, 1.000f }, "Wide" }, // fit 17.5 to SVE2 Kick 6.3.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Warm Click E1 II)
    { "Clean Rumble E1",           "Hard Techno",       { -4.5f, 0.233f, 0.724f, 0.880f, 0.019f, 0.236f, 1.000f, 0.443f, -0.8f, 40.0f, 0.0f, 1, 0, 26.8f, 82.7f, 1, 1.000f, 0.000f, 0, 0.754f, 10.6f, 500.0f, 0.459f }, "Clean" }, // fit 39.2 to HST1 Rumble Kick Shot 17.wav (rumble tail and power refit 2026-11-12)
    { "Wide Clipped Boom B1",      "Hard Techno",       { 2.1f, 0.424f, 0.995f, 0.853f, 0.500f, 0.528f, 0.446f, 0.000f, -2.9f, 53.9f, 0.0f, 1, 4, 29.3f, 41.9f, 2, 0.264f, 0.493f, 0, 0.574f, 4.1f, 1000.0f, 0.307f }, "Wide" }, // fit 12.8 to HST1 Dirty Kick Shot 14.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Clipped Drop B1)
    { "Wide Crushed Boom A#1",     "Hard Techno",       { 1.1f, 0.321f, 0.655f, 0.900f, 0.650f, 0.786f, 0.044f, 0.200f, -1.0f, 60.0f, 0.0f, 0, 4, 43.1f, 40.2f, 0, 0.062f, 0.000f, 0, 1.000f, 4.9f, 1600.0f, 0.094f }, "Wide" }, // fit 22.0 to SVE2 Kick 9.2.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Warm Boom A#1)
    { "Crushed Knock G1",          "Hard Techno",       { -1.6f, 0.246f, 0.383f, 0.960f, 0.046f, 0.818f, 0.097f, 0.293f, 0.2f, 14.0f, 0.0f, 1, 2, 44.0f, 43.3f, 2, 0.001f, 0.000f, 0, 0.394f, 7.7f, 1600.0f, 0.064f }, "Driven" }, // fit 19.0 to HST1 Rumble Kick Shot 13.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Clang G1 III)
    { "Crushed Clang B1 II",       "Hard Techno",       { 1.5f, 0.421f, 0.550f, 0.900f, 0.900f, 0.800f, 0.632f, 0.000f, -4.9f, 40.0f, 0.0f, 0, 1, 33.4f, 30.1f, 2, 0.647f, 0.763f, 0, 0.300f, 0.8f, 500.0f }, "Wide" }, // fit 29.1 to SVE2 Sub 10.wav (refit with tonality 2026-10-23, was Wide Clean Boom B1)
    { "Clean Clang D1",            "Hard Techno",       { -7.0f, 0.427f, 0.395f, 1.000f, 0.900f, 0.194f, 0.300f, 0.000f, -1.3f, 30.0f, 0.0f, 1, 2, 45.7f, 7.3f, 2, 0.280f, 0.880f, 0, 1.000f, 18.2f, 1000.0f }, "Clean" }, // fit 10.7 to HST1 Dirty Kick Shot 18.wav (kick register)
    { "Wide Warm Rumble D#1",      "Hard Techno",       { -5.7f, 0.287f, 0.966f, 0.696f, 0.100f, 0.423f, 0.608f, 0.383f, -2.1f, 70.0f, 0.0f, 1, 1, 32.7f, 64.4f, 2, 0.800f, 0.000f, 0, 0.935f, 3.2f, 1600.0f, 0.098f }, "Wide" }, // fit 35.3 to HST1 Dirty Kick Shot 1.wav (rumble tail and power refit 2026-11-12)
    { "Wide Clipped Drop F1",      "Hard Techno",       { -4.4f, 0.320f, 0.000f, 0.900f, 0.300f, 0.607f, 0.300f, 0.200f, -0.9f, 60.0f, 0.0f, 1, 0, 48.0f, 65.4f, 1, 0.700f, 0.000f, 0, 0.000f, 6.6f, 1600.0f, 0.307f }, "Wide" }, // fit 16.5 to HST1 Rumble Kick Shot 10.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Warm Click F1)
    { "Clipped Clang G1",          "Hard Techno",       { -2.5f, 0.420f, 0.014f, 0.986f, 0.900f, 0.647f, 0.500f, 0.000f, -3.7f, 0.0f, 0.0f, 1, 1, 47.9f, 42.7f, 2, 0.000f, 0.681f, 0, 0.200f, 13.9f, 500.0f, 0.678f }, "Driven" }, // fit 14.1 to HST1 Rumble Kick Shot 18.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Click G1 III)
    { "Warm Punch F#1",            "Hard Techno",       { -3.4f, 0.435f, 0.812f, 0.692f, 0.587f, 0.462f, 0.230f, 0.000f, -1.1f, 20.2f, 0.0f, 1, 3, 33.6f, 80.3f, 1, 0.700f, 0.957f, 0, 0.700f, 0.0f, 1000.0f, 0.448f }, "Driven" }, // fit 14.2 to HST1 Rumble Kick Shot 16.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Clang F#1)
    { "Hard Techno Core",          "Hard Techno",       { -2.5f, 0.429f, 0.029f, 0.800f, 1.000f, 0.380f, 0.800f, 0.000f, -3.1f, 20.0f, 0.0f, 1, 1, 39.0f, 50.9f, 1, 0.500f, 0.980f, 0, 0.500f, 13.6f, 600.0f }, "Clean" }, // fit 9.5 to HST1 Rumble Kick Shot 25.wav (kick register)
    { "Wide Clipped Clang G#1",    "Hard Techno",       { -1.5f, 0.422f, 0.259f, 0.854f, 0.700f, 0.530f, 0.700f, 0.000f, -0.8f, 60.0f, 0.0f, 1, 2, 46.4f, 20.6f, 2, 0.153f, 0.977f, 0, 0.100f, 24.0f, 1600.0f, 0.532f }, "Clean" }, // fit 25.3 to HST1 Rumble Kick Shot 20.wav (rumble tail and power refit 2026-11-12, was Wide Clipped Rumble G#1)
    { "Grinding Drop G#1",         "Hard Techno",       { -0.7f, 0.425f, 0.597f, 0.900f, 0.200f, 0.614f, 0.878f, 0.000f, -0.3f, 0.0f, 0.0f, 2, 3, 47.9f, 16.5f, 0, 0.229f, 0.029f, 0, 0.300f, 9.3f, 500.0f, 0.228f }, "Driven" }, // fit 27.0 to HST1 Dirty Kick Shot 11.wav (rumble tail and power refit 2026-11-12, was Clipped Boom G#1 II)
    { "Warm Rumble F1 II",         "Hard Techno",       { -3.7f, 0.288f, 0.331f, 0.614f, 0.700f, 0.456f, 0.300f, 0.494f, -1.1f, 17.7f, 0.0f, 1, 1, 47.2f, 62.8f, 0, 0.290f, 0.000f, 0, 0.303f, 4.2f, 1600.0f, 0.000f }, "Driven" }, // fit 22.0 to ITHTS Rumble Kick 2.wav (rumble tail and power refit 2026-11-12)
    { "Crushed Thump G1",          "Hard Techno",       { -1.8f, 0.352f, 0.663f, 0.279f, 0.006f, 0.797f, 0.134f, 0.193f, 1.0f, 26.8f, 0.0f, 1, 2, 46.7f, 48.7f, 2, 0.900f, 0.000f, 0, 0.712f, 0.4f, 500.0f, 0.188f }, "Driven" }, // fit 38.8 to HST1 Dirty Kick Shot 20.wav (rumble tail and power refit 2026-11-12, was Crushed Rumble G1)
    { "Clipped Clang F#1 IV",      "Hard Techno",       { -3.2f, 0.425f, 1.000f, 0.800f, 0.705f, 0.632f, 0.600f, 0.000f, 0.5f, 0.0f, 0.0f, 1, 0, 31.4f, 33.3f, 2, 0.220f, 0.427f, 0, 0.046f, 10.8f, 500.0f, 1.000f }, "Clean" }, // fit 14.1 to HST1 Rumble Kick Shot 4.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang F#1)
    { "Clipped Clang G1 V",        "Hard Techno",       { -2.2f, 0.349f, 0.204f, 0.768f, 0.923f, 0.566f, 0.212f, 0.092f, -2.0f, 0.4f, 0.0f, 1, 2, 29.6f, 32.8f, 2, 0.677f, 0.000f, 0, 0.708f, 18.2f, 500.0f, 0.220f }, "Driven" }, // fit 19.1 to HST1 Rumble Kick Shot 22.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Warm Clang A1 IV",          "Hard Techno",       { 0.3f, 0.329f, 0.877f, 0.800f, 0.800f, 0.400f, 0.700f, 0.000f, -4.2f, 5.3f, 0.0f, 0, 1, 28.7f, 62.6f, 2, 0.800f, 0.085f, 0, 0.000f, 0.4f, 1000.0f }, "Short" }, // fit 15.5 to HST1 Short Kick Shot  7.wav (kick register)
    { "Clipped Rumble D#1",        "Hard Techno",       { -5.7f, 0.334f, 0.579f, 0.395f, 0.400f, 0.540f, 0.157f, 0.500f, -0.1f, 30.0f, 0.0f, 1, 1, 46.8f, 49.4f, 2, 0.000f, 0.000f, 0, 0.605f, 0.9f, 1600.0f, 0.093f }, "Clean" }, // fit 47.0 to HST1 Rumble Kick Shot 39.wav (rumble tail and power refit 2026-11-12)
    { "Grinding Click A1",         "Hard Techno",       { -0.1f, 0.342f, 0.200f, 0.399f, 0.800f, 0.586f, 0.653f, 0.100f, 1.5f, 0.0f, 0.0f, 2, 4, 9.8f, 9.4f, 0, 0.500f, 0.100f, 0, 0.400f, 24.0f, 500.0f, 0.046f }, "Long" }, // fit 19.4 to SVEKick14C.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Punch A1)
    { "Warm Clang D1",             "Hard Techno",       { -7.5f, 0.355f, 0.863f, 0.520f, 0.995f, 0.402f, 0.030f, 0.000f, -2.9f, 20.0f, 0.0f, 1, 3, 34.0f, 70.9f, 2, 0.221f, 0.986f, 0, 0.800f, 5.5f, 1000.0f, 0.775f }, "Driven" }, // fit 15.9 to HST1 Short Kick Shot  1.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Clang D1 II)
    { "Driven Click F#1 II",       "Hard Techno",       { -3.3f, 0.362f, 0.697f, 1.000f, 1.000f, 0.539f, 1.000f, 0.000f, -2.3f, 18.4f, 0.0f, 0, 1, 30.4f, 71.5f, 0, 0.900f, 0.000f, 0, 0.597f, 5.3f, 600.0f }, "Driven" }, // fit 17.8 to SVE2 Kick 3.1.wav (kick register)
    { "Warm Thump A1",             "Hard Techno",       { -0.4f, 0.456f, 0.322f, 0.300f, 0.000f, 0.485f, 0.900f, 0.000f, -3.1f, 2.9f, 0.0f, 1, 1, 28.3f, 46.0f, 0, 1.000f, 0.977f, 0, 0.100f, 4.5f, 500.0f }, "Clean" }, // fit 19.1 to HST1 Short Kick Shot  20.wav (refit with tonality 2026-10-23, was Warm Click A1 IV)
    { "Wide Driven Clang A#1",     "Hard Techno",       { 1.4f, 0.424f, 0.316f, 0.900f, 0.981f, 0.521f, 0.900f, 0.000f, -5.2f, 70.0f, 0.0f, 0, 1, 29.8f, 44.5f, 2, 0.200f, 0.405f, 0, 0.100f, 1.9f, 500.0f, 0.500f }, "Wide" }, // fit 22.7 to HST1 Rumble Kick Shot 37.wav (rumble tail and power refit 2026-11-12, was Warm Rumble A#1 II)
    { "Deep Drop",                 "Hard Techno",       { -2.3f, 0.460f, 0.221f, 0.911f, 1.000f, 0.164f, 0.800f, 0.003f, -3.2f, 0.0f, 0.0f, 1, 1, 16.7f, 23.4f, 2, 0.171f, 0.700f, 0, 0.918f, 19.4f, 600.0f }, "Long" }, // fit 12.4 to OBI Hard Rumble Kick 21.wav (refit with tonality 2026-10-23)
    { "Falling Clipper",           "Hard Techno",       { 0.6f, 0.422f, 0.895f, 0.840f, 0.691f, 0.509f, 0.735f, 0.001f, 0.2f, 0.0f, 0.0f, 1, 0, 31.9f, 29.8f, 2, 0.200f, 0.300f, 0, 0.007f, 11.0f, 500.0f, 0.900f }, "Clean" }, // fit 20.9 to OBI Hard Rumble Kick 24.wav (rumble tail and power refit 2026-11-12)
    { "Clean Rumble F1",           "Hard Techno",       { -3.9f, 0.309f, 0.348f, 0.391f, 0.000f, 0.249f, 0.951f, 0.421f, -1.3f, 20.0f, 0.0f, 1, 0, 42.1f, 39.7f, 1, 1.000f, 0.000f, 0, 0.561f, 4.7f, 500.0f, 0.114f }, "Clean" }, // fit 39.2 to HST1 Short Kick Shot  19.wav (rumble tail and power refit 2026-11-12, was Wide Warm Rumble F1)
    { "Driven Clang A#1 III",      "Hard Techno",       { 1.0f, 0.424f, 0.900f, 0.800f, 0.946f, 0.500f, 0.300f, 0.000f, -1.5f, 0.0f, 0.0f, 0, 1, 30.7f, 63.7f, 2, 0.516f, 0.089f, 0, 0.000f, 0.0f, 1000.0f }, "Driven" }, // fit 12.1 to BORKICK 07.wav (kick register)
    { "Warm Knock G1",             "Hard Techno",       { -2.5f, 0.255f, 1.000f, 0.977f, 0.500f, 0.484f, 0.000f, 0.271f, -0.7f, 0.0f, 0.0f, 0, 2, 30.6f, 48.0f, 2, 0.879f, 0.000f, 0, 0.192f, 12.0f, 500.0f, 0.000f }, "Clean" }, // fit 20.3 to HST1 Rumble Kick Shot 27.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang G1 VI)
    { "Warm Drop G1 II",           "Hard Techno",       { -2.0f, 0.398f, 0.946f, 0.546f, 0.700f, 0.462f, 1.000f, 0.283f, -0.3f, 16.6f, 0.0f, 1, 0, 10.2f, 4.0f, 1, 0.600f, 0.000f, 0, 0.366f, 21.0f, 500.0f, 0.172f }, "Clean" }, // fit 18.6 to ITHTS Rumble Kick 1.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Drop G1 III)
    { "Warm Click G1 III",         "Hard Techno",       { -2.3f, 0.428f, 0.870f, 0.882f, 1.000f, 0.370f, 0.336f, 0.000f, -0.5f, 0.0f, 0.0f, 1, 2, 44.7f, 26.2f, 0, 0.400f, 0.591f, 0, 0.477f, 15.3f, 1000.0f, 0.564f }, "Clean" }, // fit 22.8 to HST1 Rumble Kick Shot 40.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Clipped Rumble F#1 V",      "Hard Techno",       { -2.6f, 0.467f, 0.937f, 0.292f, 0.900f, 0.538f, 0.561f, 0.400f, 0.0f, 0.0f, 0.0f, 1, 0, 39.7f, 10.4f, 1, 0.200f, 0.000f, 0, 0.832f, 16.9f, 500.0f, 0.600f }, "Clean" }, // fit 23.8 to OBI Hard Rumble Kick 5.wav (rumble tail and power refit 2026-11-12)
    { "Driven Drop G1",            "Hard Techno",       { -2.0f, 0.359f, 0.111f, 0.366f, 0.024f, 0.654f, 0.900f, 0.127f, 0.4f, 29.4f, 0.0f, 0, 0, 40.8f, 38.8f, 0, 0.609f, 0.000f, 0, 0.408f, 12.8f, 500.0f, 0.230f }, "Clean" }, // fit 38.0 to HST1 Dirty Kick Shot 17.wav (rumble tail and power refit 2026-11-12, was Wide Driven Rumble G1)
    { "Clipped Drop D#1",          "Hard Techno",       { -6.3f, 0.432f, 0.900f, 0.642f, 0.000f, 0.623f, 0.700f, 0.000f, 0.0f, 0.0f, 0.0f, 1, 0, 33.6f, 10.0f, 2, 0.020f, 0.083f, 0, 0.600f, 15.4f, 500.0f, 0.300f }, "Clean" }, // fit 17.6 to HST1 Dirty Kick Shot 6.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Click D#1)
    { "Driven Clang G1 III",       "Hard Techno",       { -1.8f, 0.467f, 0.095f, 0.418f, 0.768f, 0.561f, 0.763f, 0.000f, -1.6f, 8.3f, 0.0f, 0, 4, 7.8f, 59.5f, 2, 0.000f, 0.272f, 0, 0.542f, 10.1f, 500.0f, 0.556f }, "Clean" }, // fit 27.0 to BORKICK 15.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Click G1 VI)
    { "Clipped Rumble F1",         "Hard Techno",       { -3.6f, 0.342f, 0.976f, 0.433f, 1.000f, 0.612f, 0.850f, 0.500f, 0.9f, 0.0f, 0.0f, 1, 0, 35.6f, 28.3f, 2, 0.041f, 0.131f, 0, 0.348f, 20.5f, 500.0f, 0.042f }, "Clean" }, // fit 24.9 to OBI Hard Rumble Kick 4.wav (rumble tail and power refit 2026-11-12)
    { "Warm Thump F#1 III",        "Hard Techno",       { -3.0f, 0.456f, 0.038f, 0.510f, 0.344f, 0.420f, 0.257f, 0.000f, -0.4f, 20.0f, 0.0f, 1, 2, 34.2f, 82.5f, 2, 0.079f, 0.918f, 0, 0.570f, 4.4f, 600.0f, 0.562f }, "Clean" }, // fit 17.4 to ITHTS Rumble Kick 1.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Click F#1)
    { "Warm Rumble G#1",           "Hard Techno",       { -1.1f, 0.429f, 0.585f, 0.100f, 1.000f, 0.468f, 0.542f, 0.300f, 2.0f, 17.4f, 0.0f, 1, 0, 44.9f, 56.6f, 0, 1.000f, 0.000f, 0, 1.000f, 0.0f, 500.0f, 0.200f }, "Clean" }, // fit 18.0 to SVE2 Kick 10.2.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Clang G#1)
    { "Crushed Click A#1 II",      "Hard Techno",       { 1.5f, 0.374f, 0.698f, 0.534f, 1.000f, 0.817f, 1.000f, 0.100f, 0.8f, 0.8f, 0.0f, 0, 0, 33.3f, 49.7f, 1, 0.100f, 0.000f, 0, 0.552f, 0.7f, 500.0f, 0.000f }, "Clean" }, // fit 22.5 to BORKICK 12.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Click A#1)
    { "Warm Click B1",             "Hard Techno",       { 1.6f, 0.398f, 0.461f, 0.357f, 1.000f, 0.445f, 1.000f, 0.273f, 3.4f, 0.0f, 0.0f, 2, 0, 47.1f, 33.7f, 0, 0.700f, 0.000f, 0, 1.000f, 2.1f, 500.0f, 0.092f }, "Clean" }, // fit 19.5 to BORKICK 06.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Thump B1)
    { "Driven Click G1 IV",        "Hard Techno",       { -1.6f, 0.420f, 0.821f, 0.989f, 1.000f, 0.520f, 0.900f, 0.006f, -0.2f, 0.5f, 0.0f, 0, 0, 37.0f, 46.9f, 1, 0.500f, 0.238f, 0, 0.126f, 5.1f, 600.0f, 0.575f }, "Clean" }, // fit 15.0 to OBI Hard Rumble Kick 29.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Click G1)
    { "Warm Rumble G#1 II",        "Hard Techno",       { -1.4f, 0.360f, 0.650f, 0.333f, 1.000f, 0.261f, 0.938f, 0.495f, -4.8f, 20.0f, 0.0f, 1, 3, 31.8f, 49.6f, 1, 1.000f, 0.000f, 0, 0.900f, 5.5f, 500.0f, 0.600f }, "Clean" }, // fit 33.4 to SVE2 Kick 3.3.wav (rumble tail and power refit 2026-11-12)
    { "Clipped Rumble G1",         "Hard Techno",       { -2.0f, 0.409f, 0.472f, 0.190f, 0.912f, 0.508f, 0.137f, 0.342f, -2.3f, 0.0f, 0.0f, 1, 0, 45.8f, 41.3f, 1, 0.700f, 0.000f, 0, 0.451f, 12.0f, 500.0f, 0.092f }, "Clean" }, // fit 26.3 to OBI Hard Rumble Kick 8.wav (rumble tail and power refit 2026-11-12)
    { "Warm Clang G1",             "Hard Techno",       { -1.5f, 0.480f, 0.708f, 0.500f, 0.880f, 0.466f, 0.706f, 0.100f, 0.2f, 0.0f, 0.0f, 1, 0, 39.8f, 39.5f, 2, 0.055f, 0.003f, 0, 1.000f, 5.7f, 500.0f, 0.279f }, "Clean" }, // fit 24.0 to OBI Hard Rumble Kick 5.wav (rumble tail and power refit 2026-11-12, was Warm Rumble G1 IV)
    { "Warm Knock G#1",            "Hard Techno",       { -0.9f, 0.286f, 0.100f, 0.591f, 0.700f, 0.351f, 0.973f, 0.248f, -1.7f, 20.0f, 0.0f, 1, 0, 13.8f, 77.0f, 0, 0.000f, 0.000f, 0, 0.485f, 1.3f, 500.0f, 0.125f }, "Clean" }, // fit 22.1 to HST1 Short Kick Shot  19.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Warm Click G#1 II)
    { "Warm Click D1",             "Hard Techno",       { -7.1f, 0.525f, 0.757f, 0.009f, 1.000f, 0.424f, 0.633f, 0.000f, -1.7f, 10.0f, 0.0f, 1, 1, 26.3f, 30.1f, 0, 0.441f, 0.723f, 0, 0.629f, 5.2f, 400.0f }, "Clean" }, // fit 14.0 to HST1 Short Kick Shot  14.wav (refit with tonality 2026-10-23, was Warm Drop D1)
    { "Driven Drop F1 II",         "Hard Techno",       { -3.5f, 0.405f, 0.063f, 0.379f, 0.300f, 0.510f, 0.646f, 0.132f, -0.1f, 0.0f, 0.0f, 0, 0, 43.3f, 4.0f, 1, 1.000f, 0.000f, 0, 0.000f, 22.2f, 500.0f, 0.044f }, "Clean" }, // fit 44.8 to HST1 Short Kick Shot  18.wav (rumble tail and power refit 2026-11-12, was Warm Rumble F1 III)
    { "Warm Rumble A1 III",        "Hard Techno",       { -0.3f, 0.297f, 0.980f, 0.443f, 0.000f, 0.488f, 1.000f, 0.318f, 1.6f, 0.0f, 0.0f, 1, 0, 32.8f, 60.9f, 0, 0.724f, 0.000f, 0, 0.633f, 0.3f, 500.0f, 0.120f }, "Clean" }, // fit 41.6 to BORKICK 14.wav (rumble tail and power refit 2026-11-12)
    { "Warm Clang G#1 V",          "Hard Techno",       { -0.6f, 0.541f, 0.311f, 0.052f, 0.719f, 0.493f, 0.700f, 0.000f, 2.0f, 0.0f, 0.0f, 1, 0, 47.7f, 47.5f, 2, 0.366f, 0.029f, 0, 0.769f, 8.6f, 500.0f, 0.300f }, "Clean" }, // fit 20.4 to BORKICK 01.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Driven Rumble D#1",         "Hard Techno",       { -5.8f, 0.294f, 0.596f, 1.000f, 1.000f, 0.584f, 0.600f, 0.500f, -1.0f, 0.0f, 0.0f, 0, 1, 20.8f, 71.4f, 0, 0.900f, 0.071f, 0, 0.200f, 9.2f, 600.0f, 0.400f }, "Clean" }, // fit 25.3 to OBI Hard Rumble Kick 27.wav (rumble tail and power refit 2026-11-12)
    { "Warm Drop F#1",             "Hard Techno",       { -2.5f, 0.470f, 0.240f, 0.071f, 0.729f, 0.402f, 1.000f, 0.293f, -3.7f, 0.0f, 0.0f, 1, 3, 46.7f, 26.1f, 1, 0.370f, 0.000f, 0, 0.500f, 11.6f, 500.0f, 0.963f }, "Driven" }, // fit 38.1 to BORKICK 03.wav (rumble tail and power refit 2026-11-12, was Warm Rumble F#1 II)
    { "Warm Rumble G1 V",          "Hard Techno",       { -1.5f, 0.347f, 0.987f, 0.294f, 1.000f, 0.274f, 1.000f, 0.799f, -0.5f, 0.0f, 0.0f, 1, 0, 40.5f, 38.5f, 1, 0.800f, 0.000f, 0, 0.757f, 9.9f, 500.0f, 0.387f }, "Clean" }, // fit 46.0 to HST1 Rumble Kick Shot 30.wav (rumble tail and power refit 2026-11-12)
    { "Driven Thump A#1",          "Hard Techno",       { 1.0f, 0.348f, 0.456f, 0.436f, 0.158f, 0.600f, 1.000f, 0.200f, -0.7f, 20.0f, 0.0f, 0, 0, 27.5f, 4.0f, 2, 0.051f, 0.000f, 0, 0.893f, 2.4f, 500.0f, 0.061f }, "Clean" }, // fit 24.9 to HST1 Short Kick Shot  19.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Thump A#1 II)
    { "Clipped Click F#1",         "Hard Techno",       { -2.8f, 0.379f, 0.312f, 0.515f, 0.800f, 0.666f, 0.001f, 0.198f, -2.3f, 10.0f, 0.0f, 1, 3, 47.2f, 18.8f, 0, 0.784f, 0.700f, 0, 0.670f, 19.3f, 1000.0f, 0.328f }, "Clean" }, // fit 31.4 to SVE2 Kick 8.2.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Warm Click E1",             "Hard Techno",       { -4.6f, 0.455f, 0.427f, 0.724f, 1.000f, 0.452f, 0.270f, 0.000f, -1.1f, 20.0f, 0.0f, 0, 3, 33.4f, 92.5f, 1, 0.722f, 0.231f, 0, 0.000f, 0.1f, 600.0f, 0.949f }, "Clean" }, // fit 18.5 to ITHTS Rumble Kick 1.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Clang E1)
    { "Clipped Drop G#1",          "Hard Techno",       { -0.5f, 0.424f, 0.100f, 0.920f, 0.000f, 0.527f, 0.300f, 0.000f, -0.9f, 8.4f, 0.0f, 1, 1, 46.3f, 19.8f, 0, 0.964f, 0.000f, 0, 0.000f, 13.0f, 500.0f, 0.867f }, "Clean" }, // fit 26.7 to HST1 Rumble Kick Shot 15.wav (rumble tail and power refit 2026-11-12, was Clipped Rumble G#1)
    { "Wide Crushed Drop F#1",     "Hard Techno",       { -3.2f, 0.490f, 0.283f, 0.031f, 0.400f, 0.820f, 0.700f, 0.192f, 2.2f, 50.0f, 0.0f, 1, 0, 36.4f, 46.4f, 0, 0.900f, 0.000f, 0, 0.300f, 12.8f, 1000.0f, 0.000f }, "Clean" }, // fit 15.8 to BORKICK 13.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang F#1 II)
    { "Warm Rumble A1",            "Hard Techno",       { -0.5f, 0.260f, 0.031f, 0.985f, 1.000f, 0.410f, 0.248f, 0.400f, -1.8f, 0.0f, 0.0f, 1, 1, 7.1f, 92.2f, 1, 0.607f, 0.000f, 0, 0.446f, 24.0f, 500.0f, 0.214f }, "Rumble" }, // fit 18.6 to OBI Hard Rumble Kick 9.wav (rumble tail and power refit 2026-11-12)
    { "Driven Drop F1",            "Hard Techno",       { -3.9f, 0.438f, 0.765f, 0.784f, 0.600f, 0.678f, 0.800f, 0.008f, -0.9f, 0.0f, 0.0f, 0, 1, 17.9f, 75.4f, 1, 1.000f, 0.100f, 0, 0.326f, 12.6f, 600.0f, 0.434f }, "Clean" }, // fit 14.6 to BORKICK 08.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clean Click F1 III)
    { "Driven Rumble F1 II",       "Hard Techno",       { -4.1f, 0.276f, 0.508f, 0.819f, 0.215f, 0.618f, 0.265f, 0.300f, 0.5f, 31.3f, 0.0f, 0, 3, 46.3f, 71.6f, 1, 0.100f, 0.000f, 0, 0.765f, 0.1f, 1600.0f, 0.179f }, "Clean" }, // fit 18.2 to SVE2 Kick 6.2.wav (rumble tail and power refit 2026-11-12)
    { "Warm Rumble F#1 III",       "Hard Techno",       { -2.7f, 0.271f, 0.000f, 0.953f, 1.000f, 0.482f, 0.000f, 0.300f, -1.4f, 0.0f, 0.0f, 1, 0, 15.4f, 90.8f, 1, 0.700f, 0.000f, 0, 0.547f, 23.2f, 500.0f, 0.110f }, "Clean" }, // fit 16.2 to HST1 Rumble Kick Shot 11.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang F#1 III)
    { "Crushed Drop F#1",          "Hard Techno",       { -2.9f, 0.411f, 0.905f, 0.944f, 0.500f, 1.000f, 0.739f, 0.000f, 1.7f, 0.0f, 0.0f, 0, 2, 37.2f, 12.3f, 1, 0.046f, 0.144f, 0, 0.913f, 14.6f, 1600.0f, 0.682f }, "Clean" }, // fit 16.8 to HST1 Dirty Kick Shot 10.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Click F#1 II)
    { "Warm Rumble F#1 IV",        "Hard Techno",       { -3.2f, 0.281f, 0.000f, 0.524f, 0.400f, 0.446f, 1.000f, 0.500f, -3.2f, 40.0f, 0.0f, 1, 1, 42.2f, 76.1f, 2, 0.105f, 0.000f, 0, 0.800f, 0.5f, 1600.0f, 0.200f }, "Clean" }, // fit 19.0 to HST1 Rumble Kick Shot 1.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Click F#1 III)
    { "Warm Drop F1",              "Hard Techno",       { -4.5f, 0.377f, 0.671f, 0.400f, 0.057f, 0.300f, 1.000f, 0.225f, -0.2f, 0.0f, 0.0f, 1, 2, 34.2f, 38.6f, 2, 0.311f, 0.024f, 0, 1.000f, 8.4f, 500.0f }, "Clean" }, // fit 14.7 to SVEKick02C.wav (kick register)
    // --- Schranz (100) ---
    { "Driven Clang A#1 II",       "Schranz",           { 0.8f, 0.347f, 0.721f, 1.000f, 0.922f, 0.553f, 0.858f, 0.000f, -4.7f, 20.0f, 3.0f, 0, 1, 38.3f, 48.1f, 2, 1.000f, 0.267f, 0, 0.567f, 0.0f, 250.0f }, "Short" }, // fit 13.3 to SSP Signature Schranz Kick 52.wav (refit with tonality 2026-10-23, was Warm Clang A#1 III)
    { "Clipped Clang G#1",         "Schranz",           { -0.8f, 0.372f, 0.425f, 0.482f, 0.649f, 0.700f, 0.980f, 0.100f, -4.2f, 0.0f, 0.0f, 1, 1, 43.8f, 43.9f, 2, 0.478f, 0.002f, 0, 1.000f, 3.7f, 500.0f }, "Driven" }, // fit 12.8 to ARK Kick Drum Shot 15.wav (kick register)
    { "Crushed Clang G#1 II",      "Schranz",           { -0.6f, 0.297f, 0.588f, 0.700f, 0.900f, 0.900f, 0.700f, 0.045f, -3.6f, 0.0f, 0.0f, 1, 1, 48.0f, 36.7f, 2, 0.765f, 0.018f, 0, 0.700f, 3.3f, 500.0f }, "Short" }, // fit 11.7 to ARK Kick Drum Shot 5.wav (kick register)
    { "Warm Clang F#1 V",          "Schranz",           { -2.8f, 0.322f, 0.132f, 0.436f, 0.700f, 0.382f, 0.200f, 0.000f, -1.3f, 0.0f, 0.0f, 1, 3, 14.2f, 51.2f, 2, 0.885f, 0.000f, 0, 1.000f, 22.0f, 1000.0f, 0.488f }, "Short" }, // fit 18.4 to ARK Kick Drum Shot 23.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Clang F#1 III)
    { "Crushed Clang F#1 II",      "Schranz",           { -3.0f, 0.254f, 0.880f, 1.000f, 0.816f, 0.954f, 0.000f, 0.000f, -2.1f, 0.0f, 0.0f, 0, 2, 23.8f, 51.0f, 2, 0.418f, 0.521f, 0, 0.000f, 6.9f, 500.0f }, "Short" }, // fit 9.4 to ARK Kick Drum Shot 3.wav (kick register)
    { "Clipped Clang G1 VI",       "Schranz",           { -1.6f, 0.474f, 0.659f, 0.226f, 0.982f, 0.571f, 0.840f, 0.000f, -4.1f, 0.0f, 0.0f, 1, 1, 48.0f, 36.6f, 2, 0.443f, 0.003f, 0, 0.639f, 6.2f, 500.0f, 0.149f }, "Long" }, // fit 17.1 to ARK Kick Drum Shot 11.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Crushed Thump G1)
    { "Classic Schranz",           "Schranz",           { -2.9f, 0.442f, 0.387f, 0.448f, 0.633f, 0.700f, 0.900f, 0.000f, -2.8f, 0.0f, 0.0f, 1, 1, 45.1f, 52.7f, 2, 0.536f, 0.045f, 0, 0.819f, 1.8f, 500.0f }, "Driven" }, // fit 11.5 to ARK Kick Drum Shot 19.wav (kick register)
    { "Clipped Punch C2",          "Schranz",           { 2.5f, 0.428f, 0.972f, 0.700f, 0.300f, 0.739f, 0.700f, 0.000f, -2.2f, 0.9f, 0.0f, 1, 1, 34.7f, 24.2f, 2, 1.000f, 0.447f, 0, 0.275f, 4.8f, 1000.0f, 0.034f }, "Crushed" }, // fit 14.4 to ARK Kick Drum Shot 22.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Crushed Clang C2)
    { "Clipped Rumble A#1 IV",     "Schranz",           { 0.5f, 0.361f, 1.000f, 0.687f, 0.600f, 0.596f, 0.314f, 0.985f, -3.9f, 0.0f, 0.0f, 1, 3, 33.7f, 35.6f, 0, 0.100f, 0.000f, 0, 0.300f, 9.0f, 500.0f, 0.264f }, "Rumble" }, // fit 17.9 to OBI Driven Schranz Kick 1.wav (rumble tail and power refit 2026-11-12)
    { "Crushed Thump B1 II",       "Schranz",           { 1.9f, 0.356f, 0.406f, 0.497f, 0.544f, 0.775f, 0.894f, 0.083f, -3.3f, 0.0f, 0.0f, 1, 1, 40.4f, 32.4f, 2, 0.880f, 0.000f, 0, 0.718f, 0.9f, 500.0f }, "Crushed" }, // fit 15.8 to ARK Kick Drum Shot 8.wav (kick register)
    { "Driven Clang B1 IV",        "Schranz",           { 2.4f, 0.430f, 0.558f, 0.600f, 0.657f, 0.671f, 0.700f, 0.000f, -3.0f, 0.0f, 0.0f, 0, 1, 36.8f, 41.9f, 2, 0.939f, 0.188f, 0, 0.619f, 0.9f, 1000.0f }, "Driven" }, // fit 13.1 to ARK Kick Drum Shot 16.wav (kick register)
    { "Schranz Air",               "Schranz",           { 0.5f, 0.432f, 0.739f, 0.581f, 0.830f, 0.573f, 0.285f, 0.000f, -1.1f, 60.0f, 0.0f, 0, 0, 30.0f, 65.4f, 2, 0.570f, 0.866f, 0, 0.000f, 2.0f, 1000.0f, 0.341f }, "Wide" }, // fit 15.7 to SSP Signature Schranz Kick 38.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Warm Drop A#1",             "Schranz",           { 1.5f, 0.414f, 0.802f, 0.977f, 0.600f, 0.383f, 0.931f, 0.000f, -4.1f, 40.0f, 0.0f, 1, 1, 41.0f, 16.6f, 1, 0.000f, 0.239f, 0, 0.352f, 15.1f, 1600.0f, 0.187f }, "Wide" }, // fit 13.5 to SSP Signature Schranz Kick 54.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Wide Crushed Clang B1",     "Schranz",           { 2.5f, 0.415f, 0.801f, 0.676f, 0.979f, 0.966f, 0.041f, 0.000f, -5.6f, 91.0f, 6.0f, 0, 1, 28.4f, 62.9f, 2, 0.224f, 0.611f, 0, 0.762f, 2.2f, 1000.0f, 0.417f }, "Driven" }, // fit 21.9 to SSP Signature Schranz Kick 53.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Wide Driven Clang A1",      "Schranz",           { -0.3f, 0.411f, 0.671f, 0.889f, 1.000f, 0.530f, 0.146f, 0.000f, -4.4f, 90.0f, 0.0f, 0, 3, 14.1f, 76.5f, 2, 0.515f, 0.000f, 0, 0.200f, 12.0f, 1000.0f, 0.033f }, "Wide" }, // fit 18.6 to SSP Signature Schranz Kick 79.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Driven Click A1",           "Schranz",           { 0.2f, 0.311f, 0.992f, 0.856f, 0.900f, 0.588f, 0.186f, 0.200f, -2.0f, 40.0f, 0.0f, 0, 3, 37.4f, 41.4f, 0, 0.423f, 0.000f, 0, 0.100f, 8.5f, 1000.0f, 0.287f }, "Clean" }, // fit 17.5 to SSP Signature Schranz Kick 11.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Driven Clang A1 II)
    { "Clean Clang A#1",           "Schranz",           { 1.4f, 0.415f, 0.949f, 0.793f, 0.995f, 0.226f, 0.951f, 0.000f, -5.1f, 18.9f, 0.0f, 1, 3, 44.5f, 34.6f, 2, 0.083f, 0.904f, 0, 0.172f, 6.2f, 1600.0f, 0.943f }, "Wide" }, // fit 20.4 to SSP Signature Schranz Kick 26.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Clang A#1 IV)
    { "Wide Grinding Knock F1",    "Schranz",           { -3.5f, 0.275f, 0.000f, 0.692f, 0.000f, 0.637f, 0.100f, 0.294f, 0.9f, 60.0f, 0.0f, 2, 2, 24.4f, 86.0f, 1, 0.071f, 0.000f, 0, 0.074f, 24.0f, 1000.0f, 0.176f }, "Wide" }, // fit 17.1 to SSP Signature Schranz Kick 20.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Driven Clang F1)
    { "Driven Knock G1",           "Schranz",           { -1.7f, 0.292f, 0.278f, 0.700f, 0.200f, 0.658f, 0.426f, 0.288f, -3.2f, 39.4f, 6.0f, 0, 4, 47.6f, 59.7f, 2, 0.478f, 0.000f, 0, 0.014f, 7.0f, 1000.0f, 0.700f }, "Wide" }, // fit 16.6 to SSP Signature Schranz Kick 49.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Warm Clang G1)
    { "Driven Drop A1",            "Schranz",           { 0.5f, 0.446f, 0.449f, 0.474f, 0.200f, 0.635f, 0.420f, 0.000f, 1.1f, 20.0f, 0.0f, 0, 0, 35.4f, 29.7f, 2, 0.988f, 0.829f, 0, 0.489f, 7.7f, 1000.0f, 0.100f }, "Clean" }, // fit 17.1 to SSP Signature Schranz Kick 16.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang A1)
    { "Wide Driven Clang F1 II",   "Schranz",           { -4.3f, 0.428f, 0.451f, 0.700f, 0.800f, 0.746f, 0.800f, 0.000f, -5.9f, 100.0f, 6.0f, 0, 1, 30.7f, 32.4f, 2, 0.000f, 0.753f, 0, 0.300f, 22.7f, 1000.0f, 0.379f }, "Wide" }, // fit 18.4 to SSP Signature Schranz Kick 12.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Warm Clang F1)
    { "Wide Clipped Clang B1 II",  "Schranz",           { 1.7f, 0.415f, 0.845f, 0.950f, 0.823f, 0.578f, 0.000f, 0.000f, -6.1f, 60.0f, 0.0f, 1, 3, 15.7f, 47.6f, 2, 0.948f, 0.048f, 0, 0.000f, 11.2f, 1000.0f, 0.016f }, "Clean" }, // fit 19.8 to MS Signature Kick Shot 27.wav (duplicate split 2026-11-12)
    { "Wide Warm Boom B1",         "Schranz",           { 1.7f, 0.412f, 0.660f, 0.956f, 0.400f, 0.415f, 0.985f, 0.000f, -3.7f, 50.0f, 0.0f, 1, 1, 42.0f, 44.5f, 2, 0.789f, 0.138f, 0, 0.195f, 2.7f, 400.0f }, "Wide" }, // fit 16.0 to SSP Signature Schranz Kick 83.wav (refit with tonality 2026-10-23, was Wide Clean Clang B1)
    { "Wide Driven Clang C2 II",   "Schranz",           { 2.5f, 0.412f, 0.731f, 0.900f, 0.900f, 0.700f, 0.700f, 0.000f, -5.4f, 80.0f, 0.0f, 0, 1, 30.7f, 61.3f, 2, 0.861f, 0.204f, 0, 0.100f, 0.6f, 1000.0f }, "Wide" }, // fit 13.2 to SSP Signature Schranz Kick 62.wav (refit with tonality 2026-10-23, was Wide Warm Clang C2)
    { "Schranz Metal",             "Schranz",           { 2.4f, 0.413f, 0.968f, 1.000f, 0.700f, 0.340f, 0.900f, 0.000f, -3.7f, 20.0f, 0.0f, 1, 1, 31.3f, 40.6f, 2, 0.600f, 0.350f, 0, 0.200f, 4.0f, 1000.0f }, "Clean" }, // fit 11.5 to MS Signature Kick Shot 30.wav (kick register)
    { "Schranz Pressure",          "Schranz",           { -2.4f, 0.396f, 0.604f, 0.527f, 0.300f, 0.737f, 0.235f, 0.200f, 2.4f, 20.0f, 1.0f, 2, 0, 42.1f, 68.4f, 0, 0.600f, 0.000f, 0, 0.500f, 0.4f, 1000.0f, 0.074f }, "Driven" }, // fit 20.4 to SSP Signature Schranz Kick 92.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Wide Hammer",               "Schranz",           { 1.7f, 0.422f, 0.697f, 1.000f, 1.000f, 0.700f, 0.300f, 0.000f, -4.7f, 23.0f, 6.0f, 1, 1, 35.9f, 54.7f, 2, 0.612f, 0.745f, 0, 0.100f, 3.0f, 600.0f }, "Driven" }, // fit 9.1 to ITHTS Signature Schranz Kick 4.wav (kick register)
    { "Driven Boom A1",            "Schranz",           { -0.4f, 0.422f, 0.393f, 0.997f, 0.000f, 0.595f, 0.400f, 0.000f, 0.2f, 30.0f, 0.0f, 0, 0, 33.4f, 66.1f, 0, 0.484f, 0.570f, 0, 0.249f, 5.5f, 1000.0f, 0.000f }, "Driven" }, // fit 19.5 to SSP Signature Schranz Kick 91.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Clang A1 IV)
    { "Wide Driven Clang C2",      "Schranz",           { 3.0f, 0.425f, 0.603f, 0.836f, 0.697f, 0.600f, 0.700f, 0.000f, -3.5f, 50.0f, 0.0f, 0, 1, 36.6f, 41.8f, 2, 0.956f, 0.135f, 0, 0.172f, 1.3f, 1000.0f }, "Wide" }, // fit 13.5 to SSP Signature Schranz Kick 57.wav (kick register)
    { "Crushed Clang C2 II",       "Schranz",           { 2.9f, 0.417f, 0.876f, 0.900f, 0.985f, 0.800f, 0.100f, 0.000f, -4.5f, 21.8f, 6.0f, 1, 1, 26.2f, 48.1f, 2, 0.279f, 0.390f, 0, 0.600f, 3.6f, 1000.0f }, "Crushed" }, // fit 12.3 to MS Signature Kick Shot 37.wav (refit with tonality 2026-10-23)
    { "Warm Click G#1",            "Schranz",           { -0.5f, 0.416f, 0.709f, 1.000f, 1.000f, 0.337f, 0.800f, 0.000f, -3.4f, 10.0f, 0.0f, 1, 1, 33.0f, 58.8f, 1, 0.100f, 0.139f, 0, 0.186f, 8.0f, 500.0f }, "Clean" }, // fit 11.5 to MS Signature Kick Shot 15.wav (refit with tonality 2026-10-23, was Warm Drop G#1)
    { "Crushed Thump B1",          "Schranz",           { 1.9f, 0.362f, 0.406f, 0.525f, 0.587f, 0.805f, 0.800f, 0.050f, -3.6f, 0.0f, 0.0f, 1, 1, 44.1f, 33.1f, 2, 0.823f, 0.000f, 0, 0.700f, 0.6f, 500.0f }, "Crushed" }, // fit 11.8 to ARK Kick Drum Shot 2.wav (kick register)
    { "Crushed Clang B1 III",      "Schranz",           { 2.5f, 0.419f, 0.420f, 0.800f, 0.783f, 0.773f, 0.500f, 0.000f, -3.2f, 10.0f, 0.0f, 0, 1, 42.6f, 56.9f, 2, 0.967f, 0.152f, 0, 0.400f, 1.2f, 1000.0f }, "Clean" }, // fit 12.2 to MS Signature Kick Shot 39.wav (refit with tonality 2026-10-23, was Warm Clang B1 XII)
    { "Wide Grinding Rumble F#1",  "Schranz",           { -3.0f, 0.371f, 0.104f, 0.354f, 1.000f, 0.764f, 0.900f, 0.500f, 1.9f, 50.0f, 0.0f, 2, 0, 29.1f, 15.6f, 0, 0.100f, 0.000f, 0, 0.500f, 23.5f, 1000.0f, 1.000f }, "Clean" }, // fit 41.8 to SSP Signature Schranz Kick 72.wav (rumble tail and power refit 2026-11-12, was Grinding Rumble F#1)
    { "Clipped Clang G#1 II",      "Schranz",           { -1.0f, 0.432f, 0.057f, 0.800f, 1.000f, 0.518f, 0.100f, 0.000f, -2.5f, 0.0f, 0.0f, 1, 0, 41.1f, 33.7f, 2, 0.350f, 0.390f, 0, 0.760f, 13.7f, 500.0f, 0.288f }, "Driven" }, // fit 16.7 to ITHTS Signature Schranz Kick 13.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Warm Clang A1 V",           "Schranz",           { 0.4f, 0.412f, 0.957f, 1.000f, 0.600f, 0.351f, 0.899f, 0.000f, -3.1f, 24.8f, 0.0f, 1, 1, 25.7f, 63.9f, 2, 1.000f, 0.000f, 0, 0.403f, 8.1f, 500.0f }, "Clean" }, // fit 12.7 to MS Signature Kick Shot 44.wav (kick register)
    { "Warm Clang B1 13",          "Schranz",           { 2.1f, 0.412f, 0.796f, 1.000f, 0.700f, 0.400f, 0.900f, 0.000f, -3.9f, 17.3f, 0.0f, 1, 1, 35.5f, 32.0f, 2, 0.946f, 0.285f, 0, 0.300f, 12.5f, 1600.0f }, "Clean" }, // fit 11.8 to MS Signature Kick Shot 6.wav (kick register)
    { "Wide Clipped Clang B1",     "Schranz",           { 1.7f, 0.415f, 0.897f, 0.935f, 0.900f, 0.600f, 0.100f, 0.000f, -5.5f, 50.0f, 0.0f, 1, 1, 25.8f, 30.3f, 2, 0.995f, 0.000f, 0, 0.014f, 11.3f, 1000.0f }, "Wide" }, // fit 14.4 to SSP Signature Schranz Kick 82.wav (kick register)
    { "Clipped Drop D1",           "Schranz",           { -6.9f, 0.399f, 0.168f, 0.548f, 0.500f, 0.563f, 0.605f, 0.125f, -2.6f, 0.0f, 0.0f, 1, 1, 43.4f, 56.0f, 2, 0.800f, 0.600f, 0, 0.629f, 18.7f, 500.0f, 0.022f }, "Driven" }, // fit 28.7 to SSP Signature Schranz Kick 80.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Clang D1)
    { "Crushed Clang F1 II",       "Schranz",           { -3.6f, 0.310f, 0.600f, 0.850f, 0.600f, 0.968f, 0.000f, 0.100f, -0.9f, 0.0f, 0.0f, 1, 3, 41.2f, 38.8f, 2, 0.100f, 0.000f, 0, 0.400f, 24.0f, 1000.0f, 0.300f }, "Clean" }, // fit 23.2 to ARK Kick Drum Shot 24.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang F1)
    { "Clipped Clang G1 II",       "Schranz",           { -2.4f, 0.390f, 0.859f, 0.214f, 0.700f, 0.742f, 0.600f, 0.032f, -3.7f, 0.0f, 0.0f, 1, 1, 22.3f, 30.1f, 2, 0.189f, 0.012f, 0, 1.000f, 7.5f, 500.0f }, "Driven" }, // fit 12.5 to ARK Kick Drum Shot 12.wav (kick register)
    { "Driven Click G1 V",         "Schranz",           { -1.7f, 0.419f, 1.000f, 0.758f, 1.000f, 0.664f, 0.422f, 0.000f, 1.2f, 3.2f, 0.0f, 0, 2, 18.0f, 71.2f, 0, 0.900f, 0.260f, 0, 0.258f, 13.6f, 1000.0f, 0.568f }, "Clean" }, // fit 19.0 to MS Signature Kick Shot 29.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Click G1 IV)
    { "Clipped Clang C2",          "Schranz",           { 2.9f, 0.422f, 0.612f, 0.700f, 0.600f, 0.679f, 0.800f, 0.000f, -2.1f, 20.0f, 0.0f, 1, 1, 40.0f, 36.2f, 2, 0.890f, 0.224f, 0, 0.464f, 0.7f, 1000.0f }, "Driven" }, // fit 15.5 to MS Signature Kick Shot 47.wav (kick register)
    { "Crushed Click G1 II",       "Schranz",           { -1.6f, 0.403f, 0.095f, 0.348f, 1.000f, 1.000f, 0.600f, 0.103f, 0.7f, 30.0f, 0.0f, 0, 0, 43.0f, 45.7f, 1, 0.927f, 0.000f, 0, 0.600f, 10.5f, 500.0f, 0.016f }, "Driven" }, // fit 18.2 to SSP Signature Schranz Kick 73.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Clang G1)
    { "Driven Clang C2 V",         "Schranz",           { 2.7f, 0.417f, 1.000f, 0.971f, 0.979f, 0.581f, 0.400f, 0.000f, -3.4f, 28.2f, 0.0f, 0, 1, 34.9f, 40.4f, 2, 0.712f, 0.130f, 0, 0.000f, 0.3f, 1000.0f }, "Driven" }, // fit 18.1 to MS Signature Kick Shot 11.wav (kick register)
    { "Driven Clang C2 VI",        "Schranz",           { 3.0f, 0.419f, 0.617f, 0.943f, 0.926f, 0.586f, 0.781f, 0.000f, -2.6f, 20.0f, 0.0f, 0, 1, 40.7f, 44.8f, 2, 0.715f, 1.000f, 0, 0.200f, 2.4f, 1000.0f }, "Driven" }, // fit 12.8 to SSP Signature Schranz Kick 86.wav (kick register)
    { "Warm Clang B1 14",          "Schranz",           { 2.3f, 0.417f, 0.793f, 0.905f, 0.900f, 0.486f, 0.500f, 0.000f, -3.8f, 0.5f, 0.0f, 1, 1, 31.4f, 59.1f, 2, 0.800f, 0.213f, 0, 0.009f, 1.1f, 1000.0f }, "Clean" }, // fit 10.1 to MS Signature Kick Shot 5.wav (refit with tonality 2026-10-23)
    { "Crushed Clang C2 III",      "Schranz",           { 2.6f, 0.419f, 0.410f, 0.700f, 0.800f, 0.800f, 0.500f, 0.000f, -3.2f, 40.0f, 0.0f, 0, 1, 37.6f, 48.9f, 2, 0.966f, 0.032f, 0, 0.500f, 2.0f, 1000.0f }, "Crushed" }, // fit 12.3 to SSP Signature Schranz Kick 89.wav (kick register)
    { "Clipped Clang B1 II",       "Schranz",           { 1.9f, 0.417f, 0.991f, 0.996f, 0.963f, 0.563f, 0.298f, 0.000f, -4.1f, 20.0f, 0.0f, 1, 1, 26.6f, 59.7f, 2, 0.697f, 0.128f, 0, 0.235f, 3.1f, 1000.0f }, "Clean" }, // fit 11.8 to MS Signature Kick Shot 50.wav (refit with tonality 2026-10-23, was Warm Clang B1)
    { "Warm Clang A#1 VIII",       "Schranz",           { 1.0f, 0.412f, 0.587f, 1.000f, 0.800f, 0.307f, 1.000f, 0.003f, -4.6f, 30.0f, 0.0f, 1, 1, 36.1f, 29.8f, 2, 0.938f, 0.800f, 0, 0.552f, 15.9f, 1600.0f }, "Clean" }, // fit 13.5 to MS Signature Kick Shot 7.wav (refit with tonality 2026-10-23)
    { "Clipped Click A#1",         "Schranz",           { 1.5f, 0.414f, 0.757f, 0.746f, 0.861f, 0.730f, 0.377f, 0.000f, -1.5f, 13.8f, 0.0f, 1, 1, 42.1f, 36.5f, 0, 0.937f, 0.058f, 0, 0.321f, 1.6f, 1000.0f, 0.005f }, "Driven" }, // fit 21.7 to MS Signature Kick Shot 49.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Clang A#1)
    { "Driven Clang B1 III",       "Schranz",           { 2.0f, 0.415f, 0.784f, 1.000f, 0.973f, 0.641f, 0.300f, 0.000f, -2.7f, 30.0f, 0.0f, 0, 1, 26.0f, 60.9f, 2, 0.517f, 0.271f, 0, 0.000f, 2.8f, 1000.0f }, "Driven" }, // fit 13.1 to SSP Signature Schranz Kick 70.wav (kick register)
    { "Warm Drop B1",              "Schranz",           { 2.1f, 0.413f, 0.914f, 1.000f, 0.300f, 0.340f, 1.000f, 0.000f, -3.7f, 19.4f, 0.0f, 1, 1, 16.5f, 30.9f, 2, 0.949f, 0.112f, 0, 0.200f, 14.7f, 500.0f }, "Clean" }, // fit 12.4 to MS Signature Kick Shot 26.wav (kick register)
    { "Clipped Rumble G#1 II",     "Schranz",           { -0.7f, 0.299f, 0.272f, 0.545f, 0.912f, 0.638f, 0.352f, 0.311f, -4.3f, 12.7f, 0.0f, 1, 1, 41.8f, 60.8f, 2, 0.036f, 0.000f, 0, 0.629f, 3.3f, 500.0f, 0.021f }, "Clean" }, // fit 38.2 to SSP Signature Schranz Kick 66.wav (rumble tail and power refit 2026-11-12)
    { "Driven Click A#1",          "Schranz",           { 1.4f, 0.419f, 0.875f, 0.700f, 0.840f, 0.727f, 0.601f, 0.000f, -1.8f, 9.4f, 6.0f, 0, 1, 40.9f, 32.7f, 0, 0.295f, 0.409f, 0, 0.500f, 6.7f, 1000.0f, 0.216f }, "Clean" }, // fit 18.1 to SSP Signature Schranz Kick 56.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Clang A#1 V)
    { "Driven Clang C2 III",       "Schranz",           { 2.9f, 0.417f, 0.977f, 0.685f, 0.825f, 0.602f, 0.600f, 0.000f, -3.7f, 0.0f, 0.0f, 0, 1, 31.3f, 40.3f, 2, 0.916f, 0.000f, 0, 0.039f, 1.9f, 1000.0f }, "Driven" }, // fit 11.9 to MS Signature Kick Shot 35.wav (kick register)
    { "Driven Clang A1",           "Schranz",           { -0.4f, 0.412f, 0.468f, 0.786f, 0.956f, 0.700f, 0.460f, 0.000f, -2.9f, 30.0f, 0.0f, 0, 1, 41.4f, 65.9f, 2, 0.400f, 0.827f, 0, 0.100f, 0.0f, 1000.0f }, "Driven" }, // fit 11.1 to MS Signature Kick Shot 45.wav (kick register)
    { "Driven Click F1 III",       "Schranz",           { -3.7f, 0.362f, 0.460f, 0.984f, 1.000f, 0.526f, 0.252f, 0.000f, -0.9f, 20.0f, 0.0f, 0, 1, 47.9f, 62.9f, 1, 1.000f, 0.547f, 0, 0.948f, 5.8f, 1000.0f, 0.024f }, "Clean" }, // fit 26.3 to SSP Signature Schranz Kick 15.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang F1 V)
    { "Driven Clang G#1 II",       "Schranz",           { -0.6f, 0.344f, 0.988f, 0.427f, 1.000f, 0.550f, 0.874f, 0.229f, -3.8f, 30.0f, 0.0f, 0, 3, 47.8f, 32.6f, 2, 0.934f, 0.000f, 0, 0.000f, 6.3f, 500.0f, 0.633f }, "Clean" }, // fit 38.1 to SSP Signature Schranz Kick 25.wav (rumble tail and power refit 2026-11-12, was Driven Rumble G#1)
    { "Driven Click A1 II",        "Schranz",           { 0.2f, 0.415f, 0.254f, 0.975f, 0.800f, 0.594f, 0.200f, 0.000f, -1.5f, 10.0f, 0.0f, 0, 1, 47.1f, 45.4f, 1, 0.093f, 0.661f, 0, 0.000f, 7.9f, 1000.0f, 0.113f }, "Driven" }, // fit 25.2 to MS Signature Kick Shot 33.wav (rumble tail and power refit 2026-11-12, was Clipped Rumble A1 III)
    { "Driven Clang B1",           "Schranz",           { 2.2f, 0.412f, 0.400f, 1.000f, 0.800f, 0.592f, 0.900f, 0.000f, -2.5f, 20.0f, 0.0f, 0, 1, 36.2f, 37.1f, 2, 0.828f, 0.992f, 0, 0.000f, 8.7f, 1600.0f }, "Driven" }, // fit 13.7 to MS Signature Kick Shot 34.wav (kick register)
    { "Schranz Wide Tick",         "Schranz",           { 2.0f, 0.409f, 0.745f, 0.900f, 0.900f, 0.644f, 0.691f, 0.000f, -3.7f, 30.0f, 0.0f, 0, 1, 48.0f, 26.7f, 2, 0.905f, 0.093f, 0, 0.413f, 3.7f, 1000.0f }, "Driven" }, // fit 12.6 to SSP Signature Schranz Kick 90.wav (kick register)
    { "Clipped Clang B1 III",      "Schranz",           { 1.9f, 0.415f, 0.715f, 0.929f, 1.000f, 0.616f, 0.400f, 0.002f, -5.2f, 10.0f, 6.0f, 1, 1, 35.7f, 57.9f, 2, 0.568f, 0.100f, 0, 0.100f, 0.0f, 600.0f, 0.143f }, "Clean" }, // fit 10.8 to MS Signature Kick Shot 25.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang B1 15)
    { "Clipped Clang A#1",         "Schranz",           { 1.4f, 0.420f, 0.571f, 0.790f, 0.724f, 0.567f, 0.800f, 0.000f, -2.7f, 15.0f, 0.0f, 1, 1, 38.0f, 49.3f, 2, 0.967f, 0.125f, 0, 0.427f, 1.5f, 1000.0f, 0.027f }, "Clean" }, // fit 16.7 to MS Signature Kick Shot 16.wav (duplicate split 2026-11-12)
    { "Driven Clang F1 II",        "Schranz",           { -3.7f, 0.554f, 0.644f, 0.430f, 0.615f, 0.500f, 0.501f, 0.000f, -1.2f, 0.9f, 0.0f, 0, 3, 24.8f, 13.6f, 2, 0.748f, 0.016f, 0, 0.178f, 15.0f, 500.0f, 0.867f }, "Clean" }, // fit 28.1 to OBI Driven Schranz Kick 4.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Crushed Click F1",          "Schranz",           { -3.6f, 0.416f, 0.352f, 0.731f, 0.975f, 0.776f, 0.296f, 0.000f, 0.2f, 0.2f, 0.0f, 0, 2, 23.0f, 33.6f, 0, 0.679f, 0.770f, 0, 0.000f, 15.8f, 1600.0f, 0.288f }, "Clean" }, // fit 22.8 to ARK Kick Drum Shot 25.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Drop F1 II)
    { "Driven Clang G#1 IV",       "Schranz",           { -1.0f, 0.430f, 0.789f, 0.700f, 0.900f, 0.575f, 0.028f, 0.000f, -2.9f, 20.0f, 0.0f, 0, 3, 23.4f, 44.7f, 2, 0.892f, 0.155f, 0, 0.200f, 14.2f, 1000.0f }, "Clean" }, // fit 12.9 to SSP Signature Schranz Kick 40.wav (refit with tonality 2026-10-23, was Warm Boom G#1)
    { "Crushed Click A1",          "Schranz",           { -0.2f, 0.419f, 0.302f, 0.900f, 1.000f, 0.790f, 0.461f, 0.000f, -0.6f, 20.0f, 0.0f, 0, 1, 48.0f, 36.5f, 1, 0.000f, 0.634f, 0, 0.027f, 10.1f, 1000.0f, 0.106f }, "Clean" }, // fit 26.6 to SSP Signature Schranz Kick 34.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Clang A1 V)
    { "Warm Clang A#1",            "Schranz",           { 0.5f, 0.421f, 0.990f, 0.884f, 1.000f, 0.350f, 0.528f, 0.000f, -4.1f, 20.0f, 0.0f, 1, 3, 46.3f, 31.0f, 2, 0.900f, 0.000f, 0, 0.000f, 12.1f, 1000.0f, 0.000f }, "Clean" }, // fit 22.2 to SSP Signature Schranz Kick 26.wav (rumble tail and power refit 2026-11-12, was Warm Rumble A#1 III)
    { "Clipped Clang A#1 II",      "Schranz",           { 0.5f, 0.429f, 0.769f, 0.637f, 1.000f, 0.549f, 0.088f, 0.000f, -4.3f, 0.0f, 0.0f, 1, 3, 25.9f, 25.6f, 2, 0.758f, 0.008f, 0, 0.386f, 14.2f, 1000.0f, 0.200f }, "Clean" }, // fit 21.4 to MS Signature Kick Shot 9.wav (rumble tail and power refit 2026-11-12)
    { "Driven Rumble G#1 II",      "Schranz",           { -0.8f, 0.217f, 0.186f, 0.812f, 0.969f, 0.734f, 0.081f, 0.633f, -2.9f, 40.0f, 0.0f, 0, 1, 35.1f, 37.1f, 2, 0.297f, 0.000f, 0, 0.300f, 14.7f, 1000.0f, 1.000f }, "Clean" }, // fit 48.4 to SSP Signature Schranz Kick 30.wav (rumble tail and power refit 2026-11-12)
    { "Driven Clang A1 III",       "Schranz",           { 0.3f, 0.256f, 0.939f, 0.698f, 0.952f, 0.625f, 0.392f, 0.251f, -2.9f, 20.0f, 6.0f, 0, 3, 26.4f, 30.1f, 2, 0.000f, 0.000f, 0, 0.675f, 15.5f, 1000.0f, 0.200f }, "Driven" }, // fit 22.0 to SSP Signature Schranz Kick 31.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Driven Clang B1 II",        "Schranz",           { 1.5f, 0.420f, 0.469f, 0.804f, 0.733f, 0.619f, 0.568f, 0.000f, -3.5f, 30.0f, 0.0f, 0, 1, 38.3f, 43.0f, 2, 0.909f, 0.182f, 0, 0.393f, 1.2f, 1000.0f }, "Driven" }, // fit 10.7 to MS Signature Kick Shot 28.wav (kick register)
    { "Wide Clipped Clang B1 III", "Schranz",           { 1.5f, 0.416f, 0.700f, 0.900f, 0.900f, 0.700f, 0.400f, 0.000f, -4.8f, 50.0f, 0.0f, 1, 1, 34.7f, 57.5f, 2, 0.831f, 0.339f, 0, 0.188f, 1.2f, 1000.0f }, "Wide" }, // fit 15.5 to SSP Signature Schranz Kick 69.wav (refit with tonality 2026-10-23, was Wide Clean Clang B1 II)
    { "Warm Clang A#1 II",         "Schranz",           { 1.3f, 0.421f, 0.928f, 1.000f, 1.000f, 0.412f, 1.000f, 0.000f, -3.7f, 28.8f, 0.0f, 0, 1, 40.9f, 38.9f, 2, 0.564f, 0.976f, 0, 0.600f, 5.3f, 1600.0f, 0.173f }, "Clean" }, // fit 15.7 to SSP Signature Schranz Kick 43.wav (power refit 2026-11-12: WEIGHT, low end, attack)
    { "Wide Driven Clang A#1 II",  "Schranz",           { 0.6f, 0.419f, 0.312f, 0.789f, 1.000f, 0.673f, 0.000f, 0.000f, -4.0f, 50.0f, 0.0f, 0, 0, 39.0f, 44.5f, 2, 0.892f, 0.457f, 0, 0.330f, 9.0f, 500.0f, 0.500f }, "Wide" }, // fit 24.9 to SSP Signature Schranz Kick 95.wav (rumble tail and power refit 2026-11-12, was Wide Grinding Rumble A#1)
    { "Crushed Boom A1",           "Schranz",           { -0.2f, 0.411f, 0.186f, 0.950f, 0.749f, 0.800f, 0.580f, 0.000f, -0.3f, 29.6f, 0.0f, 0, 1, 48.0f, 46.6f, 1, 1.000f, 0.081f, 0, 0.600f, 4.6f, 1600.0f, 0.658f }, "Clean" }, // fit 24.8 to MS Signature Kick Shot 20.wav (rumble tail and power refit 2026-11-12, was Driven Rumble A1)
    { "Wide Warm Clang A#1 III",   "Schranz",           { 1.4f, 0.421f, 0.760f, 0.748f, 0.986f, 0.434f, 0.450f, 0.000f, -5.2f, 60.0f, 0.0f, 1, 1, 27.6f, 55.4f, 2, 0.767f, 0.606f, 0, 0.300f, 5.5f, 1000.0f }, "Clean" }, // fit 13.9 to SSP Signature Schranz Kick 54.wav (refit with tonality 2026-10-23, was Clean Boom A#1)
    { "Clipped Clang B1 V",        "Schranz",           { 2.2f, 0.428f, 0.698f, 0.591f, 1.000f, 0.687f, 0.100f, 0.001f, -4.1f, 10.0f, 6.0f, 1, 1, 28.1f, 47.5f, 2, 0.316f, 0.400f, 0, 0.564f, 4.7f, 1000.0f, 0.100f }, "Clean" }, // fit 13.2 to MS Signature Kick Shot 16.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang B1 III)
    { "Warm Click B1 II",          "Schranz",           { 1.8f, 0.425f, 0.888f, 1.000f, 0.800f, 0.480f, 1.000f, 0.002f, -4.1f, 20.0f, 0.0f, 1, 1, 47.2f, 33.4f, 0, 0.420f, 1.000f, 0, 0.400f, 5.3f, 1600.0f, 0.000f }, "Clean" }, // fit 19.8 to SSP Signature Schranz Kick 14.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Driven Click B1)
    { "Driven Clang B1 V",         "Schranz",           { 2.1f, 0.425f, 0.831f, 0.810f, 0.800f, 0.513f, 1.000f, 0.000f, -4.1f, 21.4f, 3.0f, 0, 1, 39.6f, 40.4f, 2, 1.000f, 0.976f, 0, 0.196f, 0.8f, 1000.0f }, "Clean" }, // fit 14.7 to SSP Signature Schranz Kick 40.wav (refit with tonality 2026-10-23, was Warm Clang B1 IX)
    { "Warm Clang B1",             "Schranz",           { 2.3f, 0.428f, 0.924f, 0.780f, 0.600f, 0.477f, 0.054f, 0.000f, -1.8f, 30.0f, 0.0f, 0, 1, 43.3f, 34.5f, 2, 0.905f, 0.199f, 0, 0.565f, 5.5f, 1000.0f, 0.000f }, "Clean" }, // fit 21.7 to SSP Signature Schranz Kick 33.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Drop B1 II)
    { "Clipped Click G#1 III",     "Schranz",           { -0.7f, 0.368f, 0.700f, 0.342f, 1.000f, 0.578f, 0.604f, 0.200f, -3.4f, 10.5f, 0.0f, 1, 3, 46.0f, 43.8f, 1, 0.700f, 0.000f, 0, 0.100f, 0.5f, 500.0f, 0.200f }, "Clean" }, // fit 22.1 to SSP Signature Schranz Kick 28.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang G#1 VII)
    { "Warm Clang A#1 IV",         "Schranz",           { 1.3f, 0.413f, 0.953f, 1.000f, 0.900f, 0.433f, 1.000f, 0.001f, -4.9f, 40.0f, 0.0f, 1, 1, 43.5f, 33.4f, 2, 0.536f, 0.700f, 0, 0.500f, 7.5f, 1600.0f }, "Clean" }, // fit 15.3 to MS Signature Kick Shot 23.wav (refit with tonality 2026-10-23)
    { "Warm Clang B1 VIII",        "Schranz",           { 2.1f, 0.418f, 0.666f, 1.000f, 1.000f, 0.348f, 0.900f, 0.000f, -3.8f, 20.0f, 0.0f, 0, 1, 31.3f, 41.0f, 2, 0.988f, 0.291f, 0, 0.598f, 8.7f, 600.0f }, "Clean" }, // fit 16.8 to SSP Signature Schranz Kick 35.wav (refit with tonality 2026-10-23)
    { "Crushed Clang B1 IV",       "Schranz",           { 2.1f, 0.421f, 0.702f, 0.875f, 1.000f, 0.781f, 0.916f, 0.000f, -4.1f, 45.1f, 0.0f, 0, 1, 46.0f, 30.9f, 2, 0.311f, 0.927f, 0, 0.155f, 2.6f, 1000.0f, 0.552f }, "Clean" }, // fit 29.4 to SSP Signature Schranz Kick 32.wav (duplicate split 2026-11-12, was Driven Clang B1 VII)
    { "Driven Clang B1 VI",        "Schranz",           { 1.5f, 0.425f, 0.875f, 0.721f, 0.938f, 0.555f, 0.383f, 0.000f, -3.1f, 26.0f, 0.0f, 0, 1, 30.4f, 58.3f, 2, 0.658f, 0.181f, 0, 0.148f, 2.2f, 1000.0f }, "Clean" }, // fit 12.7 to SSP Signature Schranz Kick 40.wav (refit with tonality 2026-10-23, was Warm Clang B1 II)
    { "Warm Clang C2",             "Schranz",           { 2.6f, 0.418f, 0.883f, 0.997f, 0.849f, 0.256f, 1.000f, 0.000f, -4.8f, 37.8f, 0.0f, 1, 1, 48.0f, 37.9f, 2, 0.589f, 0.983f, 0, 0.700f, 4.0f, 1600.0f, 0.002f }, "Driven" }, // fit 22.2 to MS Signature Kick Shot 12.wav (rumble tail and power refit 2026-11-12, was Warm Rumble C2)
    { "Driven Clang A#1 VI",       "Schranz",           { 1.4f, 0.427f, 1.000f, 0.771f, 0.700f, 0.538f, 0.940f, 0.000f, -3.2f, 20.0f, 0.0f, 0, 1, 24.5f, 43.7f, 2, 0.987f, 0.191f, 0, 0.015f, 8.6f, 500.0f }, "Clean" }, // fit 12.8 to SSP Signature Schranz Kick 40.wav (refit with tonality 2026-10-23, was Warm Clang A#1 V)
    { "Wide Crushed Rumble G#1",   "Schranz",           { -0.6f, 0.507f, 1.000f, 0.042f, 0.225f, 0.821f, 0.680f, 0.306f, -3.2f, 80.0f, 0.0f, 1, 1, 45.5f, 30.7f, 0, 1.000f, 0.000f, 0, 0.295f, 3.0f, 1000.0f, 0.431f }, "Clean" }, // fit 43.8 to SSP Signature Schranz Kick 37.wav (rumble tail and power refit 2026-11-12)
    { "Driven Clang A1 VII",       "Schranz",           { -0.2f, 0.425f, 0.465f, 0.869f, 1.000f, 0.582f, 0.447f, 0.000f, -3.7f, 30.0f, 0.0f, 0, 1, 47.9f, 40.4f, 2, 0.540f, 0.878f, 0, 0.226f, 8.3f, 1000.0f, 0.422f }, "Clean" }, // fit 19.2 to SSP Signature Schranz Kick 42.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang A1 III)
    { "Grinding Rumble F1",        "Schranz",           { -4.0f, 0.469f, 0.746f, 0.188f, 0.876f, 0.643f, 0.528f, 0.943f, 0.3f, 0.2f, 0.0f, 2, 1, 43.9f, 7.2f, 2, 0.363f, 0.000f, 0, 0.300f, 24.0f, 500.0f, 0.119f }, "Clean" }, // fit 46.2 to OBI Driven Schranz Kick 2.wav (rumble tail and power refit 2026-11-12)
    { "Driven Clang A#1 VII",      "Schranz",           { 1.5f, 0.425f, 1.000f, 0.955f, 0.975f, 0.552f, 0.440f, 0.000f, -3.6f, 20.0f, 0.0f, 0, 1, 35.5f, 35.6f, 2, 0.698f, 0.143f, 0, 0.000f, 1.1f, 1000.0f }, "Clean" }, // fit 15.1 to SSP Signature Schranz Kick 5.wav (refit with tonality 2026-10-23, was Clean Clang A#1 III)
    { "Warm Drop A#1 II",          "Schranz",           { 1.2f, 0.424f, 0.187f, 1.000f, 0.700f, 0.467f, 0.957f, 0.001f, -3.2f, 40.0f, 0.0f, 0, 1, 47.6f, 12.2f, 1, 0.800f, 1.000f, 0, 0.100f, 16.4f, 1000.0f }, "Clean" }, // fit 17.5 to OBI Driven Schranz Kick 8.wav (refit with tonality 2026-10-23, was Clean Click A#1 III)
    { "Warm Drop G#1 II",          "Schranz",           { -0.6f, 0.418f, 0.692f, 1.000f, 0.000f, 0.400f, 0.867f, 0.001f, -5.0f, 42.1f, 0.0f, 0, 3, 40.2f, 16.0f, 1, 0.929f, 0.370f, 0, 0.063f, 24.0f, 1000.0f }, "Wide" }, // fit 15.5 to SSP Signature Schranz Kick 64.wav (refit with tonality 2026-10-23, was Wide Warm Click G#1)
    { "Clipped Drop B1",           "Schranz",           { 2.5f, 0.417f, 0.678f, 0.944f, 0.575f, 0.524f, 0.963f, 0.000f, -3.7f, 17.3f, 0.0f, 1, 1, 42.2f, 29.0f, 2, 0.356f, 0.595f, 0, 0.286f, 7.4f, 1000.0f }, "Clean" }, // fit 13.0 to MS Signature Kick Shot 43.wav (refit with tonality 2026-10-23, was Warm Clang B1 IV)
    { "Crushed Click G1 III",      "Schranz",           { -2.3f, 0.330f, 0.891f, 0.334f, 0.969f, 0.904f, 0.346f, 0.299f, -0.3f, 20.0f, 6.0f, 0, 3, 11.7f, 100.7f, 1, 0.383f, 0.000f, 0, 0.000f, 11.8f, 1000.0f, 0.400f }, "Clean" }, // fit 41.6 to SSP Signature Schranz Kick 84.wav (rumble tail and power refit 2026-11-12, was Crushed Rumble G1 II)
    { "Crushed Drop G1 III",       "Schranz",           { -1.5f, 0.402f, 0.614f, 0.032f, 0.200f, 0.971f, 0.425f, 0.170f, 2.7f, 1.3f, 0.0f, 0, 0, 18.3f, 28.7f, 0, 0.800f, 0.000f, 0, 0.000f, 11.2f, 1600.0f, 0.745f }, "Clean" }, // fit 38.9 to OBI Driven Schranz Kick 7.wav (rumble tail and power refit 2026-11-12, was Crushed Rumble G1 III)
    { "Warm Rumble G1 VI",         "Schranz",           { -1.8f, 0.286f, 0.035f, 0.535f, 0.416f, 0.496f, 0.220f, 0.300f, -0.7f, 10.0f, 0.0f, 1, 3, 27.2f, 58.3f, 2, 0.164f, 0.000f, 0, 0.500f, 16.5f, 1600.0f, 0.300f }, "Clean" }, // fit 15.7 to ITHTS Signature Schranz Kick 9.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Click G1 V)
    { "Clipped Clang B1 IV",       "Schranz",           { 1.5f, 0.430f, 0.648f, 0.711f, 0.837f, 0.621f, 0.331f, 0.000f, -3.7f, 0.0f, 0.0f, 1, 0, 29.9f, 39.3f, 2, 0.732f, 0.090f, 0, 0.000f, 0.0f, 1000.0f }, "Clean" }, // fit 21.8 to ARK Kick Drum Shot 25.wav (refit with tonality 2026-10-23, was Clean Boom B1 II)
    // --- Industrial (56) ---
    { "Distorted Factory",         "Industrial",        { -3.6f, 0.445f, 0.388f, 0.900f, 0.000f, 0.887f, 0.734f, 0.000f, -1.9f, 13.6f, 6.0f, 1, 1, 44.3f, 49.4f, 1, 0.710f, 0.446f, 0, 0.100f, 0.5f, 1000.0f }, "Crushed" }, // fit 11.5 to ITHTS Distorted Kick Shot 6.wav (kick register)
    { "Warm Click A#1 II",         "Industrial",        { 1.4f, 0.348f, 0.685f, 0.800f, 0.900f, 0.472f, 0.800f, 0.243f, -2.9f, 0.0f, 0.0f, 1, 4, 42.0f, 42.9f, 1, 0.300f, 0.000f, 0, 0.500f, 2.7f, 1600.0f }, "Short" }, // fit 12.3 to HDIT3 Heavy Kick Shot 13.wav (kick register)
    { "Crushed Click A#1",         "Industrial",        { 1.0f, 0.326f, 0.926f, 0.933f, 1.000f, 0.949f, 0.800f, 0.100f, -2.5f, 30.0f, 3.0f, 1, 4, 31.6f, 27.7f, 1, 1.000f, 0.000f, 0, 0.100f, 1.0f, 1000.0f }, "Short" }, // fit 16.3 to ITHTS Distorted Kick Shot 5.wav (kick register)
    { "Crushed Clang F1",          "Industrial",        { -3.9f, 0.309f, 0.898f, 0.729f, 1.000f, 0.900f, 0.650f, 0.000f, -5.4f, 3.3f, 0.0f, 0, 1, 14.1f, 73.3f, 2, 0.000f, 0.350f, 0, 0.500f, 5.5f, 1000.0f }, "Short" }, // fit 13.8 to HDIT3 Heavy Kick Shot 12.wav (kick register)
    { "Wide Crushed Clang F1",     "Industrial",        { -4.0f, 0.443f, 0.574f, 0.853f, 0.820f, 0.983f, 0.400f, 0.000f, -6.9f, 90.0f, 6.0f, 1, 4, 48.0f, 40.3f, 2, 0.555f, 0.806f, 0, 0.700f, 1.9f, 1000.0f }, "Wide" }, // fit 14.8 to ITHTS Distorted Kick Shot 10.wav (kick register)
    { "Wide Clipped Clang F#1",    "Industrial",        { -2.6f, 0.446f, 0.672f, 0.900f, 0.900f, 0.700f, 0.200f, 0.000f, -6.5f, 50.0f, 6.0f, 1, 4, 39.1f, 36.7f, 2, 0.800f, 0.324f, 0, 0.300f, 1.7f, 1000.0f }, "Wide" }, // fit 9.9 to ITHTS Distorted Kick Shot 3.wav (kick register)
    { "Warm Rumble G1 VII",        "Industrial",        { -1.6f, 0.487f, 0.098f, 0.429f, 0.800f, 0.500f, 1.000f, 0.464f, -5.9f, 0.0f, 0.0f, 1, 1, 32.4f, 18.1f, 2, 0.514f, 0.016f, 0, 0.778f, 14.4f, 500.0f, 0.180f }, "Long" }, // fit 23.8 to HDIT3 Heavy Kick Shot 9.wav (rumble tail and power refit 2026-11-12, was Clipped Rumble G1 II)
    { "Warm Click D#1",            "Industrial",        { -5.8f, 0.391f, 0.791f, 0.208f, 1.000f, 0.456f, 1.000f, 0.180f, -6.2f, 0.0f, 0.0f, 1, 4, 40.0f, 63.5f, 1, 0.200f, 1.000f, 0, 0.566f, 4.3f, 500.0f }, "Short" }, // fit 14.8 to HDIT3 Processed FX Kick Shot 2.wav (refit with tonality 2026-10-23)
    { "Crushed Rumble F#1",        "Industrial",        { -3.3f, 0.463f, 0.842f, 0.316f, 0.995f, 0.754f, 0.000f, 0.810f, -3.8f, 0.0f, 0.0f, 1, 0, 33.4f, 36.5f, 2, 0.327f, 0.131f, 0, 0.552f, 3.0f, 500.0f, 0.000f }, "Long" }, // fit 17.5 to HDIT3 Heavy Kick Shot 14.wav (rumble tail and power refit 2026-11-12, was Clipped Rumble F#1 VI)
    { "Driven Click A#1 II",       "Industrial",        { 1.3f, 0.452f, 0.633f, 0.833f, 1.000f, 0.700f, 0.200f, 0.100f, 1.4f, 0.0f, 0.0f, 0, 2, 47.9f, 31.9f, 0, 0.100f, 0.400f, 0, 1.000f, 1.1f, 1000.0f }, "Driven" }, // fit 11.2 to HDIT3 Heavy Kick Shot 27.wav (kick register)
    { "Industrial Rumble",         "Industrial",        { 0.3f, 0.419f, 0.823f, 0.439f, 1.000f, 0.564f, 0.379f, 0.616f, -1.7f, 0.0f, 0.0f, 1, 3, 31.2f, 45.3f, 1, 0.366f, 0.000f, 0, 0.527f, 1.7f, 500.0f }, "Rumble" }, // fit 13.2 to OBI Hard Industrial Kick 16.wav (kick register)
    { "Steel Press",               "Industrial",        { 1.0f, 0.400f, 0.800f, 0.650f, 0.600f, 0.750f, 0.700f, 0.350f, -2.7f, 80.0f, 10.0f, 2, 3, 38.0f, 22.0f, 2, 0.900f, 0.450f, 0, 1.000f, 0.0f, 500.0f }, "Rumble" }, // hand-made
    { "Wide Iron Sub",             "Industrial",        { -2.0f, 0.650f, 0.650f, 0.700f, 0.500f, 0.700f, 0.750f, 0.400f, -3.4f, 100.0f, 14.0f, 2, 4, 28.0f, 34.0f, 2, 0.550f, 0.550f, 1, 1.000f, 0.0f, 500.0f }, "Rumble" }, // hand-made
    { "Clipped Rumble D#1 II",     "Industrial",        { -5.6f, 0.394f, 0.856f, 0.454f, 0.500f, 0.627f, 0.479f, 0.363f, -0.4f, 0.3f, 0.0f, 1, 3, 18.3f, 44.7f, 1, 0.100f, 0.015f, 0, 0.600f, 5.4f, 500.0f, 0.260f }, "Rumble" }, // fit 16.0 to OBI Hard Industrial Kick 9.wav (rumble tail and power refit 2026-11-12)
    { "Clipped Clang B1",          "Industrial",        { 2.1f, 0.320f, 0.300f, 0.900f, 1.000f, 0.700f, 0.800f, 0.120f, -4.4f, 10.0f, 0.0f, 1, 1, 36.2f, 28.0f, 2, 0.175f, 0.000f, 0, 0.900f, 3.6f, 1000.0f }, "Short" }, // fit 16.3 to HDIT3 Heavy Kick Shot 3.wav (kick register)
    { "Clipped Rumble F#1 IV",     "Industrial",        { -2.5f, 0.275f, 0.011f, 0.739f, 0.600f, 0.502f, 0.700f, 0.300f, -2.0f, 0.0f, 0.0f, 1, 1, 41.4f, 38.8f, 2, 0.454f, 0.000f, 0, 0.900f, 12.2f, 500.0f }, "Clean" }, // fit 15.0 to OBI Hard Industrial Kick 13.wav (refit with tonality 2026-10-23, was Clean Clang F#1 II)
    { "Clipped Rumble B1 II",      "Industrial",        { 1.6f, 0.284f, 0.552f, 0.988f, 1.000f, 0.589f, 0.700f, 0.541f, -3.8f, 0.4f, 0.0f, 1, 4, 44.0f, 23.8f, 2, 0.600f, 0.050f, 0, 0.424f, 4.3f, 1600.0f, 0.400f }, "Rumble" }, // fit 28.7 to HDIT3 Heavy Kick Shot 25.wav (rumble tail and power refit 2026-11-12)
    { "Clipped Click A#1 II",      "Industrial",        { 0.9f, 0.365f, 0.841f, 0.653f, 1.000f, 0.571f, 0.296f, 0.177f, -3.8f, 1.0f, 0.0f, 1, 3, 32.8f, 35.0f, 1, 0.070f, 0.000f, 0, 0.100f, 0.1f, 500.0f, 0.000f }, "Rumble" }, // fit 24.1 to HDIT3 Heavy Kick Shot 8.wav (rumble tail and power refit 2026-11-12, was Clipped Rumble A#1 V)
    { "Crushed Rumble F1",         "Industrial",        { -3.6f, 0.479f, 0.647f, 0.317f, 1.000f, 0.815f, 0.300f, 0.540f, -2.5f, 10.0f, 0.0f, 1, 4, 18.7f, 47.2f, 1, 0.592f, 0.100f, 0, 0.509f, 6.3f, 500.0f, 0.077f }, "Long" }, // fit 21.6 to HDIT3 Heavy Kick Shot 1.wav (rumble tail and power refit 2026-11-12)
    { "Clipped Boom B1",           "Industrial",        { 1.8f, 0.463f, 0.981f, 0.761f, 0.000f, 0.638f, 0.500f, 0.292f, 1.0f, 1.2f, 0.0f, 1, 2, 32.9f, 36.2f, 1, 0.108f, 0.000f, 0, 0.761f, 0.0f, 500.0f, 0.100f }, "Rumble" }, // fit 25.2 to HDIT3 Heavy Kick Shot 30.wav (rumble tail and power refit 2026-11-12, was Clipped Rumble B1)
    { "Warm Rumble G1",            "Industrial",        { -2.2f, 0.487f, 0.794f, 0.339f, 1.000f, 0.417f, 0.720f, 0.968f, -4.3f, 0.0f, 0.0f, 1, 4, 41.6f, 27.2f, 1, 0.219f, 0.100f, 0, 1.000f, 0.0f, 500.0f, 0.065f }, "Rumble" }, // fit 20.8 to HDIT3 Heavy Kick Shot 20.wav (rumble tail and power refit 2026-11-12)
    { "Warm Rumble C1",            "Industrial",        { -8.8f, 0.484f, 0.306f, 0.447f, 0.428f, 0.465f, 0.612f, 0.500f, -2.1f, 0.0f, 0.0f, 1, 3, 16.1f, 119.3f, 2, 0.717f, 0.154f, 1, 0.835f, 5.2f, 500.0f, 0.104f }, "Rumble" }, // fit 11.9 to HDIT3 Heavy Kick Shot 15.wav (rumble tail and power refit 2026-11-12, was Clipped Rumble C1)
    { "Clipped Rumble G#1 III",    "Industrial",        { -1.4f, 0.411f, 0.977f, 0.421f, 0.900f, 0.630f, 0.272f, 0.619f, -4.0f, 0.0f, 0.0f, 1, 3, 28.0f, 58.5f, 2, 0.244f, 0.170f, 0, 0.592f, 0.0f, 500.0f, 0.562f }, "Rumble" }, // fit 19.7 to HDIT3 Assorted Kick Shot 12.wav (rumble tail and power refit 2026-11-12, was Warm Rumble G#1 IV)
    { "Clipped Clang F#1 V",       "Industrial",        { -2.6f, 0.470f, 0.760f, 0.343f, 0.861f, 0.518f, 0.800f, 0.004f, -4.1f, 0.6f, 6.0f, 1, 3, 48.0f, 20.8f, 2, 0.833f, 0.200f, 0, 0.300f, 7.5f, 1000.0f, 0.140f }, "Rumble" }, // fit 20.0 to HDIT3 Heavy Kick Shot 24.wav (rumble tail and power refit 2026-11-12)
    { "Warm Rumble F1",            "Industrial",        { -4.1f, 0.354f, 0.486f, 0.824f, 0.300f, 0.258f, 0.994f, 0.317f, -3.9f, 0.0f, 0.0f, 1, 3, 46.2f, 42.0f, 2, 0.977f, 0.017f, 0, 0.821f, 10.3f, 500.0f, 0.015f }, "Rumble" }, // fit 19.8 to HDIT3 Heavy Kick Shot 23.wav (rumble tail and power refit 2026-11-12)
    { "Clipped Rumble F#1 II",     "Industrial",        { -2.6f, 0.713f, 0.851f, 0.008f, 1.000f, 0.616f, 0.515f, 1.000f, -4.1f, 16.6f, 0.0f, 1, 3, 35.7f, 34.9f, 1, 0.900f, 0.123f, 0, 0.318f, 2.0f, 500.0f, 0.196f }, "Rumble" }, // fit 15.2 to HDIT3 Heavy Kick Shot 2.wav (rumble tail and power refit 2026-11-12)
    { "Clipped Rumble F1 II",      "Industrial",        { -3.8f, 0.489f, 0.832f, 0.338f, 0.800f, 0.741f, 0.200f, 0.500f, -2.8f, 0.0f, 0.0f, 1, 0, 36.6f, 40.2f, 2, 0.361f, 0.090f, 0, 0.600f, 2.1f, 500.0f, 0.039f }, "Rumble" }, // fit 16.4 to OBI Hard Industrial Kick 6.wav (rumble tail and power refit 2026-11-12, was Crushed Rumble F1 II)
    { "Clipped Rumble A#1 II",     "Industrial",        { 0.5f, 0.346f, 1.000f, 1.000f, 0.900f, 0.670f, 0.700f, 0.789f, -1.8f, 0.0f, 0.0f, 1, 4, 46.8f, 23.8f, 0, 0.000f, 0.000f, 0, 0.600f, 9.0f, 500.0f, 0.146f }, "Rumble" }, // fit 17.1 to HDIT3 Heavy Kick Shot 4.wav (rumble tail and power refit 2026-11-12)
    { "Clipped Rumble F#1 III",    "Industrial",        { -3.2f, 0.347f, 0.870f, 1.000f, 0.914f, 0.624f, 0.994f, 0.900f, -1.7f, 0.0f, 0.0f, 1, 4, 19.3f, 87.8f, 1, 0.944f, 0.056f, 0, 0.219f, 4.9f, 500.0f, 0.047f }, "Rumble" }, // fit 22.7 to HDIT3 Heavy Kick Shot 10.wav (rumble tail and power refit 2026-11-12)
    { "Clipped Rumble A1 II",      "Industrial",        { -0.4f, 0.361f, 0.573f, 0.990f, 0.964f, 0.582f, 0.800f, 0.400f, -6.2f, 0.0f, 0.0f, 1, 1, 40.5f, 31.2f, 2, 0.729f, 0.000f, 0, 0.195f, 0.0f, 500.0f, 0.195f }, "Rumble" }, // fit 21.1 to HDIT3 Heavy Kick Shot 9.wav (rumble tail and power refit 2026-11-12)
    { "Clipped Rumble F#1",        "Industrial",        { -3.5f, 0.350f, 0.588f, 0.600f, 0.800f, 0.500f, 0.900f, 0.406f, -4.5f, 0.0f, 0.0f, 1, 4, 38.4f, 46.7f, 2, 0.294f, 0.120f, 0, 0.800f, 7.4f, 1600.0f, 0.106f }, "Rumble" }, // fit 18.5 to HDIT3 Processed FX Kick Shot 7.wav (rumble tail and power refit 2026-11-12, was Warm Rumble F#1)
    { "Warm Rumble A#1",           "Industrial",        { 0.8f, 0.402f, 0.478f, 0.635f, 0.900f, 0.442f, 0.800f, 0.700f, -0.3f, 0.0f, 0.0f, 1, 0, 39.1f, 20.7f, 1, 0.000f, 0.132f, 0, 0.700f, 7.6f, 500.0f, 0.400f }, "Rumble" }, // fit 23.5 to OBI Hard Industrial Kick 3.wav (rumble tail and power refit 2026-11-12, was Clipped Rumble A#1 III)
    { "Crushed Rumble A1",         "Industrial",        { 0.2f, 0.455f, 0.929f, 0.256f, 0.800f, 0.801f, 0.265f, 0.844f, -3.8f, 0.0f, 0.0f, 1, 1, 39.0f, 34.6f, 2, 0.104f, 0.167f, 0, 0.987f, 2.3f, 500.0f, 0.116f }, "Rumble" }, // fit 19.5 to OBI Hard Industrial Kick 20.wav (rumble tail and power refit 2026-11-12)
    { "Warm Rumble G1 III",        "Industrial",        { -1.6f, 0.324f, 0.837f, 0.875f, 1.000f, 0.460f, 0.316f, 0.363f, -2.7f, 0.0f, 0.0f, 1, 3, 28.5f, 54.5f, 1, 0.030f, 0.217f, 0, 0.566f, 3.9f, 500.0f, 0.000f }, "Rumble" }, // fit 21.3 to HDIT3 Heavy Kick Shot 26.wav (rumble tail and power refit 2026-11-12)
    { "Clipped Rumble C#1",        "Industrial",        { -7.8f, 0.651f, 0.467f, 0.183f, 0.312f, 0.654f, 0.735f, 0.900f, -0.7f, 0.0f, 0.0f, 1, 4, 39.4f, 22.1f, 2, 0.700f, 0.164f, 0, 0.700f, 14.2f, 500.0f, 0.106f }, "Rumble" }, // fit 13.7 to OBI Hard Industrial Kick 19.wav (rumble tail and power refit 2026-11-12)
    { "Clean Rumble G1",           "Industrial",        { -2.1f, 0.538f, 0.614f, 0.139f, 0.900f, 0.086f, 0.812f, 0.469f, -2.5f, 0.0f, 0.0f, 1, 3, 29.2f, 52.5f, 0, 0.617f, 0.470f, 0, 0.866f, 4.2f, 500.0f, 0.012f }, "Rumble" }, // fit 14.1 to OBI Hard Industrial Kick 15.wav (rumble tail and power refit 2026-11-12)
    { "Driven Click C#1",          "Industrial",        { -8.0f, 0.412f, 0.514f, 1.000f, 1.000f, 0.696f, 0.952f, 0.200f, -4.0f, 0.0f, 0.0f, 0, 3, 45.5f, 37.7f, 1, 0.000f, 0.200f, 1, 0.805f, 7.9f, 1000.0f }, "Driven" }, // fit 10.3 to HDIT3 Processed FX Kick Shot 14.wav (kick register)
    { "Clipped Rumble G1 III",     "Industrial",        { -2.1f, 0.581f, 0.790f, 0.327f, 0.000f, 0.536f, 1.000f, 0.729f, -3.8f, 0.0f, 0.0f, 1, 4, 41.1f, 21.2f, 0, 0.985f, 0.227f, 0, 0.253f, 8.5f, 500.0f, 0.200f }, "Rumble" }, // fit 19.4 to HDIT3 Heavy Kick Shot 11.wav (rumble tail and power refit 2026-11-12, was Warm Rumble G1 II)
    { "Warm Rumble G1 VIII",       "Industrial",        { -2.2f, 0.250f, 0.667f, 0.869f, 0.385f, 0.252f, 0.900f, 0.387f, -4.1f, 20.0f, 0.0f, 1, 3, 39.7f, 61.3f, 2, 0.892f, 0.000f, 0, 0.535f, 3.6f, 500.0f, 0.053f }, "Wide" }, // fit 17.5 to OBI Hard Industrial Kick 2.wav (rumble tail and power refit 2026-11-12, was Clean Rumble G1 II)
    { "Warm Clang A1 II",          "Industrial",        { -0.2f, 0.295f, 0.800f, 0.573f, 0.900f, 0.469f, 0.995f, 0.011f, -4.6f, 40.0f, 0.0f, 1, 3, 33.3f, 35.9f, 2, 1.000f, 0.000f, 0, 0.300f, 7.4f, 500.0f, 0.024f }, "Short" }, // fit 16.9 to ITHTS Heavy Kick 11.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Crushed Clang A1)
    { "Wide Crushed Boom A1",      "Industrial",        { 0.0f, 0.416f, 0.000f, 1.000f, 0.500f, 0.800f, 0.200f, 0.000f, -2.9f, 80.0f, 0.0f, 1, 4, 48.0f, 36.2f, 2, 0.759f, 0.961f, 0, 0.500f, 1.4f, 250.0f, 0.847f }, "Wide" }, // fit 16.5 to ITHTS Heavy Kick 10.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Wide Grinding Boom A1)
    { "Wide Warm Boom A#1 II",     "Industrial",        { 1.4f, 0.423f, 0.951f, 1.000f, 0.700f, 0.343f, 0.800f, 0.000f, -4.4f, 70.0f, 0.0f, 1, 1, 33.4f, 28.0f, 1, 0.173f, 0.236f, 0, 1.000f, 4.4f, 1000.0f }, "Wide" }, // fit 11.2 to OBI Hard Industrial Kick 5.wav (kick register)
    { "Crushed Clang B1",          "Industrial",        { 2.1f, 0.442f, 0.220f, 0.800f, 0.700f, 0.957f, 0.500f, 0.000f, -0.2f, 0.0f, 0.0f, 1, 2, 42.2f, 50.1f, 2, 0.600f, 0.191f, 0, 0.100f, 0.7f, 500.0f }, "Crushed" }, // fit 11.5 to OBI Hard Industrial Kick 4.wav (kick register)
    { "Clipped Clang F#1 VI",      "Industrial",        { -2.9f, 0.455f, 0.319f, 0.700f, 1.000f, 0.697f, 0.102f, 0.000f, -3.6f, 20.0f, 0.0f, 1, 3, 35.0f, 55.8f, 2, 0.763f, 0.008f, 0, 0.387f, 12.4f, 1000.0f, 0.500f }, "Clean" }, // fit 13.6 to ITHTS Distorted Kick Shot 2.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Warm Clang F#1 IV)
    { "Clipped Click F1",          "Industrial",        { -4.2f, 0.453f, 0.996f, 0.029f, 1.000f, 0.607f, 1.000f, 0.005f, -5.6f, 0.0f, 0.0f, 1, 4, 29.7f, 67.2f, 1, 0.100f, 0.300f, 0, 0.804f, 3.9f, 500.0f }, "Driven" }, // fit 15.0 to HDIT3 Processed FX Kick Shot 8.wav (kick register)
    { "Crushed Drop G1",           "Industrial",        { -2.1f, 0.444f, 0.161f, 1.000f, 0.005f, 1.000f, 0.669f, 0.000f, -2.8f, 27.3f, 3.0f, 1, 1, 22.8f, 12.3f, 1, 0.435f, 0.687f, 0, 0.975f, 13.4f, 250.0f }, "Crushed" }, // fit 18.4 to ITHTS Distorted Kick Shot 4.wav (kick register)
    { "Crushed Click G1",          "Industrial",        { -2.2f, 0.445f, 0.609f, 0.900f, 0.800f, 1.000f, 1.000f, 0.000f, 0.2f, 0.0f, 0.0f, 1, 4, 33.4f, 63.5f, 1, 0.000f, 0.967f, 0, 0.700f, 3.7f, 500.0f }, "Crushed" }, // fit 14.9 to ITHTS Distorted Kick Shot 1.wav (kick register)
    { "Warm Click F1",             "Industrial",        { -4.3f, 0.451f, 0.780f, 0.900f, 1.000f, 0.400f, 1.000f, 0.003f, -3.1f, 11.7f, 0.0f, 1, 1, 43.8f, 60.7f, 0, 1.000f, 0.000f, 0, 0.600f, 1.8f, 1000.0f }, "Clean" }, // fit 15.2 to ITHTS Distorted Kick Shot 9.wav (kick register)
    { "Warm Thump A1 II",          "Industrial",        { -0.4f, 0.440f, 0.434f, 0.394f, 0.178f, 0.491f, 0.783f, 0.067f, -2.1f, 3.8f, 0.0f, 1, 3, 46.4f, 18.5f, 2, 0.828f, 0.405f, 0, 0.200f, 1.3f, 1000.0f, 0.338f }, "Driven" }, // fit 16.5 to HDIT3 Assorted Kick Shot 11.wav (power refit 2026-11-12: WEIGHT, low end, attack, was Clipped Clang A1)
    { "Clipped Click A#1 III",     "Industrial",        { 1.0f, 0.397f, 0.463f, 0.667f, 0.952f, 0.578f, 0.685f, 0.138f, -3.1f, 0.0f, 0.0f, 1, 3, 38.1f, 28.7f, 1, 0.800f, 0.036f, 0, 0.800f, 3.4f, 500.0f }, "Driven" }, // fit 11.8 to ITHTS Heavy Kick 7.wav (kick register)
    { "Clipped Rumble F1 III",     "Industrial",        { -3.5f, 0.374f, 0.873f, 0.300f, 1.000f, 0.746f, 0.171f, 0.494f, -2.9f, 0.0f, 0.0f, 1, 4, 33.5f, 57.9f, 1, 0.000f, 0.000f, 0, 0.800f, 2.3f, 500.0f, 0.066f }, "Clean" }, // fit 32.3 to OBI Hard Industrial Kick 11.wav (rumble tail and power refit 2026-11-12, was Crushed Rumble F1 III)
    { "Warm Clang E1",             "Industrial",        { -4.7f, 0.346f, 0.400f, 0.634f, 1.000f, 0.300f, 0.900f, 0.296f, -4.9f, 0.0f, 0.0f, 1, 3, 40.2f, 54.2f, 2, 0.000f, 0.000f, 0, 0.700f, 9.7f, 500.0f }, "Short" }, // fit 15.6 to HDIT3 Heavy Kick Shot 7.wav (kick register)
    { "Warm Thump F#1",            "Industrial",        { -3.1f, 0.418f, 0.419f, 0.405f, 0.223f, 0.445f, 0.700f, 0.180f, -1.7f, 0.0f, 0.0f, 1, 3, 45.1f, 41.7f, 2, 0.806f, 0.069f, 0, 0.700f, 0.2f, 1000.0f }, "Clean" }, // fit 11.6 to HDIT3 Heavy Kick Shot 6.wav (kick register)
    { "Warm Click F#1",            "Industrial",        { -3.4f, 0.478f, 0.635f, 0.360f, 1.000f, 0.294f, 0.699f, 0.100f, -4.7f, 0.5f, 0.0f, 1, 3, 48.0f, 4.0f, 1, 0.152f, 0.000f, 0, 0.962f, 19.2f, 500.0f, 0.000f }, "Clean" }, // fit 22.0 to HDIT3 Heavy Kick Shot 8.wav (rumble tail and power refit 2026-11-12, was Clean Rumble F#1 II)
    { "Warm Click F1 II",          "Industrial",        { -4.2f, 0.324f, 0.951f, 0.991f, 0.800f, 0.459f, 0.802f, 0.284f, -3.3f, 30.0f, 3.0f, 1, 4, 27.5f, 53.2f, 1, 0.500f, 0.029f, 0, 0.400f, 4.4f, 1000.0f, 0.148f }, "Rumble" }, // fit 15.4 to ITHTS Heavy Kick 3.wav (rumble tail and power refit 2026-11-12, was Driven Rumble F1)
    { "Clean Rumble F#1",          "Industrial",        { -2.5f, 0.353f, 0.164f, 0.800f, 0.500f, 0.177f, 0.946f, 0.563f, -1.0f, 0.4f, 0.0f, 1, 4, 45.3f, 39.4f, 0, 0.800f, 0.003f, 0, 0.800f, 3.7f, 500.0f, 0.232f }, "Rumble" }, // fit 23.5 to HDIT3 Heavy Kick Shot 16.wav (rumble tail and power refit 2026-11-12)
    // --- Rave / Hard Dance (4) ---
    { "Hardstyle Tok",             "Rave / Hard Dance", { 3.0f, 0.500f, 0.850f, 0.700f, 0.500f, 0.750f, 0.700f, 0.100f, -3.8f, 25.0f, 2.0f, 0, 1, 48.0f, 36.0f, 1, 0.700f, 0.500f, 0, 1.000f, 0.0f, 500.0f }, "Long" }, // hand-made (pitch time 45 -> 36 ms: kick register)
    { "Rawstyle Screech",          "Rave / Hard Dance", { 3.0f, 0.480f, 0.850f, 0.750f, 0.600f, 0.950f, 0.850f, 0.150f, 0.0f, 45.0f, 4.0f, 2, 4, 46.0f, 40.0f, 2, 0.800f, 0.500f, 0, 1.000f, 0.0f, 500.0f }, "Long" }, // hand-made
    { "Gabba Lite",                "Rave / Hard Dance", { 4.0f, 0.450f, 0.900f, 0.750f, 0.550f, 0.900f, 0.700f, 0.000f, -3.0f, 0.0f, 0.0f, 1, 3, 46.0f, 30.0f, 1, 0.600f, 0.500f, 0, 1.000f, 0.0f, 500.0f }, "Crushed" }, // hand-made
    { "Euphoric Punch",            "Rave / Hard Dance", { 4.0f, 0.400f, 0.900f, 0.600f, 0.550f, 0.550f, 0.600f, 0.000f, -3.5f, 35.0f, 3.0f, 0, 1, 42.0f, 30.0f, 0, 0.600f, 0.500f, 0, 1.000f, 0.0f, 500.0f }, "Driven" }, // hand-made
};

const char* const rowIds[23] = { id::tune, id::decay, id::punch, id::body, id::click, id::drive, id::character,
                                 id::rumble, id::output, id::width, id::delay, id::driveMode, id::charMode,
                                 id::pitchDepth, id::pitchTime, id::clickType, id::clickTone, id::rumbleTime,
                                 id::rumbleSync, id::bodyTone, id::pitchFall, id::splitFreq, id::weight };

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
            for (int i = 0; i < 23; ++i)
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
    static const Snapshot neutral { { id::bodyTone, 1.0f }, { id::pitchFall, 0.0f }, { id::splitFreq, 1000.0f },
                                    { id::beatLength, 0.0f }, { id::subLevel, 0.0f }, { id::startPhase, 0.0f }, { id::weight, 0.0f },
                                    { id::driveEnv, 0.0f }, { id::velDrive, 0.0f }, { id::oversample, 1.0f },
                                    { id::pitchCurve, 0.0f }, { id::keyRoot, 60.0f }, { id::modDrive, 0.0f },
                                    { id::choke, 0.0f }, { id::rumbleMode, 0.0f }, { id::analog, 0.0f } };
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
        // OVERSAMPLE is a quality setting of this instance, not part of a sound: presets (and A/B,
        // variations) never change it. Factory presets do not store it, so a switch used to fall
        // back to 4x and change the plug-in latency while the host was playing.
        // DIGITAL | ANALOGUE is kept the same way: a character switch over any preset.
        if (pid == id::oversample || pid == id::analog)
            continue;
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
    p.beatLength = juce::roundToInt (get (id::beatLength, (float) p.beatLength));
    p.subLevel = get (id::subLevel, p.subLevel);
    p.startPhase = get (id::startPhase, p.startPhase);
    p.weight = get (id::weight, p.weight);
    p.driveEnv = get (id::driveEnv, p.driveEnv);
    p.velDrive = get (id::velDrive, p.velDrive);
    p.analog = get (id::analog, 0.0f) > 0.5f;
    p.oversampleIndex = juce::roundToInt (get (id::oversample, (float) p.oversampleIndex));
    p.pitchCurve = get (id::pitchCurve, p.pitchCurve);
    p.keyRoot = juce::roundToInt (get (id::keyRoot, (float) p.keyRoot));
    p.modDrive = get (id::modDrive, p.modDrive);
    p.choke = get (id::choke, 0.0f) > 0.5f;
    p.rumbleMode = juce::roundToInt (get (id::rumbleMode, (float) p.rumbleMode));
    return p;
}

std::vector<int> filterPresets (const std::vector<Preset>& presets, const juce::String& category,
                                const juce::StringArray& groups, const juce::String& text)
{
    juce::StringArray words;
    words.addTokens (text, " \t", "");
    words.removeEmptyStrings();
    std::vector<int> out;
    for (int i = 0; i < (int) presets.size(); ++i)
    {
        const auto& p = presets[(size_t) i];
        if (category == favouritesCategory ? ! p.favourite : (category.isNotEmpty() && p.category != category))
            continue;
        if (! groups.isEmpty() && ! groups.contains (p.group))
            continue;
        bool match = true;
        for (const auto& w : words)
            match = match && (p.name.containsIgnoreCase (w) || p.category.containsIgnoreCase (w) || p.group.containsIgnoreCase (w));
        if (match)
            out.push_back (i);
    }
    // For reading: genres in library order (user presets last), names in natural order within a genre.
    std::map<juce::String, int> rank;
    for (const auto& p : presets)
        rank.emplace (p.category, (int) rank.size());
    std::stable_sort (out.begin(), out.end(), [&presets, &rank] (int a, int b)
    {
        const auto& pa = presets[(size_t) a];
        const auto& pb = presets[(size_t) b];
        if (pa.category != pb.category)
            return rank[pa.category] < rank[pb.category];
        return pa.name.compareNatural (pb.name) < 0;
    });
    return out;
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
// Favourites.json: { "format": 1, "factory": [names], "user": [names] }. Keys inside: "F|name" and "U|name".
namespace
{
juce::String favouriteKey (const Preset& p) { return (p.isFactory() ? "F|" : "U|") + p.name; }

juce::StringArray readFavourites (const juce::File& file)
{
    juce::StringArray keys;
    const auto json = juce::JSON::parse (file.loadFileAsString());
    if (auto* obj = json.getDynamicObject())
        for (auto [field, prefix] : { std::pair { "factory", "F|" }, std::pair { "user", "U|" } })
            if (auto* names = obj->getProperty (field).getArray())
                for (const auto& n : *names)
                    if (n.isString() && n.toString().isNotEmpty())
                        keys.addIfNotAlreadyThere (prefix + n.toString());
    return keys;
}

bool writeFavourites (const juce::File& file, const juce::StringArray& keys)
{
    juce::Array<juce::var> factory, user;
    for (const auto& k : keys)
        (k.startsWith ("F|") ? factory : user).add (k.substring (2));
    auto obj = std::make_unique<juce::DynamicObject>();
    obj->setProperty ("format", 1);
    obj->setProperty ("factory", factory);
    obj->setProperty ("user", user);
    return file.getParentDirectory().createDirectory() && file.replaceWithText (juce::JSON::toString (juce::var (obj.release())));
}

void markFavourites (std::vector<Preset>& presets, const juce::StringArray& keys)
{
    for (auto& p : presets)
        p.favourite = keys.contains (favouriteKey (p));
}

// Replaces one key by another (empty = drop it) in the file, keeping every other instance's stars.
void moveFavourite (const juce::File& file, const juce::String& from, const juce::String& to)
{
    auto keys = readFavourites (file);
    if (! keys.contains (from) && (to.isEmpty() || ! keys.contains (to)))
        return;
    keys.removeString (from);
    if (to.isNotEmpty())
        keys.addIfNotAlreadyThere (to);
    writeFavourites (file, keys);
}
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
    markFavourites (presets, readFavourites (favouritesFile()));
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
    const bool isNew = ! file.existsAsFile();
    if (! file.replaceWithText (presetToXml (sanitiseName (name), "User", values)))
    {
        error = "Cannot write " + file.getFullPathName();
        return false;
    }
    if (isNew) // a new preset starts without a star, even if one of that name was deleted outside the plug-in
        moveFavourite (favouritesFile(), "U|" + file.getFileNameWithoutExtension(), {});
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
    moveFavourite (favouritesFile(), "U|" + old.getFileNameWithoutExtension(), "U|" + target.getFileNameWithoutExtension());
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
    if (! moveToTrash (file))
    {
        error = "Could not move the preset to the Trash; nothing was deleted";
        return false;
    }
    moveFavourite (favouritesFile(), "U|" + file.getFileNameWithoutExtension(), {});
    rescan();
    return true;
}

bool PresetLibrary::toggleFavourite (int index)
{
    if (index < 0 || index >= (int) presets.size())
        return false;
    auto keys = readFavourites (favouritesFile()); // fresh: another instance may have starred something
    const auto key = favouriteKey (presets[(size_t) index]);
    const bool on = ! keys.contains (key);
    if (on)
        keys.add (key);
    else
        keys.removeString (key);
    if (writeFavourites (favouritesFile(), keys))
        markFavourites (presets, keys);
    else
        markFavourites (presets, readFavourites (favouritesFile()));
    return presets[(size_t) index].favourite;
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
