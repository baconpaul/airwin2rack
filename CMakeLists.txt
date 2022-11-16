cmake_minimum_required(VERSION 3.20)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
project(airwin2-rack-hack VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB RACK_SOURCES src/*.cpp src/autogen_airwin/*.cpp)
set(RACK_DIR "/Users/paul/dev/music/rack/Rack-SDK")

add_library(${PROJECT_NAME} STATIC ${RACK_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
        ${RACK_DIR}/include
        ${RACK_DIR}/dep/include
        )

add_custom_command(TARGET ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND RACK_DIR=${RACK_DIR} arch -x86_64 make -j
        COMMAND ${CMAKE_COMMAND} -E copy plugin.dylib /Users/paul/Documents/Rack2/plugins/Airwin2Rack
        USES_TERMINAL)


# This is the full build
add_custom_target(${PROJECT_NAME}_FULL)
add_dependencies(${PROJECT_NAME}_FULL ${PROJECT_NAME})
add_custom_command(TARGET ${PROJECT_NAME}_FULL
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND RACK_DIR=${RACK_DIR} arch -x86_64 make dist -j
        COMMAND RACK_DIR=${RACK_DIR} arch -x86_64 make install -j
        USES_TERMINAL)

