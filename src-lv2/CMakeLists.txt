# vi:set sw=2 et:

find_package(PkgConfig REQUIRED)
pkg_check_modules(LV2 REQUIRED lv2)

set(AW_LV2_VERSION_MAJOR "${Airwin2Rack_VERSION_MAJOR}")
set(AW_LV2_VERSION_MINOR "${Airwin2Rack_VERSION_MINOR}")
set(AW_LV2_VERSION_MICRO "${Airwin2Rack_VERSION_PATCH}")
set(AW_LV2_VERSION_TEXT "${Airwin2Rack_VERSION}")

set(AW_LV2_BUNDLE_DIR "${CMAKE_BINARY_DIR}/airwindows-individual.lv2")
file(MAKE_DIRECTORY "${AW_LV2_BUNDLE_DIR}")

# ------------------------------------------------------------------
# TTL generator (native executable — always built for the host)
# ------------------------------------------------------------------
add_executable(airwindows-ttl-gen generate-ttl.cpp)
target_link_libraries(airwindows-ttl-gen PRIVATE airwin-registry)
target_compile_definitions(airwindows-ttl-gen PRIVATE
  AW_LV2_VERSION_MAJOR=${AW_LV2_VERSION_MAJOR}
  AW_LV2_VERSION_MINOR=${AW_LV2_VERSION_MINOR}
  AW_LV2_VERSION_MICRO=${AW_LV2_VERSION_MICRO}
  AW_LV2_VERSION_TEXT="${AW_LV2_VERSION_TEXT}"
)

if (NOT MSVC)
  target_compile_options(airwindows-ttl-gen PRIVATE
    -Wno-unused-function
    -Wno-unused-value
    -Wno-unused-but-set-variable
    -Wno-multichar
  )
endif()

# ------------------------------------------------------------------
# Run the generator at build time to produce the TTL bundle files
# ------------------------------------------------------------------
add_custom_command(
  OUTPUT
    "${AW_LV2_BUNDLE_DIR}/manifest.ttl"
    "${AW_LV2_BUNDLE_DIR}/airwindows-individual.ttl"
  COMMAND "$<TARGET_FILE:airwindows-ttl-gen>" "${AW_LV2_BUNDLE_DIR}"
  DEPENDS airwindows-ttl-gen
  COMMENT "Generating airwindows-individual LV2 TTL files"
  VERBATIM
)

add_custom_target(airwindows-individual-ttl
  DEPENDS
    "${AW_LV2_BUNDLE_DIR}/manifest.ttl"
    "${AW_LV2_BUNDLE_DIR}/airwindows-individual.ttl"
)

# ------------------------------------------------------------------
# LV2 plugin shared library
# ------------------------------------------------------------------
add_library(airwindows-individual MODULE plugin.cpp)

target_include_directories(airwindows-individual PRIVATE ${LV2_INCLUDE_DIRS})
target_link_libraries(airwindows-individual PRIVATE airwin-registry)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(airwindows-individual PRIVATE -static-libstdc++ -static-libgcc)
  else()
    message(STATUS "Linux LV2 builds use static libstdc++/libgcc when compiling with GCC; leaving the runtime dynamic for ${CMAKE_CXX_COMPILER_ID}")
  endif()
endif()

if (NOT MSVC)
  target_compile_options(airwindows-individual PRIVATE
    -Wno-unused-function
    -Wno-unused-value
    -Wno-unused-but-set-variable
    -Wno-multichar
  )
endif()

# LV2 .so names use no lib-prefix and no version suffix
set_target_properties(airwindows-individual PROPERTIES
  PREFIX  ""
  SUFFIX  ".so"
  LIBRARY_OUTPUT_DIRECTORY "${AW_LV2_BUNDLE_DIR}"
)

# ------------------------------------------------------------------
# Top-level convenience target: build the complete bundle
# ------------------------------------------------------------------
add_custom_target(airwindows-individual-lv2 ALL
  DEPENDS airwindows-individual airwindows-individual-ttl
)

# ------------------------------------------------------------------
# Install: copy the bundle to the standard LV2 path
# ------------------------------------------------------------------
install(
  DIRECTORY "${AW_LV2_BUNDLE_DIR}"
  DESTINATION "$ENV{HOME}/.lv2"
)
