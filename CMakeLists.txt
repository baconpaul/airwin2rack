cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0091 NEW)
if (DEFINED BUILD_FOR_RACK_ONLY)
    message(STATUS "Building for VCV Rack only; going back to macOS 10.9")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version")
else()
    message(STATUS "Building for JUCE. Setting minimum to 10.11")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "Minimum OS X deployment version")
endif()
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

string(TIMESTAMP DAY_OF_YEAR "%j")
string(TIMESTAMP YEAR "%Y")
math(EXPR PART0 "${YEAR}")
math(EXPR PART1 "${DAY_OF_YEAR} + 1")

project(Airwin2Rack VERSION 1.${PART0}.${PART1} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

option(BUILD_JUCE_PLUGIN "Build a JUCE plugin" OFF)
option(USE_JUCE_PROGRAMS "Add effects as presets when building juce plugin" OFF)
option(BUILD_RACK_PLUGIN "Build a VCV Rack plugin" OFF)
option(COPY_PLUGIN_AFTER_BUILD "Copy plugin after build" OFF)

math(EXPR AW_BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)
message(STATUS "Performing ${AW_BITNESS}-bit build")

message(STATUS "AirwinRackAdapter for Rack Build Process" )
message(STATUS "Installing into '${CMAKE_INSTALL_PREFIX}'")

include(src/autogen_airwin/CMakeLists.txt)

if(NOT MSVC)
    message(STATUS "Adding clang/gcc visibility options")
    add_compile_options(-fvisibility=hidden -fvisibility-inlines-hidden)
endif()


file(GLOB AWDOC_RESOURCES_GLOB
        res/awpdoc/*.txt
)
include(cmake/CmakeRC.cmake)
cmrc_add_resource_library(awdoc_resources ${AWDOC_RESOURCES_GLOB})

add_library(airwin-registry STATIC
        src/AirwinRegistry.cpp
        src/airwin_consolidated_base.cpp

        ${AIRWIN_SOURCES})

target_compile_definitions(airwin-registry PRIVATE _USE_MATH_DEFINES)
target_link_libraries(airwin-registry awdoc_resources)

if (NOT MSVC)
    # consistent even in warnings that chris is
    target_compile_options(airwin-registry PRIVATE
            -Wno-unused-function
            -Wno-unused-value
            -Wno-unused-but-set-variable
            -Wno-multichar
    )
endif()

if (MSVC)
    target_compile_options(airwin-registry PRIVATE /bigobj)
endif()
target_include_directories(airwin-registry PUBLIC src)

if (${BUILD_RACK_PLUGIN})
    set(PLUGIN_NAME ${PROJECT_NAME})
    set(ADDITIONAL_PLUGIN_DISTRIBUTABLES res LICENSE.md README.md)
    include(RackSDK.cmake)

    add_subdirectory(libs/sst-rackhelpers)

    target_include_directories(${RACK_PLUGIN_LIB} PRIVATE src src-rack)
    target_sources(${RACK_PLUGIN_LIB} PRIVATE
            src-rack/Airwin2Rack.cpp
            src-rack/Module.cpp
            )

    target_compile_options(${RACK_PLUGIN_LIB} PUBLIC -Wno-suggest-override -Wno-multichar -Wno-unused-value -Wno-unused-but-set-variable -Wno-unused-variable )
    target_link_libraries(${RACK_PLUGIN_LIB} PUBLIC sst-rackhelpers airwin-registry)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(${RACK_PLUGIN_LIB} PUBLIC -Wno-stringop-truncation)
    endif()
endif()

if (${BUILD_JUCE_PLUGIN})
    file(GLOB AWCON_RESOURCES_GLOB
            res/*.ttf
            res/clipper.svg
            res/mono.svg
            res/stereo.svg
    )
    cmrc_add_resource_library(awconsolidated_resources ${AWCON_RESOURCES_GLOB})

    add_subdirectory(src-juce)
endif()
