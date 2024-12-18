# A basic installer setup.
#
# This cmake file introduces two targets
#  awcons-products:      moves all the built assets to a well named directory
#  awcons-installer:   depends on awcons-products, builds an installer
#
# Right now awcons-installer builds just the crudest zip file but this is the target
# on which we will hang the proper installers later

set(AWCONS_PRODUCT_DIR ${CMAKE_BINARY_DIR}/awcons-products)
file(MAKE_DIRECTORY ${AWCONS_PRODUCT_DIR})

message(STATUS "Configuring installer")
add_custom_target(awcons-products ALL)
add_custom_target(awcons-installer)

function(awcons_package format)
    if (TARGET airwin-consolidated)
        get_target_property(output_dir airwin-consolidated RUNTIME_OUTPUT_DIRECTORY)

        if (TARGET airwin-consolidated_${format})
            message(STATUS "Adding airwin-consolidated_${format} to installer")
            add_dependencies(awcons-products airwin-consolidated_${format})
            add_custom_command(
                    TARGET awcons-products
                    POST_BUILD
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMAND echo "Installing ${output_dir}/${format} to ${AWCONS_PRODUCT_DIR}"
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${output_dir}/${format}/ ${AWCONS_PRODUCT_DIR}/
            )
        endif ()
    endif()
endfunction()

awcons_package(VST3)
awcons_package(VST)
awcons_package(LV2)
awcons_package(AU)
awcons_package(CLAP)
awcons_package(Standalone)

if (WIN32)
    message(STATUS "Including special Windows cleanup installer stage")
    add_custom_command(TARGET awcons-products
            POST_BUILD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${CMAKE_COMMAND} -E echo "Cleaning up Windows goobits"
            COMMAND ${CMAKE_COMMAND} -E rm -f "${AWCONS_PRODUCT_DIR}/Airwindows Consolidated.exp"
            COMMAND ${CMAKE_COMMAND} -E rm -f "${AWCONS_PRODUCT_DIR}/Airwindows Consolidated.ilk"
            COMMAND ${CMAKE_COMMAND} -E rm -f "${AWCONS_PRODUCT_DIR}/Airwindows Consolidated.lib"
            COMMAND ${CMAKE_COMMAND} -E rm -f "${AWCONS_PRODUCT_DIR}/Airwindows Consolidated.pdb"
            )
endif ()

add_dependencies(awcons-installer awcons-products)

add_custom_command(
        TARGET awcons-installer
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND echo "Installing LICENSE and so forth to ${AWCONS_PRODUCT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/res/LICENSE.gpl3 ${AWCONS_PRODUCT_DIR}/
)

find_package(Git)

if (Git_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE VERSION_CHUNK
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else ()
    set(VERSION_CHUNK "unknownhash")
endif ()

string(TIMESTAMP AWCONS_DATE "%Y-%m-%d")
if (DEFINED AWCO_ARM64EC)
    set(AWXTRANAME "-arm64ec")
elseif (DEFINED AWCO_ONJUCE7)
    set(AWXTRANAME "-windows7")
else()
    set(AWXTRANAME "")
endif()
set(AWCONS_ZIP AirwindowsConsolidated-${AWCONS_DATE}-${VERSION_CHUNK}-${CMAKE_SYSTEM_NAME}${AWXTRANAME}.zip)
message(STATUS "Will create zip file '${AWCONS_ZIP}'")

if (APPLE)
    message(STATUS "Configuring for Mac installer.")
    add_custom_command(
            TARGET awcons-installer
            POST_BUILD
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory installer
            COMMAND ${CMAKE_SOURCE_DIR}/scripts/installer_mac/make_installer.sh "Airwindows Consolidated" ${CMAKE_BINARY_DIR}/awcons-products ${CMAKE_SOURCE_DIR}/res/installer_mac ${CMAKE_BINARY_DIR}/installer "${AWCONS_DATE}-${VERSION_CHUNK}"
            # COMMAND zip -r installer/${AWCONS_ZIP} ${AWCONS_PRODUCT_DIR}/
    )
elseif (WIN32)
    message(STATUS "Configuring for Windows installer")
    add_custom_command(
            TARGET awcons-installer
            POST_BUILD
            WORKING_DIRECTORY ${AWCONS_PRODUCT_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/installer
            COMMAND 7z a -r ${CMAKE_BINARY_DIR}/installer/${AWCONS_ZIP} .
            COMMAND ${CMAKE_COMMAND} -E echo "ZIP Installer in: ${CMAKE_BINARY_DIR}/installer/${AWCONS_ZIP}")
    find_program(AWCONS_NUGET_EXE nuget.exe PATHS ENV "PATH")
    if(AWCONS_NUGET_EXE AND NOT DEFINED AWCO_ARM64EC)
       message(STATUS "NuGet found at ${AWCONS_NUGET_EXE}")
       add_custom_command(
           TARGET awcons-installer
           POST_BUILD
           WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
           COMMAND ${AWCONS_NUGET_EXE} install Tools.InnoSetup -version 6.2.2
           COMMAND Tools.InnoSetup.6.2.2/tools/iscc.exe /O"installer" /DAWCONS_SRC="${CMAKE_SOURCE_DIR}" /DAWCONS_BIN="${CMAKE_BINARY_DIR}" /DMyAppVersion="${AWCONS_DATE}-${VERSION_CHUNK}${AWXTRANAME}" "${CMAKE_SOURCE_DIR}/scripts/installer_win/awcons.iss")
    else()
       message(STATUS "NuGet not found!")
    endif()
else ()
    message(STATUS "Basic installer: Target is installer/${AWCONS_ZIP}")
    add_custom_command(
            TARGET awcons-installer
            POST_BUILD
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory installer
            COMMAND ${CMAKE_COMMAND} -E tar cvf installer/${AWCONS_ZIP} --format=zip ${AWCONS_PRODUCT_DIR}/
            COMMAND ${CMAKE_COMMAND} -E echo "Installer in: installer/${AWCONS_ZIP}")
endif ()
