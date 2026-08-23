cmake_minimum_required(VERSION 3.20)
project(4D-for-OCI LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# Paths
# ============================================================

set(SDK_DIR "${CMAKE_SOURCE_DIR}/../4D-Plugin-SDK/4D Plugin API")
set(ORACLE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../oracle/include")

if(APPLE)
    set(ORACLE_LIB_DIR "${CMAKE_SOURCE_DIR}/../oracle/macos-aarm64")
elseif(WIN32)
    set(ORACLE_LIB_DIR "${CMAKE_SOURCE_DIR}/../oracle/windows-x64")
    set(ORACLE_MSVC_LIB_DIR "${CMAKE_SOURCE_DIR}/../oracle/lib/msvc")
endif()

# ============================================================
# Source files
# ============================================================

set(PLUGIN_SOURCES
    4D-for-OCI-4dplugin.cpp
    "${SDK_DIR}/4DPluginAPI.c"
)

set(PLUGIN_HEADERS
    4D-for-OCI-4dplugin.h
    oci_bridge.h
)

set(PLUGIN_RESOURCES
    manifest.json
    constants.xlf
)

# ============================================================
# Platform-specific target
# ============================================================

if(APPLE)
    # macOS: build a .bundle
    add_library(${PROJECT_NAME} MODULE ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "bundle"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
    )

    # Resources go into Contents/Resources/
    foreach(res ${PLUGIN_RESOURCES})
        target_sources(${PROJECT_NAME} PRIVATE ${res})
        set_source_files_properties(${res} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    endforeach()

    # Link Oracle Instant Client (versioned dylib)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "${ORACLE_LIB_DIR}/libclntsh.dylib"
    )

    # CoreGraphics (required by 4D SDK)
    find_library(COREGRAPHICS_FW CoreGraphics)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COREGRAPHICS_FW})

    # Post-build: fix Oracle dylib install names to use @loader_path
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        # Copy Oracle dylibs with their versioned names (no symlinks)
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ORACLE_LIB_DIR}/libclntsh.dylib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libclntsh.dylib.23.1"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ORACLE_LIB_DIR}/libclntshcore.dylib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libclntshcore.dylib.23.1"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ORACLE_LIB_DIR}/libnnz.dylib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libnnz.dylib"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ORACLE_LIB_DIR}/libociei.dylib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libociei.dylib"

        # Fix plugin binary: @rpath → @loader_path
        COMMAND install_name_tool -change
            "@rpath/libclntsh.dylib.23.1"
            "@loader_path/libclntsh.dylib.23.1"
            "$<TARGET_FILE:${PROJECT_NAME}>"

        # Fix libclntsh.dylib.23.1: id + dependencies
        COMMAND install_name_tool -id
            "@loader_path/libclntsh.dylib.23.1"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libclntsh.dylib.23.1"
        COMMAND install_name_tool -change
            "@rpath/libnnz.dylib"
            "@loader_path/libnnz.dylib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libclntsh.dylib.23.1"
        COMMAND install_name_tool -change
            "@rpath/libclntshcore.dylib.23.1"
            "@loader_path/libclntshcore.dylib.23.1"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libclntsh.dylib.23.1"

        # Fix libclntshcore.dylib.23.1: id
        COMMAND install_name_tool -id
            "@loader_path/libclntshcore.dylib.23.1"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libclntshcore.dylib.23.1"

        # Fix libnnz.dylib: id + dependency on libclntshcore
        COMMAND install_name_tool -id
            "@loader_path/libnnz.dylib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libnnz.dylib"
        COMMAND install_name_tool -change
            "@rpath/libclntshcore.dylib.23.1"
            "@loader_path/libclntshcore.dylib.23.1"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libnnz.dylib"

        # Fix libociei.dylib: id + dependencies
        COMMAND install_name_tool -id
            "@loader_path/libociei.dylib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libociei.dylib"
        COMMAND install_name_tool -change
            "@rpath/libclntsh.dylib.23.1"
            "@loader_path/libclntsh.dylib.23.1"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libociei.dylib"
        COMMAND install_name_tool -change
            "@rpath/libclntshcore.dylib.23.1"
            "@loader_path/libclntshcore.dylib.23.1"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libociei.dylib"

        # Re-sign all modified dylibs (install_name_tool invalidates code signatures)
        COMMAND codesign -fs - "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libclntsh.dylib.23.1"
        COMMAND codesign -fs - "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libclntshcore.dylib.23.1"
        COMMAND codesign -fs - "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libnnz.dylib"
        COMMAND codesign -fs - "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libociei.dylib"

        COMMENT "Copying Oracle libraries with versioned names and fixing install names"
    )

elseif(WIN32)
    # Windows: build a DLL with .4DX extension
    add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

    set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".4DX"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Plugins/${PROJECT_NAME}/Contents/Windows64"
    )

    # Module definition file for FourDPackex export
    target_link_options(${PROJECT_NAME} PRIVATE
        "/DEF:${SDK_DIR}/4DPluginAPI.def"
    )

    # Static CRT
    set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Link Oracle OCI
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "${ORACLE_MSVC_LIB_DIR}/oci.lib"
    )

    # Post-build: copy resources and Oracle DLLs
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "${CMAKE_BINARY_DIR}/Plugins/${PROJECT_NAME}/Contents/Resources"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/manifest.json"
            "${CMAKE_BINARY_DIR}/Plugins/${PROJECT_NAME}/Contents/Resources/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/constants.xlf"
            "${CMAKE_BINARY_DIR}/Plugins/${PROJECT_NAME}/Contents/Resources/"
        # Copy Oracle DLLs next to the .4DX
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ORACLE_LIB_DIR}/oci.dll"
            "${CMAKE_BINARY_DIR}/Plugins/${PROJECT_NAME}/Contents/Windows64/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ORACLE_LIB_DIR}/orannz.dll"
            "${CMAKE_BINARY_DIR}/Plugins/${PROJECT_NAME}/Contents/Windows64/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ORACLE_LIB_DIR}/oraociei.dll"
            "${CMAKE_BINARY_DIR}/Plugins/${PROJECT_NAME}/Contents/Windows64/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ORACLE_LIB_DIR}/orasql.dll"
            "${CMAKE_BINARY_DIR}/Plugins/${PROJECT_NAME}/Contents/Windows64/"
        COMMENT "Copying resources and Oracle DLLs"
    )
endif()

# ============================================================
# Include directories
# ============================================================

target_include_directories(${PROJECT_NAME} PRIVATE
    "${SDK_DIR}"
    "${ORACLE_INCLUDE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

# ============================================================
# Compile 4DPluginAPI.c as C++
# ============================================================

set_source_files_properties("${SDK_DIR}/4DPluginAPI.c"
    PROPERTIES LANGUAGE CXX
)

# ============================================================
# Output to test project Plugins folder (for development)
# ============================================================

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}-test/Plugins"
    )
endif()
