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
    // 300 presets. Fitted ones are matched to measured reference kicks (Tools/fit_kicks.py,
    // Tools/build_library.py); hand-made ones have no close reference. Last field: browser group.
    // --- Techno (30) ---
    { "Short Knock",               "Techno",            { -1.8f, 0.292f, 0.926f, 0.800f, 1.000f, 0.800f, 0.600f, 0.013f, -3.7f, 0.0f, 0.0f, 0, 1, 18.6f, 48.6f, 2, 0.036f, 0.000f, 0, 0.000f, 7.2f, 500.0f }, "Short" }, // fit 11.4 to SVEKick04S.wav
    { "Clipped Knock G1",          "Techno",            { -2.5f, 0.282f, 0.821f, 0.800f, 0.600f, 0.700f, 0.400f, 0.018f, -3.5f, 20.0f, 0.0f, 1, 1, 22.7f, 41.7f, 1, 0.100f, 0.000f, 0, 0.195f, 7.4f, 1000.0f }, "Short" }, // fit 13.0 to OBI Punchy Kick 19.wav
    { "Driven Clang G#1",          "Techno",            { -1.2f, 0.287f, 0.653f, 0.679f, 0.990f, 0.652f, 0.600f, 0.032f, -4.3f, 0.0f, 0.0f, 0, 1, 26.4f, 45.7f, 2, 0.011f, 0.000f, 0, 0.100f, 6.3f, 500.0f }, "Short" }, // fit 12.3 to SVEKick11S.wav
    { "Driven Clang G1",           "Techno",            { -1.7f, 0.295f, 0.930f, 0.800f, 0.900f, 0.700f, 0.500f, 0.008f, -3.5f, 0.0f, 0.0f, 0, 1, 18.0f, 52.3f, 2, 0.040f, 0.015f, 0, 0.100f, 7.5f, 500.0f }, "Short" }, // fit 13.2 to SVEKick03S.wav
    { "Driven Click D#1",          "Techno",            { -6.0f, 0.255f, 0.400f, 0.600f, 0.800f, 0.600f, 0.300f, 0.087f, -0.2f, 0.0f, 0.0f, 0, 1, 36.4f, 63.1f, 1, 0.100f, 0.389f, 0, 0.000f, 9.2f, 500.0f }, "Short" }, // fit 12.0 to OBI Punchy Kick 13.wav
    { "Driven Knock B1",           "Techno",            { 2.0f, 0.237f, 0.594f, 0.900f, 0.000f, 0.700f, 0.500f, 0.007f, -2.3f, 0.0f, 0.0f, 0, 1, 10.0f, 41.4f, 0, 0.256f, 0.000f, 0, 1.000f, 8.9f, 500.0f }, "Short" }, // fit 16.5 to SVEKick05S.wav
    { "Driven Knock G1",           "Techno",            { -2.5f, 0.266f, 0.769f, 0.839f, 0.647f, 0.687f, 0.382f, 0.000f, -3.3f, 20.0f, 0.0f, 0, 4, 27.2f, 70.0f, 1, 0.253f, 0.420f, 0, 0.200f, 8.1f, 1000.0f }, "Short" }, // fit 12.5 to OBI Punchy Kick 3.wav
    { "Warm Clang C#1",            "Techno",            { -8.0f, 0.262f, 0.921f, 0.558f, 0.882f, 0.341f, 0.765f, 0.016f, -3.1f, 0.0f, 0.0f, 0, 3, 23.0f, 68.5f, 2, 0.005f, 0.000f, 0, 0.100f, 0.4f, 500.0f }, "Short" }, // fit 15.0 to OBI Punchy Kick 20.wav
    { "Warm Clang F#1",            "Techno",            { -3.1f, 0.303f, 1.000f, 0.782f, 0.957f, 0.449f, 0.000f, 0.011f, -3.4f, 0.0f, 0.0f, 0, 1, 22.3f, 55.8f, 2, 0.000f, 0.000f, 0, 0.974f, 7.4f, 500.0f }, "Short" }, // fit 15.3 to SVEKick02S.wav
    { "Clipped Punch A1",          "Techno",            { -0.3f, 0.483f, 0.833f, 0.149f, 0.686f, 0.664f, 0.300f, 0.000f, -1.6f, 0.0f, 0.0f, 1, 1, 25.2f, 42.5f, 1, 1.000f, 1.000f, 0, 0.500f, 2.3f, 500.0f }, "Long" }, // fit 15.2 to OBI Punchy Kick 12.wav
    { "Clipped Punch G#1",         "Techno",            { -1.2f, 0.488f, 0.803f, 0.122f, 0.696f, 0.678f, 0.400f, 0.001f, -3.9f, 20.0f, 0.0f, 1, 1, 24.5f, 40.5f, 1, 0.900f, 0.800f, 0, 0.390f, 3.1f, 1000.0f }, "Long" }, // fit 10.7 to SVEKick06S.wav
    { "Crushed Click G1",          "Techno",            { -1.6f, 0.488f, 0.795f, 0.100f, 0.800f, 0.800f, 0.100f, 0.000f, -3.6f, 0.0f, 0.0f, 1, 1, 25.4f, 50.8f, 1, 1.000f, 0.993f, 0, 0.600f, 2.8f, 500.0f }, "Long" }, // fit 12.4 to SVEKick10S.wav
    { "Warm Punch F#1",            "Techno",            { -2.9f, 0.593f, 0.864f, 0.119f, 0.500f, 0.400f, 0.584f, 0.139f, -1.6f, 20.0f, 0.0f, 0, 3, 39.8f, 32.3f, 2, 0.452f, 0.600f, 0, 0.000f, 1.9f, 400.0f }, "Long" }, // fit 11.3 to OBI Punchy Kick 10.wav
    { "Wide Crushed Rumble A#1",   "Techno",            { 0.7f, 0.494f, 0.021f, 0.739f, 0.500f, 0.994f, 1.000f, 0.473f, -3.4f, 60.0f, 0.0f, 1, 0, 36.0f, 41.6f, 2, 0.086f, 0.100f, 0, 0.500f, 2.5f, 250.0f }, "Rumble" }, // fit 9.9 to OBI Punchy Kick 11.wav
    { "Wide Driven Clang G1",      "Techno",            { -2.4f, 0.429f, 0.799f, 0.590f, 1.000f, 0.527f, 0.400f, 0.000f, -4.3f, 60.0f, 0.0f, 0, 1, 29.0f, 38.8f, 2, 0.700f, 0.006f, 0, 0.200f, 6.2f, 1000.0f }, "Wide" }, // fit 13.6 to OBI Punchy Kick 9.wav
    { "Raw Thud",                  "Techno",            { 0.9f, 0.409f, 0.426f, 0.165f, 1.000f, 0.805f, 0.337f, 0.000f, -3.3f, 20.0f, 0.0f, 0, 4, 41.1f, 41.1f, 0, 0.380f, 0.704f, 0, 0.000f, 0.0f, 1000.0f }, "Crushed" }, // fit 12.6 to SVEKick12S.wav
    { "Crushed Click B1",          "Techno",            { 2.4f, 0.362f, 0.388f, 0.194f, 0.900f, 0.900f, 0.300f, 0.000f, -3.2f, 0.0f, 0.0f, 0, 4, 34.5f, 38.6f, 1, 0.248f, 0.677f, 0, 0.400f, 1.6f, 500.0f }, "Crushed" }, // fit 15.9 to SVEKick08S.wav
    { "Crushed Click F#1",         "Techno",            { -3.0f, 0.360f, 0.748f, 0.247f, 1.000f, 0.873f, 0.912f, 0.056f, -1.1f, 0.0f, 0.0f, 1, 1, 44.3f, 30.2f, 0, 0.500f, 0.255f, 0, 0.000f, 0.2f, 500.0f }, "Crushed" }, // fit 11.2 to OBI Punchy Kick 14.wav
    { "Falling Thump",             "Techno",            { -3.8f, 0.378f, 0.903f, 0.644f, 1.000f, 0.600f, 0.400f, 0.100f, -1.2f, 0.0f, 0.0f, 0, 3, 29.7f, 33.5f, 0, 0.773f, 0.000f, 0, 0.000f, 9.0f, 500.0f }, "Driven" }, // fit 11.9 to OBI Punchy Kick 4.wav
    { "Pitch Drop",                "Techno",            { -4.9f, 0.436f, 0.911f, 0.300f, 0.600f, 0.649f, 0.900f, 0.011f, -5.4f, 0.0f, 0.0f, 1, 1, 20.1f, 50.4f, 2, 0.045f, 0.200f, 0, 0.900f, 6.9f, 500.0f }, "Driven" }, // fit 12.9 to SVEKick07S.wav
    { "Clipped Click A#1",         "Techno",            { 0.7f, 0.460f, 0.443f, 0.063f, 0.900f, 0.693f, 0.200f, 0.003f, -3.8f, 20.0f, 0.0f, 1, 4, 39.4f, 39.8f, 0, 0.353f, 0.747f, 0, 0.400f, 0.5f, 1000.0f }, "Driven" }, // fit 13.8 to SVEKick13S.wav
    { "Clipped Punch F1",          "Techno",            { -3.5f, 0.414f, 0.985f, 0.229f, 0.500f, 0.658f, 0.800f, 0.009f, -2.7f, 0.0f, 0.0f, 1, 1, 24.3f, 61.0f, 2, 0.009f, 0.300f, 0, 1.000f, 3.6f, 500.0f }, "Driven" }, // fit 13.0 to SVEKick09S.wav
    { "Driven Clang G#1 II",       "Techno",            { -0.6f, 0.410f, 0.851f, 0.675f, 0.700f, 0.607f, 0.200f, 0.175f, -1.7f, 20.0f, 0.0f, 0, 0, 47.4f, 19.5f, 2, 0.402f, 0.373f, 0, 0.000f, 4.2f, 1600.0f }, "Driven" }, // fit 8.1 to OBI Punchy Kick 8.wav
    { "Driven Click A#1",          "Techno",            { 1.0f, 0.390f, 0.382f, 0.137f, 0.923f, 0.682f, 0.196f, 0.000f, -1.6f, 0.0f, 0.0f, 0, 4, 40.8f, 38.1f, 0, 0.390f, 0.743f, 0, 0.600f, 0.2f, 500.0f }, "Driven" }, // fit 14.1 to SVEKick01S.wav
    { "Classic Punch",             "Techno",            { -3.6f, 0.423f, 0.777f, 0.446f, 0.000f, 0.475f, 0.800f, 0.133f, -2.6f, 0.0f, 0.0f, 1, 1, 39.3f, 32.5f, 0, 0.302f, 0.052f, 0, 0.900f, 0.8f, 500.0f }, "Clean" }, // fit 8.5 to OBI Punchy Kick 18.wav
    { "Ticking Body",              "Techno",            { -0.4f, 0.374f, 0.709f, 0.860f, 0.825f, 0.451f, 0.400f, 0.000f, -3.3f, 20.0f, 0.0f, 0, 1, 33.7f, 52.8f, 2, 0.784f, 0.119f, 0, 0.700f, 0.1f, 1000.0f }, "Clean" }, // fit 9.4 to OBI Punchy Kick 1.wav
    { "Clean Click A1",            "Techno",            { 0.1f, 0.440f, 0.238f, 0.692f, 0.800f, 0.174f, 0.055f, 0.000f, -3.2f, 20.0f, 0.0f, 1, 2, 14.4f, 15.6f, 0, 0.900f, 0.586f, 0, 0.600f, 17.1f, 1600.0f }, "Clean" }, // fit 9.6 to OBI Punchy Kick 6.wav
    { "Clean Click F#1",           "Techno",            { -3.1f, 0.459f, 0.381f, 0.566f, 1.000f, 0.229f, 0.064f, 0.003f, -4.8f, 0.0f, 0.0f, 1, 0, 47.7f, 19.5f, 1, 0.973f, 1.000f, 0, 0.400f, 18.7f, 500.0f }, "Clean" }, // fit 10.9 to OBI Punchy Kick 7.wav
    { "Warm Clang D#1",            "Techno",            { -6.1f, 0.362f, 0.944f, 0.660f, 0.800f, 0.353f, 0.600f, 0.000f, -3.4f, 20.0f, 0.0f, 1, 1, 39.6f, 14.8f, 2, 0.900f, 0.744f, 0, 0.100f, 13.5f, 1600.0f }, "Clean" }, // fit 9.8 to OBI Punchy Kick 17.wav
    { "Warm Thump F1",             "Techno",            { -4.3f, 0.439f, 0.633f, 0.367f, 0.200f, 0.417f, 0.700f, 0.086f, -3.1f, 20.0f, 0.0f, 1, 1, 31.4f, 32.2f, 2, 0.100f, 0.139f, 0, 0.992f, 2.7f, 1600.0f }, "Clean" }, // fit 11.4 to OBI Punchy Kick 15.wav
    // --- Hard Techno (110) ---
    { "Clean Thump A#1",           "Hard Techno",       { 1.3f, 0.320f, 0.000f, 0.500f, 0.000f, 0.000f, 1.000f, 0.000f, -4.0f, 20.0f, 1.0f, 0, 0, 6.0f, 4.0f, 0, 0.550f, 0.645f, 0, 0.338f, 0.0f, 250.0f }, "Short" }, // fit 16.9 to SVE2 Sub 01.wav
    { "Clipped Click F1",          "Hard Techno",       { -3.7f, 0.267f, 0.986f, 0.462f, 0.848f, 0.600f, 0.049f, 0.000f, -3.3f, 20.0f, 0.0f, 1, 2, 13.9f, 73.6f, 1, 0.000f, 0.061f, 0, 1.000f, 15.0f, 1000.0f }, "Short" }, // fit 11.7 to HST1 Short Kick Shot  2.wav
    { "Clipped Knock C#2",         "Hard Techno",       { 4.0f, 0.224f, 0.715f, 0.565f, 0.500f, 0.500f, 0.000f, 0.000f, -3.8f, 20.0f, 3.0f, 1, 0, 30.4f, 13.6f, 1, 0.636f, 0.127f, 0, 0.900f, 20.7f, 1600.0f }, "Short" }, // fit 10.9 to SVE2 Kick 10.1.wav
    { "Crushed Click G1 II",       "Hard Techno",       { -2.4f, 0.287f, 0.571f, 0.641f, 0.970f, 0.766f, 0.000f, 0.000f, -3.4f, 20.0f, 3.0f, 1, 3, 35.5f, 69.0f, 1, 0.850f, 0.074f, 0, 1.000f, 4.5f, 1600.0f }, "Short" }, // fit 13.1 to SVE2 Kick 7.1.wav
    { "Driven Clang F#1",          "Hard Techno",       { -2.5f, 0.264f, 0.622f, 0.809f, 0.800f, 0.500f, 0.500f, 0.000f, -3.3f, 20.0f, 0.0f, 0, 1, 31.9f, 68.9f, 2, 0.300f, 0.366f, 0, 0.700f, 1.1f, 1000.0f }, "Short" }, // fit 10.4 to HST1 Short Kick Shot  12.wav
    { "Driven Clang G1 II",        "Hard Techno",       { -2.3f, 0.278f, 0.606f, 0.696f, 0.652f, 0.590f, 0.500f, 0.000f, -3.6f, 20.0f, 0.0f, 0, 1, 37.2f, 18.2f, 2, 0.810f, 0.027f, 0, 0.300f, 12.1f, 1000.0f }, "Short" }, // fit 11.7 to HST1 Short Kick Shot  11.wav
    { "Driven Click D1",           "Hard Techno",       { -6.7f, 0.187f, 0.871f, 0.854f, 0.862f, 0.500f, 0.500f, 0.000f, -3.3f, 20.0f, 0.0f, 0, 4, 19.0f, 37.7f, 1, 0.400f, 0.138f, 0, 0.900f, 20.6f, 600.0f }, "Short" }, // fit 17.5 to HST1 Short Kick Shot  5.wav
    { "Driven Click F#1",          "Hard Techno",       { -2.8f, 0.270f, 0.687f, 0.800f, 0.800f, 0.700f, 0.300f, 0.000f, -3.8f, 20.0f, 0.0f, 0, 4, 34.2f, 67.1f, 1, 0.000f, 0.071f, 0, 0.600f, 1.3f, 400.0f }, "Short" }, // fit 12.3 to SVE2 Kick 6.1.wav
    { "Driven Click G#1",          "Hard Techno",       { -1.4f, 0.285f, 0.034f, 0.507f, 0.965f, 0.658f, 0.400f, 0.000f, -3.4f, 20.0f, 0.0f, 0, 4, 44.9f, 51.4f, 0, 1.000f, 0.407f, 0, 0.300f, 8.7f, 1000.0f }, "Short" }, // fit 10.3 to SVE2 Kick 5.1.wav
    { "Driven Knock G#1",          "Hard Techno",       { -0.6f, 0.269f, 0.834f, 0.778f, 0.500f, 0.554f, 0.000f, 0.000f, -3.1f, 20.0f, 0.0f, 0, 0, 14.3f, 35.0f, 2, 0.401f, 0.000f, 0, 0.700f, 12.9f, 1000.0f }, "Short" }, // fit 11.8 to HST1 Short Kick Shot  13.wav
    { "Warm Thump F1 III",         "Hard Techno",       { -4.5f, 0.328f, 0.528f, 0.667f, 0.200f, 0.359f, 0.908f, 0.236f, -3.2f, 0.0f, 0.0f, 1, 2, 46.7f, 51.8f, 1, 0.000f, 0.001f, 0, 1.000f, 0.1f, 500.0f }, "Short" }, // fit 10.6 to SVEKick02CC.wav
    { "Wide Clipped Knock F#1",    "Hard Techno",       { -3.5f, 0.313f, 0.129f, 0.600f, 0.300f, 0.551f, 0.400f, 0.065f, -3.6f, 60.0f, 0.0f, 1, 3, 48.0f, 77.3f, 0, 0.200f, 0.032f, 0, 0.900f, 4.8f, 1000.0f }, "Short" }, // fit 15.4 to SVE2 Kick 7.2.wav (kick register)
    { "Wide Clean Click D#1",      "Hard Techno",       { -6.5f, 0.295f, 0.000f, 1.000f, 0.900f, 0.200f, 1.000f, 0.000f, -7.5f, 100.0f, 1.0f, 1, 4, 6.0f, 11.2f, 1, 0.500f, 0.785f, 0, 0.700f, 0.0f, 250.0f }, "Short" }, // fit 35.6 to SVE2 Sub 07.wav
    { "Wide Clipped Click F#1",    "Hard Techno",       { -2.9f, 0.275f, 0.666f, 0.600f, 1.000f, 0.600f, 0.200f, 0.000f, -3.6f, 60.0f, 0.0f, 1, 2, 39.6f, 12.8f, 0, 1.000f, 0.026f, 0, 1.000f, 10.5f, 1000.0f }, "Short" }, // fit 10.0 to HST1 Short Kick Shot  3.wav (kick register)
    { "Wide Driven Clang G1 II",   "Hard Techno",       { -1.6f, 0.261f, 0.443f, 0.700f, 0.993f, 0.700f, 0.000f, 0.000f, -4.1f, 60.0f, 0.0f, 0, 1, 20.5f, 77.3f, 2, 0.032f, 0.031f, 0, 0.900f, 9.0f, 1000.0f }, "Short" }, // fit 17.1 to SVE2 Kick 4.1.wav
    { "Long Clip",                 "Hard Techno",       { -1.6f, 0.509f, 0.813f, 0.075f, 0.700f, 0.700f, 0.400f, 0.000f, -3.4f, 0.0f, 0.0f, 1, 1, 24.6f, 41.3f, 1, 1.000f, 0.993f, 0, 0.400f, 3.2f, 500.0f }, "Long" }, // fit 10.8 to SVEKick04C.wav
    { "Sub Pressure",              "Hard Techno",       { 1.3f, 0.469f, 0.213f, 0.325f, 0.500f, 0.168f, 1.000f, 0.010f, -3.3f, 0.0f, 0.0f, 0, 0, 7.0f, 4.0f, 0, 0.400f, 0.700f, 0, 0.900f, 0.0f, 500.0f }, "Long" }, // fit 17.1 to SVE2 Sub 04.wav
    { "Clipped Click A#1 II",      "Hard Techno",       { 1.0f, 0.513f, 0.200f, 0.108f, 1.000f, 0.586f, 1.000f, 0.001f, -1.5f, 0.0f, 0.0f, 1, 2, 35.4f, 16.5f, 0, 0.100f, 1.000f, 0, 0.000f, 1.7f, 500.0f }, "Long" }, // fit 15.3 to ITHTS Rumble Kick 3.wav
    { "Clipped Click F1 II",       "Hard Techno",       { -4.2f, 0.533f, 0.693f, 0.053f, 0.900f, 0.582f, 0.400f, 0.000f, 0.7f, 0.0f, 0.0f, 1, 0, 27.3f, 45.2f, 1, 0.500f, 1.000f, 0, 0.300f, 9.4f, 500.0f }, "Long" }, // fit 14.3 to SVEKick11CC.wav
    { "Clipped Click G#1",         "Hard Techno",       { -1.4f, 0.498f, 0.739f, 0.300f, 1.000f, 0.600f, 0.900f, 0.000f, -2.1f, 0.0f, 0.0f, 1, 1, 24.1f, 40.7f, 1, 1.000f, 0.966f, 0, 0.100f, 4.1f, 500.0f }, "Long" }, // fit 13.6 to SVEKick06C.wav
    { "Clipped Click G#1 II",      "Hard Techno",       { -1.1f, 0.515f, 0.798f, 0.054f, 0.900f, 0.585f, 0.000f, 0.000f, -3.5f, 0.0f, 0.0f, 1, 2, 24.2f, 38.9f, 1, 1.000f, 0.922f, 0, 1.000f, 3.5f, 500.0f }, "Long" }, // fit 18.4 to SVEKick05C.wav
    { "Clipped Click G1",          "Hard Techno",       { -1.8f, 0.515f, 0.819f, 0.100f, 1.000f, 0.600f, 0.300f, 0.000f, -3.2f, 20.0f, 0.0f, 1, 2, 24.9f, 47.8f, 1, 1.000f, 0.998f, 0, 0.300f, 2.9f, 1000.0f }, "Long" }, // fit 19.5 to ITHTS Rumble Kick 7.wav
    { "Clipped Drop A1",           "Hard Techno",       { -0.5f, 0.523f, 0.918f, 0.051f, 0.500f, 0.657f, 0.454f, 0.001f, 0.2f, 0.0f, 0.0f, 1, 1, 19.9f, 42.8f, 1, 0.600f, 0.800f, 0, 0.200f, 9.4f, 500.0f }, "Long" }, // fit 13.6 to SVEKick15C.wav
    { "Clipped Drop G1",           "Hard Techno",       { -1.5f, 0.469f, 0.619f, 0.211f, 0.600f, 0.600f, 0.455f, 0.001f, -2.7f, 20.0f, 0.0f, 1, 4, 25.9f, 45.6f, 1, 0.500f, 1.000f, 0, 0.500f, 7.9f, 600.0f }, "Long" }, // fit 20.6 to SVE2 Kick 2.2.wav
    { "Clipped Punch A1 II",       "Hard Techno",       { -0.1f, 0.529f, 0.900f, 0.000f, 0.700f, 0.630f, 0.300f, 0.000f, -1.7f, 0.0f, 0.0f, 1, 3, 25.1f, 54.2f, 0, 1.000f, 0.913f, 0, 0.397f, 3.2f, 500.0f }, "Long" }, // fit 15.5 to SVEKick13C.wav
    { "Clipped Punch G#1 II",      "Hard Techno",       { -1.0f, 0.510f, 0.903f, 0.069f, 0.674f, 0.653f, 0.400f, 0.000f, -2.7f, 20.0f, 0.0f, 1, 1, 24.0f, 47.5f, 1, 0.900f, 1.000f, 0, 0.300f, 4.9f, 1000.0f }, "Long" }, // fit 13.2 to SVEKick12C.wav
    { "Clipped Punch G1",          "Hard Techno",       { -1.6f, 0.509f, 0.811f, 0.106f, 0.700f, 0.600f, 0.000f, 0.000f, -3.2f, 0.0f, 0.0f, 1, 1, 24.6f, 41.3f, 1, 1.000f, 0.987f, 0, 0.600f, 3.2f, 500.0f }, "Long" }, // fit 11.0 to SVEKick03C.wav
    { "Clipped Thump G#1",         "Hard Techno",       { -1.2f, 0.505f, 0.787f, 0.100f, 0.600f, 0.700f, 0.300f, 0.000f, -1.4f, 0.0f, 0.0f, 1, 1, 27.0f, 48.7f, 1, 0.900f, 0.982f, 0, 0.800f, 2.5f, 500.0f }, "Long" }, // fit 13.2 to SVEKick09CC.wav
    { "Clipped Thump G1 II",       "Hard Techno",       { -2.2f, 0.498f, 0.734f, 0.205f, 0.300f, 0.513f, 0.500f, 0.000f, -0.9f, 0.0f, 0.0f, 1, 1, 29.0f, 39.6f, 0, 0.700f, 0.969f, 0, 0.290f, 5.1f, 500.0f }, "Long" }, // fit 12.0 to SVEKick04CC.wav
    { "Clipped Thump G1 III",      "Hard Techno",       { -1.6f, 0.496f, 0.500f, 0.100f, 0.600f, 0.680f, 0.200f, 0.000f, -3.6f, 20.0f, 0.0f, 1, 1, 35.4f, 38.8f, 1, 0.600f, 0.903f, 0, 0.900f, 3.1f, 1000.0f }, "Long" }, // fit 12.8 to SVEKick09C.wav
    { "Wide Clipped Boom G#1",     "Hard Techno",       { -0.7f, 0.431f, 0.383f, 0.859f, 0.000f, 0.563f, 0.085f, 0.000f, -3.9f, 90.0f, 0.0f, 1, 0, 37.0f, 51.8f, 1, 0.885f, 0.424f, 0, 0.499f, 5.6f, 1000.0f }, "Wide" }, // fit 12.8 to SVE2 Kick 9.3.wav (kick register)
    { "Crushed Click F#1 II",      "Hard Techno",       { -2.7f, 0.487f, 0.900f, 0.205f, 0.800f, 0.830f, 0.300f, 0.000f, -4.0f, 0.0f, 0.0f, 0, 4, 17.1f, 72.5f, 1, 1.000f, 0.963f, 0, 0.200f, 9.3f, 500.0f }, "Long" }, // fit 15.8 to SVEKick08C.wav
    { "Driven Click G1",           "Hard Techno",       { -1.7f, 0.505f, 0.967f, 0.100f, 0.800f, 0.600f, 0.400f, 0.000f, 0.2f, 0.0f, 0.0f, 0, 2, 24.2f, 49.3f, 0, 1.000f, 0.928f, 0, 0.600f, 4.7f, 500.0f }, "Long" }, // fit 14.9 to SVEKick01C.wav
    { "Wide Warm Boom G#1",        "Hard Techno",       { -0.9f, 0.422f, 0.384f, 0.812f, 0.156f, 0.392f, 0.100f, 0.000f, -3.5f, 59.5f, 0.0f, 1, 3, 35.6f, 57.1f, 2, 0.883f, 0.301f, 0, 0.800f, 5.7f, 1000.0f }, "Wide" }, // fit 16.2 to SVE2 Kick 7.3.wav (kick register)
    { "Warm Click E1",             "Hard Techno",       { -5.2f, 0.469f, 0.872f, 0.400f, 1.000f, 0.400f, 1.000f, 0.000f, -2.3f, 0.0f, 0.0f, 0, 1, 21.0f, 32.2f, 0, 1.000f, 0.681f, 0, 0.700f, 6.1f, 500.0f }, "Long" }, // fit 11.5 to ITHTS Rumble Kick 10.wav
    { "Wide Crushed Punch F#1",    "Hard Techno",       { -3.1f, 0.506f, 0.966f, 0.018f, 0.500f, 0.796f, 0.024f, 0.000f, -2.7f, 100.0f, 0.0f, 1, 1, 27.8f, 69.7f, 1, 0.200f, 0.711f, 0, 0.600f, 1.8f, 1000.0f }, "Long" }, // fit 15.9 to SVE2 Kick 5.2.wav
    { "Rumble Tail",               "Hard Techno",       { -0.4f, 0.306f, 0.998f, 0.318f, 1.000f, 0.506f, 0.934f, 0.656f, -1.0f, 0.0f, 0.0f, 1, 0, 38.5f, 17.9f, 0, 1.000f, 0.000f, 0, 1.000f, 0.0f, 500.0f }, "Rumble" }, // fit 15.4 to HST1 Rumble Kick Shot 31.wav
    { "Clipped Rumble A#1",        "Hard Techno",       { 0.6f, 0.393f, 0.750f, 0.344f, 0.700f, 0.550f, 0.000f, 0.818f, -2.6f, 0.0f, 0.0f, 1, 4, 29.8f, 32.7f, 0, 0.424f, 0.000f, 0, 0.652f, 0.0f, 500.0f }, "Rumble" }, // fit 9.7 to OBI Hard Rumble Kick 11.wav
    { "Clipped Rumble A#1 II",     "Hard Techno",       { 0.8f, 0.397f, 0.832f, 0.501f, 0.800f, 0.500f, 0.600f, 0.600f, -0.8f, 0.0f, 0.0f, 1, 0, 33.8f, 38.2f, 2, 0.200f, 0.030f, 0, 0.600f, 2.0f, 500.0f }, "Rumble" }, // fit 11.6 to OBI Hard Rumble Kick 2.wav
    { "Clipped Rumble A#1 III",    "Hard Techno",       { 0.9f, 0.307f, 0.822f, 0.364f, 1.000f, 0.535f, 0.000f, 0.800f, -2.1f, 0.0f, 0.0f, 1, 0, 37.7f, 17.9f, 0, 0.900f, 0.000f, 0, 0.900f, 1.2f, 500.0f }, "Rumble" }, // fit 19.1 to HST1 Dirty Kick Shot 8.wav
    { "Clipped Rumble A1",         "Hard Techno",       { -0.1f, 0.297f, 0.971f, 0.260f, 1.000f, 0.612f, 0.935f, 0.600f, -0.8f, 0.0f, 0.0f, 1, 0, 40.0f, 12.7f, 0, 0.800f, 0.000f, 0, 0.916f, 0.3f, 500.0f }, "Rumble" }, // fit 15.2 to HST1 Rumble Kick Shot 14.wav
    { "Clipped Rumble A#1 IX",     "Hard Techno",       { 0.8f, 0.416f, 0.752f, 0.355f, 0.800f, 0.638f, 0.005f, 0.800f, -2.9f, 0.0f, 0.0f, 1, 2, 35.2f, 21.0f, 0, 0.400f, 0.075f, 0, 0.600f, 0.3f, 500.0f }, "Rumble" }, // fit 12.1 to OBI Hard Rumble Kick 26.wav (kick register)
    { "Driven Rumble B1",          "Hard Techno",       { 2.5f, 0.275f, 0.700f, 0.430f, 1.000f, 0.727f, 0.900f, 0.435f, 0.7f, 0.0f, 0.0f, 0, 0, 30.6f, 22.5f, 1, 1.000f, 0.000f, 0, 0.300f, 0.8f, 500.0f }, "Rumble" }, // fit 14.8 to OBI Hard Rumble Kick 23.wav
    { "Warm Rumble B1",            "Hard Techno",       { 2.2f, 0.400f, 0.100f, 0.400f, 0.100f, 0.491f, 1.000f, 0.300f, -3.5f, 0.0f, 0.0f, 1, 1, 39.6f, 29.2f, 1, 0.000f, 0.000f, 0, 1.000f, 2.2f, 500.0f }, "Rumble" }, // fit 15.5 to SVEKick10C.wav
    { "Warm Rumble E1",            "Hard Techno",       { -4.9f, 0.351f, 0.786f, 0.381f, 0.300f, 0.341f, 0.947f, 0.460f, -2.9f, 0.0f, 0.0f, 1, 1, 23.1f, 57.6f, 1, 0.000f, 0.000f, 0, 0.800f, 3.3f, 500.0f }, "Rumble" }, // fit 15.8 to ITHTS Rumble Kick 9.wav
    { "Wide Warm Click F1",        "Hard Techno",       { -4.2f, 0.448f, 0.009f, 0.893f, 1.000f, 0.400f, 0.300f, 0.000f, -3.4f, 90.0f, 0.0f, 1, 1, 43.9f, 28.9f, 0, 1.000f, 0.697f, 0, 0.600f, 17.8f, 1000.0f }, "Wide" }, // fit 12.0 to SVE2 Kick 6.3.wav (kick register)
    { "Wide Clean Thump F#1",      "Hard Techno",       { -3.0f, 0.421f, 0.028f, 0.744f, 0.000f, 0.126f, 0.500f, 0.000f, -4.0f, 80.0f, 0.0f, 1, 2, 47.5f, 59.9f, 1, 0.911f, 0.811f, 0, 1.000f, 2.1f, 1600.0f }, "Wide" }, // fit 14.1 to HST1 Rumble Kick Shot 17.wav
    { "Wide Clipped Boom A1",      "Hard Techno",       { 0.1f, 0.414f, 0.645f, 1.000f, 0.500f, 0.600f, 0.200f, 0.000f, -4.4f, 100.0f, 0.0f, 1, 1, 33.6f, 43.7f, 2, 0.600f, 0.567f, 0, 0.900f, 2.7f, 1600.0f }, "Wide" }, // fit 10.3 to HST1 Dirty Kick Shot 15.wav
    { "Wide Clipped Drop G#1",     "Hard Techno",       { -0.7f, 0.432f, 0.386f, 0.828f, 0.073f, 0.594f, 0.131f, 0.000f, -3.7f, 60.0f, 0.0f, 1, 1, 36.5f, 49.0f, 2, 0.913f, 0.447f, 0, 0.500f, 6.1f, 1000.0f }, "Wide" }, // fit 13.3 to SVE2 Kick 9.2.wav
    { "Wide Crushed Clang A#1",    "Hard Techno",       { 0.7f, 0.402f, 0.735f, 1.000f, 0.989f, 1.000f, 0.500f, 0.000f, -4.1f, 80.0f, 0.0f, 1, 3, 32.3f, 50.7f, 2, 0.182f, 0.735f, 0, 1.000f, 3.3f, 1000.0f }, "Wide" }, // fit 25.8 to CORE 09 - E.wav
    { "Wide Warm Boom B1",         "Hard Techno",       { 1.8f, 0.421f, 0.442f, 0.754f, 0.600f, 0.463f, 0.800f, 0.000f, -4.2f, 80.0f, 0.0f, 1, 1, 8.0f, 19.8f, 0, 0.500f, 0.526f, 0, 1.000f, 0.2f, 1600.0f }, "Wide" }, // fit 19.0 to SVE2 Sub 10.wav
    { "Wide Warm Click D1",        "Hard Techno",       { -6.7f, 0.419f, 0.202f, 0.859f, 1.000f, 0.280f, 0.041f, 0.001f, -3.7f, 60.0f, 0.0f, 1, 3, 44.4f, 8.1f, 1, 0.000f, 1.000f, 0, 0.628f, 15.8f, 1000.0f }, "Wide" }, // fit 14.5 to HST1 Dirty Kick Shot 18.wav
    { "Wide Warm Click E1",        "Hard Techno",       { -5.4f, 0.419f, 0.007f, 0.800f, 1.000f, 0.310f, 0.200f, 0.001f, -3.8f, 80.0f, 0.0f, 1, 3, 48.0f, 31.2f, 1, 0.800f, 0.400f, 0, 0.700f, 13.9f, 1000.0f }, "Wide" }, // fit 9.5 to HST1 Dirty Kick Shot 1.wav
    { "Wide Warm Click F#1",       "Hard Techno",       { -2.7f, 0.421f, 0.000f, 0.764f, 1.000f, 0.400f, 0.800f, 0.000f, -3.6f, 80.0f, 0.0f, 1, 1, 44.9f, 24.5f, 1, 0.700f, 0.980f, 0, 0.599f, 14.2f, 1600.0f }, "Wide" }, // fit 10.6 to HST1 Rumble Kick Shot 10.wav
    { "Crushed Boom G#1",          "Hard Techno",       { -0.6f, 0.416f, 0.162f, 0.849f, 0.131f, 0.790f, 0.600f, 0.000f, -3.5f, 0.0f, 0.0f, 1, 1, 42.3f, 56.1f, 2, 0.953f, 1.000f, 0, 0.400f, 2.9f, 500.0f }, "Crushed" }, // fit 12.2 to HST1 Rumble Kick Shot 18.wav
    { "Crushed Drop A1",           "Hard Techno",       { -0.2f, 0.413f, 0.724f, 0.965f, 0.300f, 0.778f, 0.047f, 0.000f, -3.8f, 20.0f, 0.0f, 1, 1, 30.3f, 32.1f, 2, 0.500f, 0.095f, 0, 0.950f, 6.5f, 1000.0f }, "Crushed" }, // fit 13.4 to HST1 Dirty Kick Shot 12.wav
    { "Hard Techno Core",          "Hard Techno",       { -0.7f, 0.421f, 0.471f, 0.742f, 0.141f, 0.599f, 0.300f, 0.001f, -2.9f, 20.0f, 0.0f, 1, 1, 39.8f, 54.7f, 2, 1.000f, 0.500f, 0, 0.500f, 1.4f, 1000.0f }, "Driven" }, // fit 9.8 to HST1 Rumble Kick Shot 25.wav
    { "Clipped Boom A1",           "Hard Techno",       { -0.5f, 0.414f, 0.393f, 0.979f, 0.100f, 0.507f, 0.100f, 0.000f, -4.3f, 40.0f, 0.0f, 1, 2, 39.3f, 66.7f, 2, 0.600f, 0.100f, 0, 0.700f, 0.0f, 400.0f }, "Driven" }, // fit 12.0 to HST1 Rumble Kick Shot 20.wav
    { "Clipped Boom C2",           "Hard Techno",       { 2.7f, 0.415f, 0.862f, 1.000f, 0.300f, 0.741f, 0.300f, 0.003f, -4.6f, 0.0f, 0.0f, 1, 1, 20.7f, 12.5f, 2, 0.300f, 0.100f, 0, 0.985f, 0.0f, 500.0f }, "Driven" }, // fit 12.1 to HST1 Dirty Kick Shot 7.wav
    { "Clipped Boom G1",           "Hard Techno",       { -2.1f, 0.437f, 0.544f, 0.843f, 0.244f, 0.600f, 0.273f, 0.000f, -3.5f, 20.0f, 0.0f, 1, 1, 38.1f, 48.7f, 2, 0.200f, 0.684f, 0, 0.700f, 2.7f, 1000.0f }, "Driven" }, // fit 15.0 to ITHTS Rumble Kick 2.wav
    { "Clipped Clang C2",          "Hard Techno",       { 3.0f, 0.430f, 1.000f, 0.928f, 1.000f, 0.686f, 0.100f, 0.000f, -4.8f, 40.0f, 0.0f, 1, 0, 34.1f, 42.8f, 2, 0.100f, 0.578f, 0, 0.800f, 2.0f, 1000.0f }, "Driven" }, // fit 15.4 to SVE2 Kick 1.2.wav
    { "Clipped Click F#1",         "Hard Techno",       { -3.4f, 0.417f, 0.018f, 0.908f, 0.834f, 0.518f, 0.219f, 0.001f, -2.9f, 20.0f, 0.0f, 1, 1, 46.1f, 30.2f, 1, 0.900f, 0.800f, 0, 0.200f, 12.6f, 1000.0f }, "Driven" }, // fit 10.6 to HST1 Rumble Kick Shot 4.wav
    { "Clipped Click F#1 II",      "Hard Techno",       { -2.9f, 0.422f, 0.032f, 0.800f, 1.000f, 0.600f, 0.000f, 0.000f, -3.2f, 20.0f, 0.0f, 1, 0, 46.9f, 29.0f, 1, 1.000f, 0.865f, 0, 0.200f, 13.1f, 1600.0f }, "Driven" }, // fit 11.2 to HST1 Rumble Kick Shot 22.wav
    { "Clipped Click G#1 III",     "Hard Techno",       { -1.4f, 0.435f, 0.204f, 0.180f, 1.000f, 0.700f, 0.000f, 0.000f, -3.1f, 40.0f, 0.0f, 1, 4, 41.3f, 52.1f, 0, 0.200f, 0.751f, 0, 0.400f, 4.4f, 1000.0f }, "Driven" }, // fit 20.4 to SVE2 Kick 2.1.wav
    { "Clipped Click G1 II",       "Hard Techno",       { -2.3f, 0.420f, 0.220f, 0.866f, 1.000f, 0.500f, 0.200f, 0.000f, -3.3f, 20.0f, 0.0f, 1, 1, 44.9f, 9.6f, 1, 0.200f, 0.845f, 0, 0.762f, 15.0f, 1000.0f }, "Driven" }, // fit 14.0 to HST1 Rumble Kick Shot 39.wav
    { "Clipped Thump A1",          "Hard Techno",       { -0.3f, 0.382f, 0.666f, 0.354f, 0.400f, 0.582f, 0.900f, 0.240f, -3.9f, 0.0f, 0.0f, 1, 1, 33.7f, 47.7f, 2, 0.690f, 0.034f, 0, 0.700f, 0.7f, 500.0f }, "Driven" }, // fit 14.0 to SVEKick14C.wav
    { "Clipped Thump G1",          "Hard Techno",       { -1.7f, 0.354f, 0.707f, 0.581f, 0.500f, 0.500f, 0.200f, 0.000f, -4.3f, 0.0f, 0.0f, 1, 2, 33.4f, 55.5f, 2, 0.655f, 0.030f, 0, 1.000f, 2.2f, 500.0f }, "Driven" }, // fit 9.0 to HST1 Short Kick Shot  6.wav
    { "Driven Boom B1",            "Hard Techno",       { 2.2f, 0.357f, 0.729f, 0.858f, 0.500f, 0.576f, 0.400f, 0.000f, -3.9f, 20.0f, 0.0f, 0, 1, 28.0f, 29.0f, 2, 0.642f, 0.181f, 0, 0.600f, 0.0f, 600.0f }, "Driven" }, // fit 15.7 to SVE2 Kick 3.1.wav
    { "Driven Click A1",           "Hard Techno",       { 0.4f, 0.390f, 0.417f, 0.289f, 0.956f, 0.624f, 0.223f, 0.000f, -2.6f, 20.0f, 0.0f, 0, 4, 38.4f, 37.9f, 0, 0.497f, 0.808f, 0, 0.057f, 1.6f, 1000.0f }, "Driven" }, // fit 12.4 to HST1 Short Kick Shot  9.wav
    { "Driven Punch B1",           "Hard Techno",       { 2.2f, 0.426f, 0.996f, 0.700f, 0.570f, 0.500f, 0.500f, 0.000f, -5.2f, 40.0f, 0.0f, 0, 1, 48.0f, 23.4f, 1, 0.900f, 0.376f, 0, 0.400f, 3.3f, 400.0f }, "Driven" }, // fit 16.1 to HST1 Rumble Kick Shot 37.wav
    { "Deep Drop",                 "Hard Techno",       { -2.0f, 0.461f, 0.888f, 0.727f, 0.900f, 0.253f, 0.300f, 0.000f, -3.3f, 0.0f, 0.0f, 1, 0, 34.9f, 26.8f, 1, 1.000f, 0.000f, 0, 0.700f, 8.2f, 500.0f }, "Clean" }, // fit 9.9 to OBI Hard Rumble Kick 21.wav
    { "Falling Clipper",           "Hard Techno",       { -2.7f, 0.425f, 0.012f, 0.783f, 1.000f, 0.261f, 0.089f, 0.000f, -4.1f, 20.0f, 0.0f, 1, 1, 47.3f, 29.9f, 1, 1.000f, 0.896f, 0, 0.490f, 14.0f, 1600.0f }, "Clean" }, // fit 10.5 to OBI Hard Rumble Kick 24.wav
    { "Clean Boom F1",             "Hard Techno",       { -3.7f, 0.429f, 0.100f, 0.800f, 0.000f, 0.014f, 1.000f, 0.000f, -4.5f, 0.0f, 0.0f, 1, 0, 6.0f, 4.0f, 0, 0.044f, 0.611f, 0, 0.000f, 0.2f, 500.0f }, "Clean" }, // fit 15.4 to SVE2 Sub 03.wav
    { "Warm Clang A1 II",          "Hard Techno",       { 0.4f, 0.419f, 0.893f, 0.948f, 0.955f, 0.380f, 0.500f, 0.000f, -3.7f, 0.0f, 0.0f, 0, 1, 30.4f, 62.3f, 2, 0.500f, 0.000f, 0, 0.200f, 0.0f, 1000.0f }, "Clean" }, // fit 11.9 to BORKICK 07.wav (kick register)
    { "Clean Clang F1 II",         "Hard Techno",       { -4.0f, 0.425f, 0.000f, 0.800f, 0.900f, 0.200f, 0.300f, 0.000f, -6.1f, 0.0f, 0.0f, 1, 1, 39.6f, 50.0f, 2, 0.800f, 0.992f, 0, 1.000f, 6.9f, 500.0f }, "Clean" }, // fit 10.5 to HST1 Rumble Kick Shot 27.wav
    { "Clean Clang G#1",           "Hard Techno",       { -0.7f, 0.432f, 0.000f, 0.746f, 0.900f, 0.100f, 0.300f, 0.000f, -3.1f, 0.0f, 0.0f, 1, 0, 44.2f, 18.9f, 2, 0.200f, 0.980f, 0, 0.900f, 10.4f, 500.0f }, "Clean" }, // fit 15.5 to OBI Hard Rumble Kick 22.wav
    { "Clean Clang G1",            "Hard Techno",       { -2.3f, 0.420f, 0.220f, 0.857f, 0.781f, 0.100f, 0.400f, 0.000f, -4.4f, 20.0f, 0.0f, 1, 3, 48.0f, 22.6f, 2, 0.400f, 0.945f, 0, 0.000f, 12.6f, 1600.0f }, "Clean" }, // fit 10.7 to HST1 Rumble Kick Shot 40.wav
    { "Clean Clang G1 II",         "Hard Techno",       { -2.3f, 0.420f, 0.032f, 0.800f, 0.600f, 0.200f, 0.000f, 0.000f, -3.7f, 20.0f, 0.0f, 1, 0, 44.6f, 25.5f, 2, 0.459f, 0.200f, 0, 0.300f, 13.4f, 1000.0f }, "Clean" }, // fit 10.3 to HST1 Rumble Kick Shot 24.wav
    { "Clean Click A1 II",         "Hard Techno",       { -0.1f, 0.435f, 0.184f, 0.673f, 1.000f, 0.209f, 0.200f, 0.000f, -3.9f, 20.0f, 0.0f, 1, 2, 23.7f, 40.1f, 1, 0.992f, 0.856f, 0, 0.500f, 16.3f, 1600.0f }, "Clean" }, // fit 14.8 to BORKICK 10.wav
    { "Clean Click D#1",           "Hard Techno",       { -6.5f, 0.425f, 0.138f, 0.769f, 1.000f, 0.244f, 0.100f, 0.000f, -3.6f, 0.0f, 0.0f, 1, 3, 42.8f, 17.9f, 1, 0.400f, 0.532f, 0, 0.998f, 13.6f, 500.0f }, "Clean" }, // fit 10.5 to HST1 Dirty Kick Shot 6.wav
    { "Clean Click F#1 II",        "Hard Techno",       { -3.1f, 0.425f, 0.018f, 0.823f, 1.000f, 0.233f, 0.900f, 0.000f, -3.5f, 0.0f, 0.0f, 1, 1, 46.2f, 32.5f, 0, 0.800f, 0.828f, 0, 0.511f, 15.3f, 500.0f }, "Clean" }, // fit 14.4 to BORKICK 15.wav
    { "Clean Click F1",            "Hard Techno",       { -3.8f, 0.461f, 0.817f, 0.937f, 1.000f, 0.200f, 0.100f, 0.010f, -3.1f, 20.0f, 0.0f, 1, 0, 30.6f, 43.4f, 0, 0.800f, 0.153f, 0, 0.700f, 7.4f, 1000.0f }, "Clean" }, // fit 11.4 to OBI Hard Rumble Kick 4.wav
    { "Clean Click F1 II",         "Hard Techno",       { -3.6f, 0.444f, 0.102f, 0.886f, 1.000f, 0.137f, 0.700f, 0.000f, -4.2f, 20.0f, 0.0f, 1, 1, 29.3f, 20.5f, 0, 1.000f, 0.939f, 0, 1.000f, 19.6f, 600.0f }, "Clean" }, // fit 9.8 to ITHTS Rumble Kick 1.wav
    { "Clean Click G#1",           "Hard Techno",       { -1.3f, 0.432f, 0.006f, 0.705f, 1.000f, 0.247f, 0.133f, 0.000f, -3.2f, 40.0f, 0.0f, 1, 0, 42.6f, 31.5f, 1, 0.300f, 0.979f, 0, 0.552f, 14.5f, 1000.0f }, "Clean" }, // fit 14.7 to SVE2 Kick 10.2.wav
    { "Clean Click G#1 II",        "Hard Techno",       { -0.7f, 0.419f, 0.218f, 0.900f, 1.000f, 0.142f, 1.000f, 0.002f, -4.0f, 0.0f, 0.0f, 1, 1, 43.8f, 22.2f, 0, 0.900f, 1.000f, 0, 0.800f, 14.4f, 500.0f }, "Clean" }, // fit 18.3 to BORKICK 12.wav
    { "Clean Click G#1 III",       "Hard Techno",       { -0.6f, 0.430f, 0.595f, 0.747f, 0.900f, 0.193f, 0.045f, 0.000f, -5.1f, 0.0f, 0.0f, 1, 2, 46.5f, 4.0f, 1, 1.000f, 0.650f, 0, 0.363f, 23.9f, 500.0f }, "Clean" }, // fit 17.3 to BORKICK 11.wav
    { "Clean Click G1",            "Hard Techno",       { -2.1f, 0.420f, 0.000f, 0.862f, 0.800f, 0.227f, 0.200f, 0.000f, -4.3f, 20.0f, 0.0f, 1, 2, 47.3f, 33.8f, 0, 1.000f, 0.714f, 0, 0.235f, 13.2f, 1600.0f }, "Clean" }, // fit 12.3 to OBI Hard Rumble Kick 29.wav
    { "Clean Click G1 II",         "Hard Techno",       { -2.2f, 0.431f, 0.041f, 0.672f, 1.000f, 0.211f, 0.500f, 0.000f, -4.4f, 20.0f, 0.0f, 1, 1, 44.6f, 34.4f, 0, 1.000f, 0.781f, 0, 1.000f, 10.3f, 250.0f }, "Clean" }, // fit 16.1 to SVE2 Kick 3.3.wav
    { "Clean Click G1 III",        "Hard Techno",       { -1.8f, 0.426f, 0.046f, 0.913f, 1.000f, 0.221f, 0.500f, 0.003f, -3.3f, 0.0f, 0.0f, 1, 1, 48.0f, 31.1f, 1, 0.800f, 1.000f, 0, 0.300f, 14.7f, 500.0f }, "Clean" }, // fit 11.4 to OBI Hard Rumble Kick 8.wav
    { "Clean Click G1 IV",         "Hard Techno",       { -1.7f, 0.430f, 0.991f, 0.810f, 1.000f, 0.249f, 0.100f, 0.200f, -3.2f, 20.0f, 0.0f, 1, 0, 35.1f, 25.1f, 0, 1.000f, 0.000f, 0, 0.700f, 8.8f, 1000.0f }, "Clean" }, // fit 9.8 to OBI Hard Rumble Kick 5.wav
    { "Clean Drop A1",             "Hard Techno",       { 0.5f, 0.443f, 0.915f, 0.751f, 0.000f, 0.000f, 0.500f, 0.000f, -5.1f, 0.0f, 0.0f, 0, 1, 44.5f, 12.4f, 0, 0.400f, 0.437f, 0, 0.600f, 8.8f, 500.0f }, "Clean" }, // fit 12.9 to ITHTS Rumble Kick 11.wav
    { "Clean Drop D1",             "Hard Techno",       { -7.1f, 0.431f, 0.764f, 0.475f, 0.400f, 0.244f, 0.600f, 0.000f, -3.1f, 20.0f, 0.0f, 1, 1, 14.4f, 20.2f, 2, 0.100f, 0.990f, 0, 0.300f, 9.5f, 400.0f }, "Clean" }, // fit 10.9 to HST1 Short Kick Shot  14.wav
    { "Clean Drop F1",             "Hard Techno",       { -3.5f, 0.398f, 0.192f, 1.000f, 0.700f, 0.100f, 0.600f, 0.000f, -3.5f, 0.0f, 0.0f, 1, 1, 31.2f, 15.6f, 0, 0.900f, 0.942f, 0, 0.462f, 14.2f, 500.0f }, "Clean" }, // fit 12.9 to HST1 Short Kick Shot  18.wav
    { "Clean Drop A1 II",          "Hard Techno",       { 0.1f, 0.409f, 0.259f, 1.000f, 0.300f, 0.115f, 1.000f, 0.000f, -4.4f, 0.0f, 0.0f, 1, 1, 22.8f, 27.2f, 2, 0.000f, 0.905f, 0, 0.900f, 16.6f, 500.0f }, "Clean" }, // fit 14.6 to BORKICK 14.wav (kick register)
    { "Warm Clang G1 V",           "Hard Techno",       { -2.1f, 0.418f, 0.866f, 0.960f, 0.743f, 0.344f, 0.000f, 0.000f, -3.0f, 0.0f, 0.0f, 0, 1, 30.6f, 83.0f, 2, 0.803f, 0.636f, 0, 0.300f, 3.6f, 1600.0f }, "Clean" }, // fit 15.4 to BORKICK 01.wav (kick register)
    { "Warm Clang F1",             "Hard Techno",       { -3.7f, 0.462f, 0.933f, 0.837f, 1.000f, 0.258f, 0.000f, 0.000f, -6.8f, 0.0f, 0.0f, 1, 3, 47.4f, 17.7f, 2, 0.000f, 0.100f, 0, 0.900f, 7.2f, 500.0f }, "Clean" }, // fit 12.2 to OBI Hard Rumble Kick 27.wav
    { "Warm Clang G1",             "Hard Techno",       { -1.6f, 0.424f, 0.180f, 0.715f, 0.867f, 0.400f, 0.000f, 0.000f, -4.8f, 0.0f, 0.0f, 1, 2, 39.6f, 29.0f, 2, 0.270f, 0.884f, 0, 0.200f, 14.4f, 500.0f }, "Clean" }, // fit 11.0 to BORKICK 03.wav
    { "Warm Clang G1 II",          "Hard Techno",       { -2.0f, 0.424f, 0.009f, 0.741f, 1.000f, 0.307f, 0.002f, 0.000f, -5.0f, 0.0f, 0.0f, 1, 2, 47.3f, 22.9f, 2, 0.000f, 0.600f, 0, 0.000f, 14.0f, 500.0f }, "Clean" }, // fit 13.8 to HST1 Rumble Kick Shot 30.wav
    { "Warm Click A1",             "Hard Techno",       { -0.1f, 0.426f, 0.600f, 0.541f, 0.800f, 0.257f, 0.900f, 0.000f, -3.2f, 20.0f, 0.0f, 1, 2, 10.2f, 32.7f, 0, 1.000f, 0.277f, 0, 0.000f, 2.4f, 250.0f }, "Clean" }, // fit 12.8 to HST1 Short Kick Shot  19.wav
    { "Warm Click F#1",            "Hard Techno",       { -2.7f, 0.430f, 0.017f, 0.931f, 1.000f, 0.400f, 0.000f, 0.000f, -3.2f, 20.0f, 0.0f, 1, 1, 36.3f, 37.8f, 1, 1.000f, 0.990f, 0, 0.530f, 14.3f, 1000.0f }, "Clean" }, // fit 16.2 to SVE2 Kick 8.2.wav
    { "Warm Click F#1 II",         "Hard Techno",       { -2.9f, 0.440f, 0.000f, 0.715f, 1.000f, 0.374f, 0.200f, 0.000f, -3.4f, 20.0f, 0.0f, 1, 1, 46.3f, 12.3f, 1, 0.900f, 0.895f, 0, 0.592f, 15.7f, 1600.0f }, "Clean" }, // fit 15.4 to ITHTS Rumble Kick 6.wav
    { "Warm Click F#1 III",        "Hard Techno",       { -2.7f, 0.421f, 0.021f, 0.936f, 1.000f, 0.300f, 0.000f, 0.000f, -4.3f, 20.0f, 0.0f, 1, 0, 42.9f, 32.5f, 0, 0.700f, 0.995f, 0, 0.000f, 12.3f, 1600.0f }, "Clean" }, // fit 13.7 to HST1 Rumble Kick Shot 15.wav
    { "Warm Click F#1 IV",         "Hard Techno",       { -2.6f, 0.419f, 0.064f, 0.862f, 0.947f, 0.329f, 0.073f, 0.000f, -3.3f, 20.0f, 0.0f, 1, 0, 47.3f, 22.9f, 1, 0.800f, 0.888f, 0, 0.518f, 19.2f, 600.0f }, "Clean" }, // fit 12.1 to BORKICK 13.wav
    { "Warm Click F1",             "Hard Techno",       { -3.8f, 0.421f, 0.106f, 0.800f, 1.000f, 0.371f, 0.100f, 0.000f, -3.8f, 20.0f, 0.0f, 1, 4, 46.3f, 39.0f, 1, 1.000f, 0.969f, 0, 0.400f, 11.5f, 1600.0f }, "Clean" }, // fit 16.5 to OBI Hard Rumble Kick 18.wav
    { "Warm Click F1 II",          "Hard Techno",       { -3.6f, 0.435f, 0.045f, 0.694f, 1.000f, 0.256f, 0.300f, 0.000f, -3.6f, 0.0f, 0.0f, 1, 2, 33.1f, 41.6f, 0, 1.000f, 0.969f, 0, 0.815f, 14.6f, 500.0f }, "Clean" }, // fit 13.8 to BORKICK 08.wav
    { "Warm Click F1 III",         "Hard Techno",       { -4.3f, 0.436f, 0.000f, 0.918f, 0.900f, 0.377f, 0.300f, 0.000f, -3.2f, 40.0f, 0.0f, 1, 1, 44.0f, 29.7f, 0, 1.000f, 0.757f, 0, 0.554f, 17.8f, 1000.0f }, "Clean" }, // fit 11.9 to SVE2 Kick 6.2.wav
    { "Warm Drop F#1",             "Hard Techno",       { -3.2f, 0.412f, 0.028f, 0.937f, 0.700f, 0.462f, 0.300f, 0.000f, -3.2f, 0.0f, 0.0f, 1, 1, 47.3f, 39.3f, 1, 0.400f, 0.742f, 0, 0.300f, 8.1f, 500.0f }, "Clean" }, // fit 8.9 to HST1 Rumble Kick Shot 11.wav
    { "Warm Drop F#1 II",          "Hard Techno",       { -3.0f, 0.416f, 0.032f, 0.849f, 0.400f, 0.448f, 0.100f, 0.000f, -4.0f, 20.0f, 0.0f, 1, 3, 47.6f, 16.8f, 1, 1.000f, 0.968f, 0, 0.800f, 14.5f, 1600.0f }, "Clean" }, // fit 11.0 to HST1 Dirty Kick Shot 10.wav
    { "Warm Thump A#1",            "Hard Techno",       { 1.4f, 0.405f, 0.000f, 0.564f, 0.200f, 0.488f, 1.000f, 0.006f, -4.2f, 40.0f, 3.0f, 1, 0, 6.0f, 4.0f, 1, 0.000f, 0.000f, 0, 0.900f, 0.0f, 400.0f }, "Clean" }, // fit 22.1 to SVE2 Sub 08.wav
    { "Warm Thump F1 II",          "Hard Techno",       { -4.0f, 0.365f, 0.379f, 0.400f, 0.000f, 0.412f, 0.700f, 0.261f, -3.0f, 0.0f, 0.0f, 1, 2, 38.0f, 37.0f, 0, 0.302f, 0.000f, 0, 1.000f, 4.9f, 500.0f }, "Clean" }, // fit 11.5 to SVEKick02C.wav
    // --- Schranz (100) ---
    { "Clipped Boom A#1",          "Schranz",           { 1.1f, 0.339f, 0.538f, 1.000f, 0.500f, 0.500f, 0.700f, 0.000f, -4.1f, 20.0f, 3.0f, 1, 1, 40.3f, 40.1f, 2, 0.920f, 0.226f, 0, 0.676f, 0.5f, 250.0f }, "Short" }, // fit 12.5 to SSP Signature Schranz Kick 52.wav
    { "Clipped Clang A1",          "Schranz",           { -0.5f, 0.328f, 0.431f, 0.511f, 0.600f, 0.641f, 1.000f, 0.200f, -3.7f, 0.0f, 0.0f, 1, 1, 45.3f, 40.8f, 2, 0.470f, 0.000f, 0, 0.900f, 0.6f, 500.0f }, "Short" }, // fit 12.9 to ARK Kick Drum Shot 15.wav
    { "Crushed Clang A1",          "Schranz",           { -0.2f, 0.312f, 0.500f, 0.625f, 0.969f, 0.815f, 0.800f, 0.063f, -5.3f, 0.0f, 0.0f, 1, 1, 47.2f, 29.0f, 2, 0.788f, 0.000f, 0, 0.800f, 3.7f, 500.0f }, "Short" }, // fit 11.8 to ARK Kick Drum Shot 5.wav
    { "Crushed Clang G1",          "Schranz",           { -1.8f, 0.273f, 0.270f, 0.601f, 1.000f, 0.800f, 0.169f, 0.000f, -4.7f, 20.0f, 0.0f, 1, 1, 23.4f, 51.7f, 2, 0.500f, 0.283f, 0, 0.932f, 9.5f, 1000.0f }, "Short" }, // fit 11.6 to ARK Kick Drum Shot 23.wav
    { "Crushed Knock G1",          "Schranz",           { -2.3f, 0.262f, 1.000f, 0.852f, 0.700f, 0.890f, 0.200f, 0.000f, -4.3f, 0.0f, 0.0f, 0, 4, 18.4f, 46.3f, 1, 0.100f, 0.035f, 0, 0.200f, 6.0f, 500.0f }, "Short" }, // fit 9.8 to ARK Kick Drum Shot 3.wav
    { "Crushed Thump F#1",         "Schranz",           { -3.4f, 0.489f, 0.696f, 0.063f, 0.500f, 0.800f, 0.900f, 0.002f, -4.5f, 20.0f, 0.0f, 1, 1, 29.8f, 83.2f, 2, 0.744f, 0.700f, 0, 0.900f, 2.3f, 1600.0f }, "Long" }, // fit 15.0 to ARK Kick Drum Shot 11.wav
    { "Classic Schranz",           "Schranz",           { 0.4f, 0.358f, 0.528f, 0.440f, 0.600f, 0.610f, 0.700f, 0.377f, -4.7f, 0.0f, 0.0f, 1, 1, 41.6f, 31.0f, 2, 0.629f, 0.000f, 0, 0.900f, 0.2f, 500.0f }, "Rumble" }, // fit 13.0 to ARK Kick Drum Shot 19.wav
    { "Crushed Boom A1",           "Schranz",           { -0.2f, 0.419f, 0.186f, 0.806f, 0.000f, 0.804f, 0.900f, 0.000f, -3.3f, 0.0f, 0.0f, 1, 1, 42.0f, 50.2f, 1, 0.926f, 0.989f, 0, 0.500f, 2.6f, 500.0f }, "Crushed" }, // fit 13.8 to ARK Kick Drum Shot 22.wav (kick register)
    { "Clipped Rumble A1 II",      "Schranz",           { 0.4f, 0.421f, 0.962f, 0.353f, 1.000f, 0.500f, 0.300f, 0.900f, -1.9f, 0.0f, 0.0f, 1, 3, 35.0f, 36.7f, 1, 0.700f, 0.000f, 0, 0.100f, 0.6f, 500.0f }, "Rumble" }, // fit 12.7 to OBI Driven Schranz Kick 1.wav
    { "Clipped Rumble A1 III",     "Schranz",           { -0.3f, 0.298f, 0.352f, 0.412f, 0.340f, 0.593f, 0.000f, 0.334f, -3.1f, 0.0f, 0.0f, 1, 3, 41.3f, 38.2f, 2, 0.700f, 0.000f, 0, 0.994f, 0.9f, 500.0f }, "Rumble" }, // fit 13.8 to ARK Kick Drum Shot 14.wav
    { "Warm Rumble G1",            "Schranz",           { -1.5f, 0.326f, 0.500f, 0.461f, 0.500f, 0.458f, 0.600f, 0.316f, -4.1f, 0.0f, 0.0f, 1, 1, 36.6f, 51.6f, 2, 0.938f, 0.000f, 0, 1.000f, 2.9f, 500.0f }, "Rumble" }, // fit 13.0 to ARK Kick Drum Shot 16.wav
    { "Schranz Air",               "Schranz",           { 2.3f, 0.427f, 0.999f, 0.700f, 0.619f, 0.400f, 0.600f, 0.000f, -5.4f, 60.0f, 0.0f, 0, 1, 47.2f, 25.2f, 2, 0.532f, 0.372f, 0, 0.700f, 3.3f, 1000.0f }, "Wide" }, // fit 13.5 to SSP Signature Schranz Kick 38.wav
    { "Wide Clean Boom A1",        "Schranz",           { -0.2f, 0.408f, 0.655f, 1.000f, 0.500f, 0.244f, 0.900f, 0.000f, -4.2f, 60.0f, 0.0f, 1, 1, 37.9f, 48.1f, 2, 0.988f, 0.252f, 0, 0.488f, 5.6f, 400.0f }, "Wide" }, // fit 14.7 to MS Signature Kick Shot 13.wav
    { "Wide Crushed Clang B1",     "Schranz",           { 2.3f, 0.403f, 0.767f, 0.900f, 0.998f, 0.800f, 0.005f, 0.000f, -4.1f, 60.0f, 0.0f, 1, 2, 32.0f, 49.1f, 2, 0.400f, 0.563f, 0, 0.300f, 2.8f, 1600.0f }, "Wide" }, // fit 19.5 to SSP Signature Schranz Kick 53.wav
    { "Wide Driven Clang B1",      "Schranz",           { 1.6f, 0.409f, 0.904f, 0.900f, 0.769f, 0.600f, 0.400f, 0.007f, -4.5f, 100.0f, 0.0f, 0, 1, 38.9f, 37.2f, 2, 0.766f, 0.000f, 0, 0.200f, 0.0f, 1600.0f }, "Wide" }, // fit 13.4 to SSP Signature Schranz Kick 79.wav
    { "Wide Driven Punch A#1",     "Schranz",           { 1.0f, 0.421f, 0.936f, 0.719f, 0.500f, 0.500f, 0.400f, 0.000f, -3.6f, 80.0f, 0.0f, 0, 4, 45.9f, 30.4f, 2, 0.986f, 0.132f, 0, 0.300f, 2.4f, 1600.0f }, "Wide" }, // fit 12.1 to SSP Signature Schranz Kick 11.wav
    { "Wide Warm Clang A#1",       "Schranz",           { 0.7f, 0.421f, 0.903f, 0.900f, 0.600f, 0.300f, 0.500f, 0.000f, -5.4f, 60.0f, 0.0f, 0, 1, 45.2f, 26.2f, 2, 0.400f, 0.000f, 0, 0.200f, 11.3f, 1000.0f }, "Wide" }, // fit 17.7 to SSP Signature Schranz Kick 22.wav
    { "Wide Warm Clang A#1 II",    "Schranz",           { 1.4f, 0.427f, 0.999f, 0.701f, 0.700f, 0.470f, 0.500f, 0.000f, -5.7f, 80.0f, 0.0f, 0, 1, 47.9f, 27.1f, 2, 0.548f, 0.413f, 0, 0.500f, 3.3f, 1000.0f }, "Wide" }, // fit 14.3 to SSP Signature Schranz Kick 20.wav
    { "Wide Warm Clang A1",        "Schranz",           { -0.4f, 0.424f, 0.900f, 0.752f, 0.800f, 0.462f, 0.400f, 0.000f, -4.4f, 60.0f, 0.0f, 0, 1, 48.0f, 28.3f, 2, 0.900f, 0.356f, 0, 0.600f, 7.7f, 1000.0f }, "Wide" }, // fit 12.0 to SSP Signature Schranz Kick 49.wav
    { "Wide Warm Clang A1 II",     "Schranz",           { -0.1f, 0.422f, 0.902f, 0.800f, 0.600f, 0.468f, 0.700f, 0.000f, -5.2f, 60.0f, 0.0f, 0, 1, 47.0f, 26.7f, 2, 0.836f, 0.533f, 0, 0.500f, 4.0f, 600.0f }, "Wide" }, // fit 17.0 to SSP Signature Schranz Kick 10.wav
    { "Wide Warm Clang A1 III",    "Schranz",           { 0.1f, 0.420f, 0.989f, 0.800f, 1.000f, 0.400f, 0.300f, 0.000f, -6.0f, 60.0f, 0.0f, 0, 3, 44.4f, 36.2f, 2, 0.945f, 0.334f, 0, 0.700f, 3.2f, 600.0f }, "Wide" }, // fit 13.1 to SSP Signature Schranz Kick 12.wav
    { "Wide Warm Clang B1",        "Schranz",           { 2.0f, 0.407f, 0.962f, 1.000f, 0.841f, 0.332f, 0.500f, 0.000f, -4.6f, 60.0f, 0.0f, 1, 1, 33.8f, 27.2f, 2, 0.300f, 0.573f, 0, 0.500f, 6.3f, 1000.0f }, "Wide" }, // fit 14.3 to SSP Signature Schranz Kick 71.wav
    { "Wide Warm Clang B1 II",     "Schranz",           { 2.0f, 0.407f, 0.952f, 1.000f, 0.900f, 0.355f, 0.600f, 0.000f, -6.0f, 60.0f, 0.0f, 1, 1, 44.2f, 21.1f, 2, 0.734f, 0.235f, 0, 0.371f, 8.4f, 1000.0f }, "Wide" }, // fit 13.5 to SSP Signature Schranz Kick 83.wav
    { "Wide Warm Clang C2",        "Schranz",           { 2.8f, 0.408f, 0.927f, 1.000f, 0.962f, 0.304f, 0.710f, 0.000f, -6.0f, 80.0f, 0.0f, 1, 1, 30.1f, 29.1f, 2, 0.500f, 0.449f, 0, 0.716f, 7.8f, 1000.0f }, "Wide" }, // fit 13.1 to SSP Signature Schranz Kick 62.wav
    { "Schranz Metal",             "Schranz",           { 2.9f, 0.414f, 0.957f, 0.631f, 0.755f, 0.800f, 0.800f, 0.000f, -2.9f, 20.0f, 0.0f, 0, 1, 39.0f, 28.9f, 2, 0.738f, 0.131f, 0, 0.400f, 0.9f, 1000.0f }, "Crushed" }, // fit 15.8 to MS Signature Kick Shot 30.wav
    { "Schranz Pressure",          "Schranz",           { -0.8f, 0.419f, 0.916f, 0.834f, 0.500f, 0.778f, 0.300f, 0.001f, -1.8f, 20.0f, 1.0f, 0, 1, 28.7f, 60.1f, 0, 0.800f, 1.000f, 0, 0.200f, 2.4f, 1000.0f }, "Crushed" }, // fit 12.8 to SSP Signature Schranz Kick 92.wav
    { "Wide Hammer",               "Schranz",           { 2.3f, 0.414f, 0.736f, 1.000f, 0.977f, 0.900f, 0.000f, 0.000f, -5.1f, 20.0f, 3.0f, 1, 2, 34.6f, 44.4f, 2, 0.300f, 0.623f, 0, 0.400f, 3.4f, 600.0f }, "Crushed" }, // fit 10.0 to ITHTS Signature Schranz Kick 4.wav
    { "Crushed Boom G1",           "Schranz",           { -1.8f, 0.426f, 0.987f, 0.800f, 0.300f, 0.755f, 0.028f, 0.000f, -3.2f, 20.0f, 6.0f, 0, 3, 25.2f, 68.0f, 2, 0.847f, 0.813f, 0, 0.000f, 1.0f, 1000.0f }, "Crushed" }, // fit 13.5 to SSP Signature Schranz Kick 91.wav
    { "Crushed Boom G1 II",        "Schranz",           { -2.4f, 0.419f, 0.831f, 0.783f, 0.700f, 0.826f, 0.459f, 0.000f, -1.9f, 40.0f, 0.0f, 0, 1, 28.8f, 70.7f, 0, 0.600f, 0.930f, 0, 0.191f, 1.8f, 600.0f }, "Crushed" }, // fit 16.3 to SSP Signature Schranz Kick 57.wav
    { "Crushed Clang B1",          "Schranz",           { 2.4f, 0.407f, 0.801f, 1.000f, 0.965f, 0.800f, 0.012f, 0.000f, -5.0f, 20.0f, 6.0f, 1, 2, 28.3f, 44.4f, 2, 0.200f, 0.504f, 0, 0.600f, 2.5f, 1000.0f }, "Crushed" }, // fit 11.5 to MS Signature Kick Shot 37.wav
    { "Crushed Click F#1 III",     "Schranz",           { -2.8f, 0.413f, 0.698f, 0.732f, 1.000f, 0.783f, 0.500f, 0.000f, -0.2f, 20.0f, 0.0f, 0, 1, 27.8f, 72.7f, 1, 0.400f, 0.972f, 0, 0.200f, 9.6f, 1000.0f }, "Crushed" }, // fit 12.9 to MS Signature Kick Shot 15.wav
    { "Crushed Thump B1",          "Schranz",           { 1.9f, 0.367f, 0.407f, 0.500f, 0.590f, 0.776f, 0.900f, 0.050f, -3.8f, 0.0f, 0.0f, 1, 1, 40.9f, 31.8f, 2, 0.881f, 0.000f, 0, 0.700f, 0.6f, 500.0f }, "Crushed" }, // fit 11.4 to ARK Kick Drum Shot 2.wav
    { "Clipped Boom E1",           "Schranz",           { -5.4f, 0.407f, 0.412f, 0.981f, 0.500f, 0.691f, 0.088f, 0.000f, -3.6f, 20.0f, 0.0f, 1, 1, 38.7f, 119.7f, 1, 1.000f, 0.456f, 0, 0.262f, 2.3f, 1000.0f }, "Driven" }, // fit 11.5 to MS Signature Kick Shot 39.wav
    { "Clipped Boom F1",           "Schranz",           { -3.6f, 0.421f, 0.301f, 0.786f, 0.300f, 0.501f, 0.458f, 0.000f, -3.7f, 40.0f, 0.0f, 1, 3, 46.0f, 62.7f, 2, 0.900f, 0.379f, 0, 0.100f, 0.7f, 1000.0f }, "Driven" }, // fit 11.1 to SSP Signature Schranz Kick 72.wav
    { "Clipped Boom G#1",          "Schranz",           { -0.8f, 0.421f, 0.432f, 0.835f, 0.600f, 0.732f, 0.217f, 0.000f, -3.7f, 20.0f, 0.0f, 1, 1, 39.3f, 58.9f, 1, 0.800f, 0.408f, 0, 0.400f, 2.1f, 1000.0f }, "Driven" }, // fit 12.6 to ITHTS Signature Schranz Kick 13.wav
    { "Clipped Clang B1",          "Schranz",           { 1.6f, 0.407f, 0.756f, 1.000f, 0.979f, 0.600f, 0.500f, 0.000f, -6.1f, 20.0f, 6.0f, 1, 1, 33.7f, 48.7f, 2, 0.600f, 0.716f, 0, 0.300f, 4.5f, 600.0f }, "Driven" }, // fit 15.4 to SSP Signature Schranz Kick 96.wav
    { "Clipped Clang B1 II",       "Schranz",           { 2.3f, 0.402f, 0.747f, 1.000f, 0.900f, 0.600f, 0.200f, 0.000f, -3.7f, 20.0f, 0.0f, 1, 2, 35.8f, 50.4f, 2, 0.500f, 0.639f, 0, 0.300f, 2.2f, 1000.0f }, "Driven" }, // fit 13.1 to MS Signature Kick Shot 6.wav
    { "Clipped Clang C2 II",       "Schranz",           { 3.1f, 0.410f, 0.960f, 0.937f, 0.800f, 0.500f, 0.500f, 0.000f, -3.6f, 40.0f, 0.0f, 1, 1, 40.6f, 22.6f, 2, 0.881f, 0.590f, 0, 0.700f, 4.7f, 1000.0f }, "Driven" }, // fit 14.3 to SSP Signature Schranz Kick 82.wav
    { "Clipped Clang D1",          "Schranz",           { -6.8f, 0.414f, 0.197f, 0.600f, 0.800f, 0.700f, 0.300f, 0.000f, -3.5f, 0.0f, 0.0f, 1, 0, 43.0f, 74.7f, 2, 0.997f, 0.813f, 0, 0.700f, 8.7f, 500.0f }, "Driven" }, // fit 10.2 to SSP Signature Schranz Kick 80.wav
    { "Clipped Clang G1",          "Schranz",           { -1.9f, 0.421f, 0.000f, 0.863f, 0.812f, 0.500f, 0.300f, 0.000f, -5.0f, 20.0f, 0.0f, 1, 1, 46.9f, 29.9f, 2, 0.759f, 0.965f, 0, 0.700f, 12.4f, 1000.0f }, "Driven" }, // fit 12.6 to ARK Kick Drum Shot 24.wav
    { "Clipped Clang G1 II",       "Schranz",           { -2.3f, 0.447f, 0.851f, 0.133f, 0.658f, 0.739f, 0.273f, 0.023f, -4.7f, 0.0f, 0.0f, 1, 1, 21.4f, 25.3f, 2, 0.200f, 0.000f, 0, 1.000f, 7.1f, 500.0f }, "Driven" }, // fit 12.7 to ARK Kick Drum Shot 12.wav
    { "Clipped Click G#1 IV",      "Schranz",           { -0.6f, 0.414f, 0.045f, 0.640f, 1.000f, 0.521f, 0.153f, 0.000f, -2.6f, 0.0f, 0.0f, 1, 1, 32.4f, 42.4f, 1, 0.800f, 0.594f, 0, 0.000f, 12.8f, 500.0f }, "Driven" }, // fit 12.0 to MS Signature Kick Shot 29.wav
    { "Clipped Drop B1",           "Schranz",           { 1.7f, 0.414f, 0.442f, 0.857f, 0.400f, 0.510f, 0.164f, 0.000f, -3.5f, 20.0f, 0.0f, 1, 1, 26.7f, 25.5f, 2, 0.951f, 0.152f, 0, 0.500f, 11.6f, 1000.0f }, "Driven" }, // fit 13.8 to MS Signature Kick Shot 47.wav
    { "Clipped Thump G#1 II",      "Schranz",           { -1.2f, 0.425f, 0.350f, 0.672f, 0.000f, 0.600f, 0.000f, 0.000f, -3.6f, 40.0f, 0.0f, 1, 4, 39.8f, 50.8f, 0, 0.996f, 0.513f, 0, 0.400f, 2.4f, 1000.0f }, "Driven" }, // fit 12.8 to SSP Signature Schranz Kick 73.wav
    { "Driven Boom A1",            "Schranz",           { -0.4f, 0.412f, 0.892f, 0.814f, 0.500f, 0.700f, 0.238f, 0.001f, -4.2f, 40.0f, 0.0f, 0, 4, 27.2f, 61.4f, 1, 0.745f, 1.000f, 0, 0.000f, 0.3f, 1000.0f }, "Driven" }, // fit 18.7 to MS Signature Kick Shot 11.wav
    { "Driven Boom G1",            "Schranz",           { -2.5f, 0.419f, 0.876f, 0.872f, 0.300f, 0.708f, 0.187f, 0.000f, -3.1f, 20.0f, 0.0f, 0, 4, 28.4f, 80.4f, 0, 0.633f, 0.971f, 0, 0.300f, 2.8f, 1000.0f }, "Driven" }, // fit 12.6 to SSP Signature Schranz Kick 86.wav
    { "Driven Clang A#1",          "Schranz",           { 1.3f, 0.411f, 0.777f, 1.000f, 0.994f, 0.631f, 0.000f, 0.000f, -4.8f, 20.0f, 0.0f, 0, 2, 36.1f, 47.9f, 2, 0.910f, 0.568f, 0, 0.400f, 4.7f, 600.0f }, "Driven" }, // fit 12.2 to MS Signature Kick Shot 1.wav
    { "Driven Clang A#1 II",       "Schranz",           { 0.8f, 0.410f, 1.000f, 1.000f, 0.900f, 0.625f, 0.554f, 0.000f, -3.3f, 40.0f, 0.0f, 0, 1, 29.3f, 34.0f, 2, 0.996f, 0.563f, 0, 0.500f, 6.0f, 1000.0f }, "Driven" }, // fit 12.9 to SSP Signature Schranz Kick 89.wav
    { "Warm Clang B1",             "Schranz",           { 2.0f, 0.407f, 0.984f, 1.000f, 0.728f, 0.260f, 0.506f, 0.000f, -3.1f, 20.0f, 0.0f, 1, 1, 28.8f, 43.9f, 2, 0.681f, 0.176f, 0, 0.600f, 5.6f, 1000.0f }, "Clean" }, // fit 10.7 to MS Signature Kick Shot 50.wav (kick register)
    { "Driven Clang B1",           "Schranz",           { 2.1f, 0.413f, 0.783f, 1.000f, 0.923f, 0.727f, 0.220f, 0.000f, -3.1f, 20.0f, 0.0f, 0, 0, 32.4f, 48.8f, 2, 0.399f, 0.703f, 0, 0.500f, 3.5f, 600.0f }, "Driven" }, // fit 12.3 to MS Signature Kick Shot 2.wav
    { "Driven Clang B1 II",        "Schranz",           { 2.5f, 0.415f, 0.914f, 0.601f, 0.745f, 0.748f, 0.200f, 0.000f, -3.9f, 20.0f, 0.0f, 0, 1, 39.3f, 27.1f, 2, 0.845f, 0.052f, 0, 0.318f, 0.3f, 1000.0f }, "Driven" }, // fit 15.7 to MS Signature Kick Shot 49.wav
    { "Driven Clang B1 III",       "Schranz",           { 2.1f, 0.405f, 0.827f, 0.900f, 1.000f, 0.671f, 0.300f, 0.000f, -3.3f, 20.0f, 0.0f, 0, 1, 31.8f, 53.8f, 2, 0.600f, 0.206f, 0, 0.000f, 2.1f, 1000.0f }, "Driven" }, // fit 11.5 to MS Signature Kick Shot 24.wav
    { "Driven Clang C#2",          "Schranz",           { 3.6f, 0.414f, 1.000f, 0.835f, 0.944f, 0.569f, 0.400f, 0.000f, -4.4f, 40.0f, 0.0f, 0, 1, 35.2f, 35.5f, 2, 0.665f, 0.095f, 0, 0.200f, 1.2f, 1000.0f }, "Driven" }, // fit 15.2 to SSP Signature Schranz Kick 97.wav
    { "Driven Clang C2",           "Schranz",           { 2.6f, 0.419f, 0.577f, 0.757f, 0.800f, 0.726f, 0.800f, 0.000f, -2.2f, 20.0f, 0.0f, 0, 1, 37.1f, 32.1f, 2, 0.955f, 0.206f, 0, 0.450f, 0.7f, 1000.0f }, "Driven" }, // fit 14.7 to SSP Signature Schranz Kick 77.wav
    { "Driven Clang C2 II",        "Schranz",           { 3.4f, 0.410f, 0.963f, 0.800f, 0.723f, 0.573f, 0.500f, 0.001f, -3.0f, 20.0f, 0.0f, 0, 1, 35.2f, 34.6f, 2, 0.865f, 0.100f, 0, 0.100f, 0.4f, 1000.0f }, "Driven" }, // fit 15.5 to SSP Signature Schranz Kick 81.wav
    { "Driven Clang C2 III",       "Schranz",           { 2.9f, 0.409f, 0.989f, 0.960f, 0.900f, 0.500f, 0.522f, 0.002f, -3.9f, 20.0f, 0.0f, 0, 1, 43.5f, 24.5f, 2, 1.000f, 0.100f, 0, 0.400f, 5.0f, 1000.0f }, "Driven" }, // fit 15.7 to MS Signature Kick Shot 32.wav
    { "Driven Clang C2 IV",        "Schranz",           { 2.8f, 0.413f, 0.822f, 0.717f, 0.745f, 0.709f, 0.900f, 0.000f, -2.1f, 20.0f, 0.0f, 0, 1, 38.4f, 26.5f, 2, 0.600f, 0.164f, 0, 0.800f, 5.2f, 1000.0f }, "Driven" }, // fit 12.9 to MS Signature Kick Shot 45.wav
    { "Driven Clang F#1 II",       "Schranz",           { -3.5f, 0.420f, 0.768f, 0.800f, 0.800f, 0.500f, 0.500f, 0.000f, -3.4f, 20.0f, 0.0f, 0, 1, 28.2f, 73.6f, 2, 0.846f, 0.942f, 0, 0.500f, 6.6f, 1000.0f }, "Driven" }, // fit 11.7 to SSP Signature Schranz Kick 15.wav
    { "Driven Clang G#1 III",      "Schranz",           { -1.0f, 0.425f, 0.997f, 0.637f, 0.855f, 0.516f, 0.200f, 0.000f, -4.7f, 40.0f, 0.0f, 0, 1, 47.9f, 33.5f, 2, 0.766f, 0.398f, 0, 0.200f, 7.3f, 1000.0f }, "Driven" }, // fit 13.0 to SSP Signature Schranz Kick 25.wav
    { "Driven Click G#1 II",       "Schranz",           { -0.6f, 0.412f, 0.558f, 0.959f, 0.947f, 0.700f, 0.800f, 0.000f, -0.5f, 20.0f, 0.0f, 0, 1, 33.5f, 59.3f, 1, 0.700f, 1.000f, 0, 0.200f, 2.3f, 1600.0f }, "Driven" }, // fit 14.7 to MS Signature Kick Shot 33.wav
    { "Driven Punch G#1",          "Schranz",           { -1.0f, 0.416f, 0.824f, 0.740f, 0.200f, 0.738f, 0.264f, 0.000f, -4.1f, 20.0f, 6.0f, 0, 4, 29.4f, 72.1f, 0, 0.800f, 0.952f, 0, 0.200f, 0.9f, 1000.0f }, "Driven" }, // fit 15.0 to MS Signature Kick Shot 34.wav
    { "Schranz Wide Tick",         "Schranz",           { 2.0f, 0.407f, 0.928f, 0.945f, 0.860f, 0.400f, 0.600f, 0.000f, -3.1f, 20.0f, 0.0f, 1, 1, 37.6f, 26.9f, 2, 0.800f, 0.410f, 0, 0.500f, 5.2f, 1000.0f }, "Clean" }, // fit 11.3 to SSP Signature Schranz Kick 90.wav
    { "Clean Clang A#1",           "Schranz",           { 1.3f, 0.407f, 0.880f, 1.000f, 0.900f, 0.227f, 0.700f, 0.000f, -4.1f, 20.0f, 0.0f, 1, 1, 38.5f, 27.7f, 2, 1.000f, 0.305f, 0, 0.400f, 13.6f, 1600.0f }, "Clean" }, // fit 10.4 to MS Signature Kick Shot 19.wav
    { "Clean Clang B1",            "Schranz",           { 1.5f, 0.419f, 1.000f, 0.810f, 0.700f, 0.171f, 0.978f, 0.003f, -4.9f, 20.0f, 3.0f, 0, 1, 47.5f, 27.0f, 2, 0.900f, 1.000f, 0, 0.826f, 3.4f, 1000.0f }, "Clean" }, // fit 12.6 to SSP Signature Schranz Kick 23.wav
    { "Clean Clang G1 III",        "Schranz",           { -1.9f, 0.430f, 0.099f, 0.913f, 0.800f, 0.076f, 0.130f, 0.000f, -3.3f, 0.0f, 0.0f, 1, 2, 40.4f, 35.7f, 2, 0.947f, 0.900f, 0, 0.500f, 12.8f, 500.0f }, "Clean" }, // fit 11.6 to OBI Driven Schranz Kick 4.wav
    { "Clean Drop F#1",            "Schranz",           { -3.3f, 0.423f, 0.044f, 0.818f, 0.600f, 0.185f, 0.700f, 0.000f, -3.8f, 0.0f, 0.0f, 1, 1, 45.9f, 31.3f, 1, 0.300f, 0.972f, 0, 0.797f, 10.3f, 500.0f }, "Clean" }, // fit 9.4 to ARK Kick Drum Shot 25.wav
    { "Warm Boom A1",              "Schranz",           { -0.3f, 0.415f, 0.975f, 0.856f, 0.500f, 0.310f, 1.000f, 0.000f, -1.9f, 20.0f, 10.0f, 0, 0, 39.1f, 34.1f, 2, 0.442f, 0.638f, 0, 0.800f, 4.5f, 250.0f }, "Clean" }, // fit 13.5 to SSP Signature Schranz Kick 45.wav
    { "Warm Clang A#1",            "Schranz",           { 1.1f, 0.420f, 1.000f, 0.800f, 0.800f, 0.300f, 0.700f, 0.000f, -5.8f, 20.0f, 10.0f, 0, 1, 47.3f, 23.2f, 2, 0.900f, 0.004f, 0, 0.300f, 7.4f, 1000.0f }, "Clean" }, // fit 14.2 to SSP Signature Schranz Kick 34.wav
    { "Warm Clang A#1 II",         "Schranz",           { 0.8f, 0.419f, 0.774f, 0.900f, 1.000f, 0.400f, 0.600f, 0.000f, -3.7f, 20.0f, 0.0f, 0, 1, 29.4f, 60.7f, 2, 0.744f, 0.909f, 0, 0.021f, 0.7f, 1000.0f }, "Clean" }, // fit 11.0 to SSP Signature Schranz Kick 26.wav
    { "Warm Clang A1 III",         "Schranz",           { 0.4f, 0.408f, 0.916f, 1.000f, 0.700f, 0.342f, 0.882f, 0.000f, -3.0f, 0.0f, 0.0f, 1, 1, 24.1f, 62.0f, 2, 0.880f, 0.090f, 0, 0.200f, 7.7f, 500.0f }, "Clean" }, // fit 10.0 to MS Signature Kick Shot 9.wav (kick register)
    { "Warm Clang A#1 IV",         "Schranz",           { 1.2f, 0.418f, 0.868f, 0.887f, 0.900f, 0.315f, 0.500f, 0.000f, -4.1f, 40.0f, 0.0f, 0, 1, 41.7f, 30.0f, 2, 0.742f, 0.391f, 0, 0.600f, 2.7f, 1600.0f }, "Clean" }, // fit 13.5 to SSP Signature Schranz Kick 30.wav
    { "Warm Clang A#1 V",          "Schranz",           { 1.0f, 0.423f, 0.867f, 0.635f, 0.722f, 0.453f, 0.600f, 0.000f, -2.8f, 40.0f, 0.0f, 0, 1, 48.0f, 24.4f, 2, 0.867f, 0.159f, 0, 0.500f, 3.7f, 1000.0f }, "Clean" }, // fit 18.6 to SSP Signature Schranz Kick 31.wav
    { "Warm Clang A#1 VI",         "Schranz",           { 1.2f, 0.411f, 0.555f, 0.800f, 0.600f, 0.400f, 1.000f, 0.000f, -3.3f, 40.0f, 0.0f, 1, 1, 39.9f, 24.0f, 2, 0.500f, 0.101f, 0, 0.700f, 5.6f, 1600.0f }, "Clean" }, // fit 11.6 to MS Signature Kick Shot 28.wav
    { "Warm Clang A#1 VII",        "Schranz",           { 1.5f, 0.407f, 0.902f, 0.950f, 0.800f, 0.267f, 0.400f, 0.001f, -4.7f, 40.0f, 0.0f, 1, 3, 40.9f, 27.7f, 2, 0.746f, 1.000f, 0, 0.100f, 7.5f, 1000.0f }, "Clean" }, // fit 14.8 to SSP Signature Schranz Kick 69.wav
    { "Warm Clang A1",             "Schranz",           { -0.5f, 0.424f, 1.000f, 0.725f, 0.863f, 0.298f, 0.500f, 0.000f, -4.1f, 20.0f, 0.0f, 0, 3, 40.4f, 34.6f, 2, 0.900f, 0.435f, 0, 0.200f, 8.0f, 1000.0f }, "Clean" }, // fit 12.2 to SSP Signature Schranz Kick 43.wav
    { "Warm Clang B1 11",          "Schranz",           { 1.9f, 0.413f, 0.890f, 0.800f, 0.600f, 0.407f, 0.462f, 0.001f, -3.1f, 40.0f, 0.0f, 1, 1, 43.5f, 26.7f, 2, 0.719f, 1.000f, 0, 0.700f, 6.9f, 1600.0f }, "Clean" }, // fit 13.7 to SSP Signature Schranz Kick 75.wav
    { "Warm Clang B1 12",          "Schranz",           { 1.8f, 0.407f, 0.665f, 1.000f, 0.987f, 0.328f, 0.400f, 0.000f, -5.6f, 20.0f, 6.0f, 1, 1, 37.6f, 27.9f, 2, 1.000f, 0.390f, 0, 0.100f, 5.5f, 1000.0f }, "Clean" }, // fit 13.8 to MS Signature Kick Shot 20.wav
    { "Warm Clang B1 II",          "Schranz",           { 1.8f, 0.407f, 1.000f, 1.000f, 0.735f, 0.260f, 0.500f, 0.000f, -4.0f, 40.0f, 0.0f, 1, 1, 28.5f, 42.8f, 2, 0.672f, 0.167f, 0, 0.400f, 5.9f, 1000.0f }, "Clean" }, // fit 11.3 to SSP Signature Schranz Kick 54.wav
    { "Warm Clang B1 III",         "Schranz",           { 2.2f, 0.408f, 0.998f, 0.918f, 0.661f, 0.395f, 0.600f, 0.003f, -3.9f, 20.0f, 0.0f, 1, 1, 37.8f, 24.8f, 2, 0.856f, 0.693f, 0, 0.600f, 9.9f, 1000.0f }, "Clean" }, // fit 10.9 to MS Signature Kick Shot 42.wav
    { "Warm Clang B1 IV",          "Schranz",           { 2.2f, 0.425f, 0.977f, 0.757f, 0.800f, 0.400f, 0.700f, 0.000f, -3.7f, 20.0f, 0.0f, 0, 1, 47.9f, 26.0f, 2, 0.800f, 0.500f, 0, 0.400f, 3.4f, 1000.0f }, "Clean" }, // fit 13.9 to SSP Signature Schranz Kick 14.wav
    { "Warm Clang B1 IX",          "Schranz",           { 1.6f, 0.419f, 0.912f, 0.961f, 0.989f, 0.300f, 0.600f, 0.000f, -4.3f, 20.0f, 3.0f, 0, 1, 37.5f, 29.9f, 2, 0.800f, 0.645f, 0, 0.600f, 9.3f, 1000.0f }, "Clean" }, // fit 11.9 to SSP Signature Schranz Kick 17.wav
    { "Warm Clang B1 V",           "Schranz",           { 2.3f, 0.417f, 0.988f, 0.969f, 1.000f, 0.300f, 0.579f, 0.000f, -4.2f, 40.0f, 0.0f, 0, 1, 48.0f, 21.3f, 2, 0.800f, 0.331f, 0, 0.700f, 3.2f, 1600.0f }, "Clean" }, // fit 15.4 to SSP Signature Schranz Kick 33.wav
    { "Warm Clang B1 VI",          "Schranz",           { 1.6f, 0.424f, 0.965f, 0.725f, 0.800f, 0.428f, 0.400f, 0.000f, -3.4f, 20.0f, 0.0f, 0, 1, 44.4f, 23.5f, 2, 0.711f, 0.334f, 0, 0.800f, 4.8f, 1000.0f }, "Clean" }, // fit 14.1 to SSP Signature Schranz Kick 28.wav
    { "Warm Clang B1 VII",         "Schranz",           { 2.0f, 0.407f, 0.987f, 1.000f, 0.900f, 0.400f, 0.480f, 0.000f, -4.1f, 40.0f, 0.0f, 1, 1, 34.7f, 26.5f, 2, 1.000f, 0.141f, 0, 0.200f, 8.3f, 1000.0f }, "Clean" }, // fit 15.8 to SSP Signature Schranz Kick 51.wav
    { "Warm Clang B1 VIII",        "Schranz",           { 2.1f, 0.420f, 0.941f, 0.855f, 0.908f, 0.360f, 0.662f, 0.000f, -3.0f, 20.0f, 0.0f, 0, 1, 33.4f, 43.9f, 2, 0.938f, 0.353f, 0, 0.600f, 2.3f, 600.0f }, "Clean" }, // fit 14.4 to SSP Signature Schranz Kick 35.wav
    { "Warm Clang B1 X",           "Schranz",           { 1.7f, 0.408f, 0.976f, 1.000f, 0.759f, 0.300f, 0.500f, 0.000f, -3.9f, 13.4f, 0.0f, 1, 1, 30.6f, 44.3f, 2, 0.698f, 0.221f, 0, 0.400f, 6.2f, 1000.0f }, "Clean" }, // fit 12.4 to MS Signature Kick Shot 18.wav (kick register)
    { "Warm Clang C2",             "Schranz",           { 2.6f, 0.421f, 0.987f, 0.756f, 0.786f, 0.421f, 0.300f, 0.000f, -5.9f, 20.0f, 3.0f, 0, 1, 47.3f, 23.9f, 2, 0.614f, 0.390f, 0, 0.600f, 2.8f, 600.0f }, "Clean" }, // fit 12.2 to SSP Signature Schranz Kick 48.wav
    { "Warm Clang C2 II",          "Schranz",           { 2.9f, 0.405f, 0.900f, 1.000f, 0.800f, 0.400f, 0.700f, 0.000f, -5.7f, 20.0f, 0.0f, 1, 1, 36.1f, 27.7f, 2, 0.500f, 0.597f, 0, 0.500f, 4.9f, 1000.0f }, "Clean" }, // fit 12.5 to SSP Signature Schranz Kick 63.wav
    { "Warm Clang F1 II",          "Schranz",           { -3.8f, 0.422f, 0.023f, 0.716f, 0.800f, 0.441f, 0.214f, 0.000f, -3.4f, 20.0f, 0.0f, 0, 0, 47.7f, 43.9f, 2, 0.745f, 0.884f, 0, 0.500f, 9.0f, 1000.0f }, "Clean" }, // fit 12.6 to SSP Signature Schranz Kick 9.wav
    { "Warm Clang G#1",            "Schranz",           { -0.9f, 0.423f, 0.823f, 0.759f, 0.900f, 0.465f, 0.400f, 0.000f, -5.1f, 40.0f, 1.0f, 0, 1, 39.6f, 36.2f, 2, 1.000f, 0.588f, 0, 0.600f, 0.8f, 400.0f }, "Clean" }, // fit 14.9 to SSP Signature Schranz Kick 37.wav
    { "Warm Clang G#1 II",         "Schranz",           { -0.8f, 0.412f, 0.900f, 1.000f, 0.700f, 0.400f, 0.012f, 0.000f, -3.1f, 40.0f, 0.0f, 0, 0, 27.9f, 74.8f, 2, 0.800f, 0.658f, 0, 0.000f, 3.1f, 1600.0f }, "Clean" }, // fit 13.0 to SSP Signature Schranz Kick 42.wav
    { "Warm Clang G1 III",         "Schranz",           { -2.5f, 0.426f, 0.053f, 0.817f, 1.000f, 0.300f, 0.700f, 0.000f, -3.9f, 0.0f, 0.0f, 1, 1, 35.9f, 38.9f, 2, 0.000f, 0.941f, 0, 0.400f, 13.2f, 500.0f }, "Clean" }, // fit 12.2 to OBI Driven Schranz Kick 2.wav
    { "Warm Clang G1 IV",          "Schranz",           { -1.6f, 0.421f, 0.718f, 0.800f, 0.794f, 0.418f, 0.000f, 0.000f, -3.8f, 20.0f, 1.0f, 0, 1, 35.0f, 50.3f, 2, 0.813f, 0.454f, 0, 0.300f, 2.6f, 1000.0f }, "Clean" }, // fit 12.2 to SSP Signature Schranz Kick 5.wav
    { "Warm Click A1 II",          "Schranz",           { 0.1f, 0.420f, 0.011f, 1.000f, 1.000f, 0.256f, 0.082f, 0.000f, -3.1f, 20.0f, 0.0f, 1, 1, 47.6f, 15.7f, 0, 0.700f, 0.964f, 0, 0.000f, 13.7f, 1000.0f }, "Clean" }, // fit 14.8 to OBI Driven Schranz Kick 8.wav
    { "Warm Click B1",             "Schranz",           { 2.3f, 0.410f, 0.887f, 0.919f, 0.800f, 0.423f, 0.600f, 0.000f, -5.9f, 40.0f, 3.0f, 1, 1, 45.9f, 25.4f, 1, 1.000f, 0.367f, 0, 0.300f, 5.5f, 1000.0f }, "Clean" }, // fit 12.8 to SSP Signature Schranz Kick 64.wav
    { "Warm Click B1 II",          "Schranz",           { 2.0f, 0.408f, 1.000f, 1.000f, 1.000f, 0.389f, 0.400f, 0.000f, -3.8f, 20.0f, 0.0f, 1, 1, 39.3f, 25.9f, 0, 0.300f, 0.426f, 0, 0.700f, 6.6f, 1000.0f }, "Clean" }, // fit 11.8 to MS Signature Kick Shot 43.wav
    { "Warm Click G#1",            "Schranz",           { -1.2f, 0.425f, 0.000f, 0.789f, 1.000f, 0.309f, 0.200f, 0.000f, -3.0f, 20.0f, 6.0f, 1, 1, 38.8f, 37.5f, 0, 1.000f, 0.909f, 0, 0.419f, 12.9f, 1000.0f }, "Clean" }, // fit 11.8 to SSP Signature Schranz Kick 84.wav
    { "Warm Click G1",             "Schranz",           { -2.0f, 0.423f, 0.038f, 0.788f, 1.000f, 0.300f, 0.500f, 0.000f, -3.1f, 20.0f, 0.0f, 1, 1, 43.4f, 31.9f, 0, 0.940f, 0.952f, 0, 0.400f, 5.8f, 1600.0f }, "Clean" }, // fit 14.5 to OBI Driven Schranz Kick 7.wav
    { "Warm Drop G1",              "Schranz",           { -2.0f, 0.418f, 0.054f, 0.900f, 0.200f, 0.382f, 0.200f, 0.000f, -3.7f, 20.0f, 0.0f, 1, 2, 45.4f, 24.5f, 1, 0.200f, 0.861f, 0, 0.700f, 12.8f, 1600.0f }, "Clean" }, // fit 11.6 to ITHTS Signature Schranz Kick 16.wav
    { "Warm Punch B1",             "Schranz",           { 2.4f, 0.423f, 0.994f, 0.724f, 0.504f, 0.318f, 0.400f, 0.000f, -6.3f, 20.0f, 3.0f, 0, 1, 47.0f, 22.2f, 2, 0.539f, 0.408f, 0, 0.800f, 4.4f, 400.0f }, "Clean" }, // fit 12.8 to SSP Signature Schranz Kick 18.wav
    // --- Industrial (56) ---
    { "Distorted Factory",         "Industrial",        { 1.0f, 0.322f, 0.930f, 0.928f, 1.000f, 0.900f, 0.900f, 0.200f, -2.2f, 20.0f, 0.0f, 1, 1, 28.4f, 27.4f, 1, 0.000f, 0.000f, 0, 0.650f, 0.3f, 1000.0f }, "Short" }, // fit 16.5 to ITHTS Distorted Kick Shot 6.wav
    { "Clipped Boom B1",           "Industrial",        { 1.8f, 0.347f, 0.663f, 0.837f, 0.000f, 0.541f, 0.400f, 0.232f, -3.5f, 20.0f, 0.0f, 1, 1, 45.1f, 40.2f, 0, 0.922f, 0.000f, 0, 0.961f, 0.0f, 1600.0f }, "Short" }, // fit 12.0 to HDIT3 Heavy Kick Shot 13.wav
    { "Crushed Boom A#1",          "Industrial",        { 1.0f, 0.322f, 0.883f, 1.000f, 0.600f, 0.962f, 0.600f, 0.144f, -3.6f, 40.0f, 3.0f, 1, 1, 28.9f, 25.1f, 1, 0.300f, 0.000f, 0, 0.540f, 1.0f, 1000.0f }, "Short" }, // fit 16.4 to ITHTS Distorted Kick Shot 5.wav
    { "Crushed Clang F1",          "Industrial",        { -4.3f, 0.298f, 0.968f, 0.800f, 1.000f, 0.927f, 0.500f, 0.000f, -3.6f, 20.0f, 0.0f, 0, 1, 15.4f, 71.8f, 2, 0.020f, 0.144f, 0, 0.400f, 6.4f, 1000.0f }, "Short" }, // fit 11.6 to HDIT3 Heavy Kick Shot 12.wav
    { "Wide Crushed Clang A#1 II", "Industrial",        { 1.1f, 0.328f, 1.000f, 0.900f, 0.800f, 0.972f, 0.246f, 0.115f, -3.0f, 60.0f, 0.0f, 1, 3, 34.9f, 25.2f, 2, 0.100f, 0.000f, 0, 0.970f, 0.0f, 1000.0f }, "Short" }, // fit 19.5 to ITHTS Distorted Kick Shot 10.wav
    { "Wide Crushed Click A#1",    "Industrial",        { 1.1f, 0.338f, 0.875f, 0.850f, 0.900f, 0.826f, 0.500f, 0.200f, -2.5f, 60.0f, 0.0f, 1, 1, 35.8f, 23.1f, 1, 0.400f, 0.000f, 0, 0.600f, 0.0f, 1000.0f }, "Short" }, // fit 17.3 to ITHTS Distorted Kick Shot 3.wav
    { "Clipped Drop G#1",          "Industrial",        { -0.6f, 0.514f, 0.686f, 0.114f, 0.600f, 0.649f, 0.000f, 0.000f, -3.4f, 0.0f, 0.0f, 1, 1, 22.1f, 38.8f, 1, 1.000f, 0.984f, 0, 0.300f, 6.3f, 500.0f }, "Long" }, // fit 11.3 to ITHTS Heavy Kick 1.wav
    { "Clipped Thump E1",          "Industrial",        { -4.6f, 0.468f, 0.600f, 0.000f, 0.500f, 0.600f, 0.700f, 0.200f, -3.4f, 0.0f, 0.0f, 1, 4, 36.7f, 65.0f, 2, 0.081f, 0.800f, 0, 0.500f, 1.5f, 500.0f }, "Long" }, // fit 15.5 to HDIT3 Processed FX Kick Shot 2.wav
    { "Crushed Click F#1 IV",      "Industrial",        { -2.6f, 0.525f, 0.733f, 0.373f, 1.000f, 0.802f, 0.299f, 0.100f, -3.5f, 0.0f, 0.0f, 1, 3, 35.3f, 31.2f, 1, 0.300f, 0.000f, 0, 0.700f, 1.3f, 500.0f }, "Long" }, // fit 10.9 to HDIT3 Heavy Kick Shot 14.wav
    { "Driven Click A#1 II",       "Industrial",        { 1.4f, 0.512f, 0.768f, 0.833f, 1.000f, 0.714f, 0.000f, 0.100f, -1.2f, 20.0f, 0.0f, 0, 0, 44.3f, 32.0f, 0, 1.000f, 0.300f, 0, 0.700f, 0.4f, 1000.0f }, "Long" }, // fit 13.6 to HDIT3 Heavy Kick Shot 27.wav
    { "Industrial Rumble",         "Industrial",        { 0.7f, 0.424f, 0.799f, 0.401f, 0.700f, 0.544f, 0.446f, 0.700f, -3.0f, 0.0f, 0.0f, 1, 3, 29.9f, 35.6f, 1, 0.500f, 0.000f, 0, 0.731f, 0.5f, 500.0f }, "Rumble" }, // fit 13.0 to OBI Hard Industrial Kick 16.wav
    { "Steel Press",               "Industrial",        { 1.0f, 0.400f, 0.800f, 0.650f, 0.600f, 0.750f, 0.700f, 0.350f, -3.0f, 80.0f, 10.0f, 2, 3, 38.0f, 22.0f, 2, 0.900f, 0.450f, 0, 1.000f, 0.0f, 500.0f }, "Rumble" }, // hand-made
    { "Wide Iron Sub",             "Industrial",        { -2.0f, 0.650f, 0.650f, 0.700f, 0.500f, 0.700f, 0.750f, 0.400f, -3.1f, 100.0f, 14.0f, 2, 4, 28.0f, 34.0f, 2, 0.550f, 0.550f, 1, 1.000f, 0.0f, 500.0f }, "Rumble" }, // hand-made
    { "Clean Rumble D1",           "Industrial",        { -7.3f, 0.391f, 0.647f, 0.443f, 0.326f, 0.232f, 0.800f, 0.700f, -1.7f, 0.0f, 0.0f, 1, 3, 18.1f, 12.7f, 2, 0.283f, 0.000f, 0, 0.809f, 14.5f, 500.0f }, "Rumble" }, // fit 10.7 to OBI Hard Industrial Kick 9.wav
    { "Clipped Rumble A#1 V",      "Industrial",        { 1.0f, 0.208f, 0.935f, 0.745f, 0.800f, 0.672f, 0.775f, 0.539f, -3.2f, 20.0f, 1.0f, 1, 4, 23.7f, 27.2f, 1, 0.500f, 0.000f, 0, 0.790f, 3.4f, 1000.0f }, "Rumble" }, // fit 13.1 to HDIT3 Heavy Kick Shot 3.wav
    { "Clean Clang F1",            "Industrial",        { -4.1f, 0.431f, 0.083f, 0.895f, 0.900f, 0.184f, 0.392f, 0.003f, -5.3f, 0.0f, 0.0f, 1, 1, 33.0f, 49.4f, 2, 0.857f, 1.000f, 0, 1.000f, 8.4f, 500.0f }, "Clean" }, // fit 13.2 to OBI Hard Industrial Kick 13.wav (kick register)
    { "Clipped Rumble A#1 VII",    "Industrial",        { 1.5f, 0.390f, 0.500f, 0.488f, 0.400f, 0.502f, 0.987f, 0.345f, -2.6f, 0.0f, 0.0f, 1, 1, 43.3f, 15.6f, 1, 0.400f, 0.000f, 0, 0.800f, 2.4f, 500.0f }, "Rumble" }, // fit 17.0 to HDIT3 Heavy Kick Shot 25.wav
    { "Clipped Rumble A#1 VIII",   "Industrial",        { 1.2f, 0.266f, 0.255f, 0.568f, 0.379f, 0.500f, 1.000f, 0.360f, -3.8f, 0.0f, 0.0f, 1, 4, 47.1f, 35.1f, 2, 0.534f, 0.000f, 0, 0.800f, 0.1f, 500.0f }, "Rumble" }, // fit 22.6 to HDIT3 Processed FX Kick Shot 1.wav
    { "Clipped Rumble A1 IV",      "Industrial",        { 0.4f, 0.425f, 0.745f, 0.539f, 0.500f, 0.700f, 1.000f, 0.308f, -3.0f, 0.0f, 0.0f, 1, 4, 30.6f, 34.7f, 1, 0.000f, 0.085f, 0, 0.700f, 1.9f, 500.0f }, "Rumble" }, // fit 24.5 to HDIT3 Processed FX Kick Shot 4.wav
    { "Clipped Rumble B1",         "Industrial",        { 1.7f, 0.432f, 0.686f, 0.466f, 0.900f, 0.703f, 0.433f, 0.558f, -4.1f, 0.0f, 0.0f, 1, 3, 32.3f, 34.1f, 1, 0.500f, 0.051f, 0, 0.900f, 0.0f, 500.0f }, "Rumble" }, // fit 14.2 to HDIT3 Heavy Kick Shot 30.wav
    { "Clipped Rumble F1 II",      "Industrial",        { -3.8f, 0.410f, 1.000f, 0.666f, 1.000f, 0.587f, 0.400f, 0.900f, -3.6f, 0.0f, 0.0f, 1, 4, 31.0f, 27.4f, 0, 1.000f, 0.000f, 0, 1.000f, 0.3f, 500.0f }, "Rumble" }, // fit 13.3 to HDIT3 Heavy Kick Shot 20.wav (kick register)
    { "Clipped Rumble C1",         "Industrial",        { -8.6f, 0.645f, 0.231f, 0.200f, 0.225f, 0.553f, 0.500f, 0.800f, -2.4f, 0.0f, 0.0f, 1, 3, 15.3f, 114.0f, 2, 0.559f, 0.105f, 1, 0.604f, 4.7f, 500.0f }, "Rumble" }, // fit 6.8 to HDIT3 Heavy Kick Shot 15.wav
    { "Clipped Rumble D#1",        "Industrial",        { -6.0f, 0.505f, 0.634f, 0.234f, 0.600f, 0.570f, 0.136f, 0.948f, -2.5f, 20.0f, 0.0f, 1, 3, 44.3f, 64.8f, 1, 0.000f, 0.180f, 0, 0.995f, 0.0f, 1000.0f }, "Rumble" }, // fit 15.1 to HDIT3 Heavy Kick Shot 17.wav
    { "Clipped Rumble F#1",        "Industrial",        { -3.4f, 0.365f, 0.364f, 0.374f, 0.500f, 0.582f, 0.700f, 0.400f, -4.6f, 0.0f, 0.0f, 1, 1, 45.4f, 34.6f, 2, 0.936f, 0.000f, 0, 0.800f, 0.6f, 500.0f }, "Rumble" }, // fit 13.6 to HDIT3 Heavy Kick Shot 24.wav
    { "Warm Drop E1",              "Industrial",        { -4.6f, 0.332f, 0.303f, 0.751f, 0.000f, 0.469f, 0.700f, 0.228f, -3.6f, 0.0f, 0.0f, 1, 3, 41.9f, 49.8f, 1, 0.619f, 0.000f, 0, 0.900f, 10.3f, 500.0f }, "Short" }, // fit 13.2 to HDIT3 Heavy Kick Shot 23.wav (kick register)
    { "Clipped Rumble G#1",        "Industrial",        { -1.1f, 0.353f, 0.956f, 0.637f, 0.700f, 0.591f, 0.300f, 0.500f, -3.9f, 20.0f, 0.0f, 1, 4, 36.2f, 40.9f, 2, 0.201f, 0.175f, 0, 1.000f, 3.0f, 600.0f }, "Rumble" }, // fit 18.3 to HDIT3 Assorted Kick Shot 13.wav
    { "Clipped Rumble G1 II",      "Industrial",        { -2.2f, 0.456f, 1.000f, 0.490f, 0.900f, 0.739f, 0.560f, 0.500f, -3.3f, 0.0f, 0.0f, 1, 4, 27.0f, 62.0f, 1, 0.400f, 0.002f, 0, 0.948f, 1.0f, 500.0f }, "Rumble" }, // fit 25.4 to HDIT3 Processed FX Kick Shot 13.wav
    { "Crushed Rumble A#1",        "Industrial",        { 0.7f, 0.425f, 0.721f, 0.466f, 0.400f, 0.788f, 0.609f, 0.748f, -3.0f, 0.0f, 0.0f, 1, 3, 38.6f, 30.2f, 0, 0.000f, 0.000f, 0, 0.800f, 3.1f, 500.0f }, "Rumble" }, // fit 12.4 to HDIT3 Heavy Kick Shot 4.wav
    { "Crushed Rumble F1",         "Industrial",        { -3.8f, 0.252f, 0.745f, 0.950f, 1.000f, 0.783f, 0.388f, 1.000f, -4.4f, 0.0f, 0.0f, 1, 4, 28.2f, 55.1f, 1, 1.000f, 0.049f, 0, 0.313f, 0.7f, 500.0f }, "Rumble" }, // fit 17.0 to HDIT3 Heavy Kick Shot 10.wav
    { "Warm Clang G#1 III",        "Industrial",        { -0.8f, 0.357f, 0.105f, 0.865f, 0.800f, 0.400f, 0.900f, 0.292f, -7.2f, 0.0f, 0.0f, 1, 1, 44.6f, 41.4f, 2, 0.592f, 0.018f, 0, 0.500f, 0.1f, 500.0f }, "Clean" }, // fit 13.1 to HDIT3 Heavy Kick Shot 9.wav (kick register)
    { "Clipped Rumble G#1 II",     "Industrial",        { -1.2f, 0.475f, 0.572f, 0.309f, 1.000f, 0.583f, 0.489f, 0.900f, -4.4f, 0.0f, 0.0f, 1, 4, 31.4f, 39.2f, 1, 0.800f, 0.021f, 0, 0.769f, 2.6f, 500.0f }, "Rumble" }, // fit 13.7 to HDIT3 Processed FX Kick Shot 7.wav (kick register)
    { "Warm Rumble B1 II",         "Industrial",        { 1.5f, 0.416f, 0.590f, 0.459f, 0.690f, 0.381f, 0.294f, 1.000f, -1.5f, 0.0f, 0.0f, 1, 3, 34.0f, 33.2f, 0, 0.807f, 0.027f, 0, 0.421f, 0.1f, 500.0f }, "Rumble" }, // fit 15.6 to OBI Hard Industrial Kick 3.wav
    { "Warm Rumble G#1",           "Industrial",        { -1.3f, 0.428f, 0.911f, 0.300f, 0.600f, 0.439f, 0.400f, 0.536f, -1.9f, 20.0f, 0.0f, 1, 3, 41.1f, 33.2f, 0, 0.541f, 0.045f, 0, 0.900f, 1.2f, 1000.0f }, "Rumble" }, // fit 18.1 to HDIT3 Heavy Kick Shot 29.wav
    { "Warm Rumble G#1 II",        "Industrial",        { -1.4f, 0.428f, 0.764f, 0.520f, 1.000f, 0.454f, 0.432f, 0.900f, -3.2f, 0.0f, 0.0f, 1, 3, 34.5f, 41.7f, 1, 0.700f, 0.042f, 0, 0.900f, 0.4f, 500.0f }, "Rumble" }, // fit 11.0 to HDIT3 Heavy Kick Shot 26.wav
    { "Warm Rumble G#1 III",       "Industrial",        { -1.0f, 0.374f, 0.945f, 0.748f, 0.600f, 0.373f, 0.500f, 0.500f, -2.7f, 20.0f, 0.0f, 1, 4, 33.5f, 43.5f, 1, 0.200f, 0.166f, 0, 1.000f, 6.8f, 1600.0f }, "Rumble" }, // fit 19.9 to HDIT3 Heavy Kick Shot 18.wav
    { "Warm Rumble G1 II",         "Industrial",        { -1.8f, 0.691f, 0.845f, 0.377f, 0.900f, 0.294f, 0.000f, 0.600f, -2.4f, 0.0f, 0.0f, 1, 3, 29.7f, 43.8f, 0, 0.979f, 0.100f, 0, 0.898f, 2.5f, 500.0f }, "Rumble" }, // fit 5.6 to OBI Hard Industrial Kick 15.wav
    { "Wide Crushed Rumble C1",    "Industrial",        { -9.0f, 0.432f, 0.705f, 0.600f, 0.800f, 0.800f, 1.000f, 0.799f, -7.2f, 100.0f, 3.0f, 1, 3, 46.5f, 41.6f, 1, 0.300f, 0.700f, 1, 1.000f, 6.9f, 1000.0f }, "Rumble" }, // fit 13.3 to HDIT3 Assorted Kick Shot 2.wav
    { "Clipped Rumble F1",         "Industrial",        { -4.4f, 0.512f, 0.725f, 0.497f, 0.700f, 0.700f, 0.300f, 0.500f, -4.0f, 0.0f, 0.0f, 1, 4, 21.9f, 68.1f, 1, 1.000f, 0.231f, 0, 0.400f, 8.7f, 500.0f }, "Rumble" }, // fit 14.9 to HDIT3 Heavy Kick Shot 11.wav (kick register)
    { "Wide Clipped Clang G#1",    "Industrial",        { -0.8f, 0.412f, 0.485f, 0.489f, 0.660f, 0.500f, 0.100f, 0.000f, -5.0f, 80.0f, 0.0f, 1, 1, 41.7f, 44.9f, 2, 0.455f, 0.888f, 0, 0.700f, 1.3f, 1000.0f }, "Wide" }, // fit 9.6 to ITHTS Heavy Kick 6.wav
    { "Wide Clipped Drop D1",      "Industrial",        { -7.0f, 0.400f, 0.838f, 0.355f, 0.000f, 0.646f, 0.700f, 0.000f, -6.6f, 60.0f, 3.0f, 1, 4, 36.9f, 18.3f, 1, 0.157f, 0.591f, 0, 0.976f, 12.0f, 600.0f }, "Wide" }, // fit 18.0 to HDIT3 Assorted Kick Shot 1.wav
    { "Wide Crushed Drop A#1",     "Industrial",        { 0.7f, 0.413f, 0.000f, 1.000f, 0.000f, 0.921f, 0.600f, 0.019f, -3.7f, 60.0f, 0.0f, 1, 1, 33.2f, 5.0f, 1, 0.334f, 0.000f, 0, 0.514f, 7.7f, 250.0f }, "Wide" }, // fit 13.6 to ITHTS Heavy Kick 10.wav
    { "Wide Warm Boom B1 II",      "Industrial",        { 2.1f, 0.419f, 0.887f, 1.000f, 0.600f, 0.400f, 0.200f, 0.002f, -3.7f, 60.0f, 0.0f, 1, 1, 39.1f, 18.7f, 0, 0.100f, 0.116f, 0, 0.900f, 4.3f, 1000.0f }, "Wide" }, // fit 11.1 to OBI Hard Industrial Kick 5.wav
    { "Crushed Boom B1",           "Industrial",        { 2.0f, 0.439f, 0.272f, 1.000f, 0.228f, 0.814f, 0.025f, 0.000f, -4.3f, 0.0f, 0.0f, 1, 2, 37.7f, 28.6f, 2, 0.751f, 0.320f, 0, 0.446f, 3.5f, 500.0f }, "Crushed" }, // fit 11.7 to OBI Hard Industrial Kick 4.wav
    { "Crushed Boom F#1",          "Industrial",        { -2.9f, 0.433f, 0.623f, 0.897f, 0.157f, 0.751f, 0.100f, 0.000f, -3.3f, 20.0f, 6.0f, 1, 3, 41.3f, 48.7f, 2, 0.987f, 0.437f, 0, 0.100f, 0.1f, 1000.0f }, "Crushed" }, // fit 11.9 to ITHTS Distorted Kick Shot 7.wav
    { "Clipped Clang F1",          "Industrial",        { -3.6f, 0.422f, 0.981f, 0.100f, 1.000f, 0.700f, 1.000f, 0.000f, -5.1f, 0.0f, 0.0f, 1, 4, 28.0f, 62.0f, 2, 0.078f, 0.389f, 0, 0.600f, 2.2f, 500.0f }, "Driven" }, // fit 16.5 to HDIT3 Processed FX Kick Shot 8.wav (kick register)
    { "Crushed Drop E1",           "Industrial",        { -4.8f, 0.437f, 0.443f, 1.000f, 0.100f, 1.000f, 0.000f, 0.000f, -3.3f, 20.0f, 3.0f, 1, 2, 41.6f, 44.8f, 0, 0.600f, 0.579f, 0, 0.500f, 8.8f, 250.0f }, "Crushed" }, // fit 15.9 to ITHTS Distorted Kick Shot 8.wav
    { "Crushed Thump G#1",         "Industrial",        { -1.2f, 0.441f, 0.619f, 0.665f, 0.300f, 0.934f, 0.539f, 0.000f, -3.3f, 0.0f, 0.0f, 1, 3, 34.7f, 60.9f, 1, 0.731f, 0.993f, 0, 0.906f, 0.1f, 500.0f }, "Crushed" }, // fit 15.4 to ITHTS Distorted Kick Shot 1.wav
    { "Clipped Boom F1 II",        "Industrial",        { -4.3f, 0.444f, 0.696f, 0.800f, 0.500f, 0.500f, 0.100f, 0.000f, -3.3f, 20.0f, 0.0f, 1, 0, 42.5f, 59.8f, 2, 0.800f, 0.818f, 0, 0.600f, 0.6f, 1000.0f }, "Driven" }, // fit 13.2 to ITHTS Distorted Kick Shot 9.wav
    { "Clipped Clang A#1",         "Industrial",        { 0.9f, 0.390f, 0.514f, 0.540f, 0.800f, 0.721f, 0.200f, 0.126f, -4.1f, 20.0f, 0.0f, 1, 1, 43.8f, 14.1f, 2, 0.128f, 0.600f, 0, 0.600f, 0.0f, 250.0f }, "Driven" }, // fit 13.8 to HDIT3 Assorted Kick Shot 11.wav
    { "Clipped Click A#1 III",     "Industrial",        { 0.8f, 0.372f, 0.643f, 0.700f, 1.000f, 0.604f, 0.623f, 0.200f, -3.8f, 0.0f, 0.0f, 1, 3, 28.3f, 29.8f, 0, 0.386f, 0.000f, 0, 0.900f, 2.4f, 500.0f }, "Driven" }, // fit 11.6 to ITHTS Heavy Kick 7.wav
    { "Clipped Click F1 III",      "Industrial",        { -3.5f, 0.433f, 0.128f, 0.732f, 0.890f, 0.500f, 0.400f, 0.000f, -3.2f, 20.0f, 0.0f, 1, 1, 35.4f, 26.6f, 1, 0.800f, 0.981f, 0, 0.800f, 14.3f, 1000.0f }, "Driven" }, // fit 10.2 to OBI Hard Industrial Kick 11.wav
    { "Clipped Thump C#1",         "Industrial",        { -8.1f, 0.452f, 0.750f, 0.125f, 0.400f, 0.617f, 0.745f, 0.004f, -3.4f, 20.0f, 0.0f, 1, 4, 46.2f, 74.0f, 2, 0.248f, 0.300f, 0, 1.000f, 1.1f, 1000.0f }, "Driven" }, // fit 22.9 to HDIT3 Heavy Kick Shot 21.wav
    { "Clipped Thump F1",          "Industrial",        { -4.1f, 0.396f, 0.608f, 0.268f, 0.300f, 0.573f, 0.546f, 0.287f, -3.1f, 20.0f, 0.0f, 1, 4, 43.0f, 47.3f, 0, 0.770f, 0.000f, 0, 0.999f, 0.2f, 1000.0f }, "Driven" }, // fit 23.5 to HDIT3 Heavy Kick Shot 22.wav
    { "Clean Click D1",            "Industrial",        { -6.7f, 0.427f, 0.031f, 0.996f, 1.000f, 0.200f, 0.000f, 0.004f, -3.3f, 0.0f, 0.0f, 1, 2, 42.3f, 45.6f, 1, 0.700f, 0.200f, 0, 0.600f, 15.2f, 500.0f }, "Clean" }, // fit 13.5 to OBI Hard Industrial Kick 10.wav
    { "Clean Click E1",            "Industrial",        { -5.2f, 0.450f, 0.851f, 0.877f, 1.000f, 0.096f, 0.288f, 0.000f, -4.6f, 20.0f, 0.0f, 1, 3, 30.9f, 41.1f, 0, 0.600f, 0.024f, 0, 0.800f, 5.5f, 1600.0f }, "Clean" }, // fit 12.2 to OBI Hard Industrial Kick 14.wav
    { "Warm Click F#1 V",          "Industrial",        { -3.2f, 0.418f, 0.761f, 0.627f, 0.800f, 0.392f, 0.700f, 0.293f, -3.1f, 0.0f, 0.0f, 1, 1, 41.6f, 36.4f, 0, 1.000f, 0.042f, 0, 1.000f, 1.2f, 500.0f }, "Clean" }, // fit 13.4 to HDIT3 Heavy Kick Shot 16.wav
    // --- Rave / Hard Dance (4) ---
    { "Hardstyle Tok",             "Rave / Hard Dance", { 3.0f, 0.500f, 0.850f, 0.700f, 0.500f, 0.750f, 0.700f, 0.100f, -5.2f, 25.0f, 2.0f, 0, 1, 48.0f, 36.0f, 1, 0.700f, 0.500f, 0, 1.000f, 0.0f, 500.0f }, "Long" }, // hand-made (pitch time 45 -> 36 ms: kick register)
    { "Rawstyle Screech",          "Rave / Hard Dance", { 3.0f, 0.480f, 0.850f, 0.750f, 0.600f, 0.950f, 0.850f, 0.150f, -0.9f, 45.0f, 4.0f, 2, 4, 46.0f, 40.0f, 2, 0.800f, 0.500f, 0, 1.000f, 0.0f, 500.0f }, "Long" }, // hand-made
    { "Gabba Lite",                "Rave / Hard Dance", { 4.0f, 0.450f, 0.900f, 0.750f, 0.550f, 0.900f, 0.700f, 0.000f, -5.3f, 0.0f, 0.0f, 1, 3, 46.0f, 30.0f, 1, 0.600f, 0.500f, 0, 1.000f, 0.0f, 500.0f }, "Crushed" }, // hand-made
    { "Euphoric Punch",            "Rave / Hard Dance", { 4.0f, 0.400f, 0.900f, 0.600f, 0.550f, 0.550f, 0.600f, 0.000f, -5.0f, 35.0f, 3.0f, 0, 1, 42.0f, 30.0f, 0, 0.600f, 0.500f, 0, 1.000f, 0.0f, 500.0f }, "Driven" }, // hand-made
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
    static const Snapshot neutral { { id::bodyTone, 1.0f }, { id::pitchFall, 0.0f }, { id::splitFreq, 1000.0f },
                                    { id::beatLength, 0.0f }, { id::subLevel, 0.0f }, { id::startPhase, 0.0f },
                                    { id::driveEnv, 0.0f }, { id::velDrive, 0.0f }, { id::oversample, 1.0f },
                                    { id::pitchCurve, 0.0f }, { id::keyRoot, 60.0f }, { id::modDrive, 0.0f },
                                    { id::choke, 0.0f }, { id::rumbleMode, 0.0f } };
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
    p.beatLength = juce::roundToInt (get (id::beatLength, (float) p.beatLength));
    p.subLevel = get (id::subLevel, p.subLevel);
    p.startPhase = get (id::startPhase, p.startPhase);
    p.driveEnv = get (id::driveEnv, p.driveEnv);
    p.velDrive = get (id::velDrive, p.velDrive);
    p.oversampleIndex = juce::roundToInt (get (id::oversample, (float) p.oversampleIndex));
    p.pitchCurve = get (id::pitchCurve, p.pitchCurve);
    p.keyRoot = juce::roundToInt (get (id::keyRoot, (float) p.keyRoot));
    p.modDrive = get (id::modDrive, p.modDrive);
    p.choke = get (id::choke, 0.0f) > 0.5f;
    p.rumbleMode = juce::roundToInt (get (id::rumbleMode, (float) p.rumbleMode));
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
