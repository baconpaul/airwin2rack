# vi:set sw=2 et:
project(airwin-consolidated VERSION ${CMAKE_PROJECT_VERSION})

include ("../cmake/CPM.cmake")


CPMAddPackage(
        NAME JUCE
        GITHUB_REPOSITORY juce-framework/JUCE
        GIT_TAG 8.0.4 # specify the tag to a version of your choice
)

CPMAddPackage(
        NAME clap-juce-extensions
        GITHUB_REPOSITORY free-audio/clap-juce-extensions
        GIT_TAG main
        SUBMODULE_RECURSIVE ON
)

if (NOT DEFINED AWCO_ARM64EC)
  list(APPEND AWCO_FORMATS VST3)
endif()
list(APPEND AWCO_FORMATS AU)
list(APPEND AWCO_FORMATS Standalone)
if (UNIX AND NOT APPLE)
  # I know it works on other platforms but demand is low there
  list(APPEND AWCO_FORMATS LV2)
endif()

set(AW2J_PRODUCT_NAME "Airwindows Consolidated")

juce_add_plugin(${PROJECT_NAME}
  PRODUCT_NAME ${AW2J_PRODUCT_NAME}
  COMPANY_NAME "Airwindows"
  COMPANY_WEBSITE "https://airwindows.com/"
  BUNDLE_ID "com.airwindows.all-windows"
  PLUGIN_MANUFACTURER_CODE Dthr
  PLUGIN_CODE alFX

  IS_SYNTH FALSE
  NEEDS_MIDI_INPUT FALSE
  NEEDS_MIDI_OUTPUT FALSE
  IS_MIDI_EFFECT FALSE
  MICROPHONE_PERMISSION_ENABLED TRUE
  MICROPHONE_PERMISSION_TEXT "AirwindowsEffects would like to use your microphone for Audio Input"

  BLUETOOTH_PERMISSION_ENABLED TRUE
  BLUETOOTH_PERMISSION_TEXT "Airwindows Effects would like to use BlueTooth"


  COPY_PLUGIN_AFTER_BUILD ${COPY_PLUGIN_AFTER_BUILD}

  VST3_CATEGORIES Fx
  AU_MAIN_TYPE kAudioUnitType_Effect
  AU_SANDBOX_SAFE TR-UE


  LV2_URI https://airwindows.com/lv2/airwindows-all
  LV2_SHARED_LIBRARY_NAME AirwindowsALL

  FORMATS ${AWCO_FORMATS}
  )


find_package(Git)

if (Git_FOUND)
  execute_process(
          COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
          OUTPUT_VARIABLE BUILD_HASH
          OUTPUT_STRIP_TRAILING_WHITESPACE
  )
else ()
  set(BUILD_HASH "unkown")
endif ()

message(STATUS "Compiling with git hash ${BUILD_HASH}")

if (${USE_JUCE_PROGRAMS})
  target_compile_definitions(${PROJECT_NAME} PUBLIC
    USE_JUCE_PROGRAMS=1
  )
endif ()

target_compile_definitions(${PROJECT_NAME} PUBLIC
        JUCE_ALLOW_STATIC_NULL_VARIABLES=0
        JUCE_STRICT_REFCOUNTEDPOINTER=1

        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_USE_CURL=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CAMERA=disabled

        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0

        JUCE_MODAL_LOOPS_PERMITTED=0

        JUCE_COREGRAPHICS_DRAW_ASYNC=1

        JUCE_ALSA=1
        JUCE_JACK=1

        JUCE_WASAPI=1
        JUCE_DIRECTSOUND=1

        JUCE_CATCH_UNHANDLED_EXCEPTIONS=0

        BUILD_HASH="${BUILD_HASH}"

        _USE_MATH_DEFINES=1
)
target_sources(${PROJECT_NAME} PRIVATE
  AWConsolidatedEditor.cpp
  AWConsolidatedEditor.h
  AWConsolidatedProcessor.cpp
  AWConsolidatedProcessor.h
  )

target_link_libraries(${PROJECT_NAME} PRIVATE
    airwin-registry
    awconsolidated_resources
    juce::juce_audio_utils
    juce::juce_audio_processors

)

clap_juce_extensions_plugin(TARGET ${PROJECT_NAME}
    CLAP_ID "com.airwindows.consolidated"
    CLAP_PROCESS_EVENTS_RESOLUTION_SAMPLES TRUE
    CLAP_FEATURES
      "audio-effect"
      "filter"
      "phaser"
      "rotary speaker"
      "equalizer"
      "granular"
      "frequency-shifter"
      "distortion"
      "flanger"
      "chorus"
      "delay"
      "reverb"
      "multi-effects"
      "stereo"
      "free and open source")

include(../cmake/basic-installer.cmake)
