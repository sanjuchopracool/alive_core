set(SKIA_PATH ${CORE_THIRD_PARTY_BINARY_DIR}/skia)
set(SKIA_PREBUILT_DOWNLOAD_BASE_PATH https://github.com/sanjuchopracool/skia_prebuilt/releases/download/weekly-2024-02-24)

if(WIN32)
    set(SKIA_LIBS
        skia
        user32
        Usp10
        opengl32)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SKIA_LIBS_PATH win/x64/clang_debug)
    else()
        set(SKIA_LIBS_PATH win/x64/clang_release)
    endif()

    set(SKIA_PREBUILT_FILE_NAME skia_win_x64_clang_debug_and_release.zip)

else()
    if(APPLE)
        set(SKIA_LIBS skia
            "-framework CoreFoundation" "-framework CoreGraphics" "-framework CoreText" "-framework CoreServices"
        z)
        if (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
            set(SKIA_LIBS_PATH macos/arm64/clang_release)
            set(SKIA_PREBUILT_FILE_NAME skia_macos_arm64_clang_release.tar.gz)
        else()
            set(SKIA_LIBS_PATH macos/x64/clang_release)
            set(SKIA_PREBUILT_FILE_NAME skia_macos_x64_clang_release.tar.gz)
        endif()
    else()
        set(SKIA_LIBS
            skia
            png
            z
            fontconfig
            freetype
            jpeg
        )
        set(SKIA_LIBS_PATH linux/x64/clang_release)
        set(SKIA_PREBUILT_FILE_NAME skia_linux_x64_clang_release.tar.gz)
    endif()
endif()


#SKIA DOWNLOAD
if(EXISTS ${SKIA_PATH}/include)
    message(STATUS "skia library exist!")
else()
    file(MAKE_DIRECTORY ${SKIA_PATH})
    set(SKIA_PREBUILT_LOCAL_PATH ${SKIA_PATH}/${SKIA_PREBUILT_FILE_NAME})

    if (NOT EXISTS ${SKIA_PREBUILT_LOCAL_PATH})
        set(SKIA_PREBUILT_DOWNLOAD_PATH ${SKIA_PREBUILT_DOWNLOAD_BASE_PATH}/${SKIA_PREBUILT_FILE_NAME})
        message(STATUS "skia library does not exist, downloading it from ${SKIA_PREBUILT_DOWNLOAD_PATH}")
        file (DOWNLOAD
            ${SKIA_PREBUILT_DOWNLOAD_PATH}
            ${SKIA_PREBUILT_LOCAL_PATH}
        )
    else()
        message(STATUS "skia archieve ${SKIA_PREBUILT_LOCAL_PATH} exists")
    endif()

    message(STATUS "extracting archieve ${SKIA_PREBUILT_LOCAL_PATH}")
    execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${SKIA_PREBUILT_LOCAL_PATH}
    WORKING_DIRECTORY ${SKIA_PATH}
    )
    message(STATUS "deleting archieve ${SKIA_PREBUILT_LOCAL_PATH}")
    file(REMOVE ${SKIA_PREBUILT_LOCAL_PATH})

endif()
