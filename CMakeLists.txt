cmake_minimum_required(VERSION 3.22)

project(PowerKick VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE AND NOT CMAKE_OSX_ARCHITECTURES)
  set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Universal binary" FORCE)
endif()
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum macOS")

option(POWERKICK_COPY_AFTER_BUILD "Install plug-ins into the user plug-in folders after build" OFF)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/JUCE/CMakeLists.txt")
  add_subdirectory(third_party/JUCE)
else()
  include(FetchContent)
  FetchContent_Declare(JUCE GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
                            GIT_TAG 29396c22c93392d6738e021b83196283d6e4d850)
  FetchContent_MakeAvailable(JUCE)
endif()

set(POWERKICK_FORMATS VST3 Standalone)
if(APPLE)
  list(APPEND POWERKICK_FORMATS AU)
endif()

juce_add_plugin(PowerKick
  COMPANY_NAME "Roman"
  COMPANY_WEBSITE ""
  BUNDLE_ID "com.roman.powerkick"
  PLUGIN_MANUFACTURER_CODE Rmnk
  PLUGIN_CODE Pwkk
  PRODUCT_NAME "PowerKick"
  FORMATS ${POWERKICK_FORMATS}
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT FALSE
  IS_MIDI_EFFECT FALSE
  EDITOR_WANTS_KEYBOARD_FOCUS FALSE
  VST3_CATEGORIES Instrument Drum Synth
  AU_MAIN_TYPE kAudioUnitType_MusicDevice
  COPY_PLUGIN_AFTER_BUILD ${POWERKICK_COPY_AFTER_BUILD})

set(POWERKICK_CORE_SOURCES
  Source/Params.cpp
  Source/Presets.cpp
  Source/KickRenderer.cpp)

target_sources(PowerKick PRIVATE
  ${POWERKICK_CORE_SOURCES}
  Source/PluginProcessor.cpp
  Source/PluginEditor.cpp)

target_include_directories(PowerKick PRIVATE Source)
target_compile_definitions(PowerKick PUBLIC
  JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0 JUCE_DISPLAY_SPLASH_SCREEN=0)
target_link_libraries(PowerKick
  PRIVATE juce::juce_audio_utils juce::juce_dsp
  PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_lto_flags juce::juce_recommended_warning_flags)

# Headless DSP/preset tests: the same engine code, no plug-in wrapper.
juce_add_console_app(powerkick_tests PRODUCT_NAME "PowerKick tests")
target_sources(powerkick_tests PRIVATE ${POWERKICK_CORE_SOURCES} Tests/PowerKickTests.cpp)
target_include_directories(powerkick_tests PRIVATE Source)
target_compile_definitions(powerkick_tests PRIVATE JUCE_USE_CURL=0 JUCE_WEB_BROWSER=0)
target_link_libraries(powerkick_tests
  PRIVATE juce::juce_audio_formats juce::juce_audio_processors juce::juce_dsp
  PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_warning_flags)

enable_testing()
add_test(NAME powerkick_tests COMMAND powerkick_tests)
