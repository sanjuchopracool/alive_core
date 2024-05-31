set(SKIA_PATH ${CORE_THIRD_PARTY_BINARY_DIR}/skia)
set(SKIA_TAG_NAME "m125-665b0e8923")
set(SKIA_PREBUILT_DOWNLOAD_BASE_PATH "https://github.com/sanjuchopracool/SkiaBuild/releases/download")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SKIA_BUILD_TYPE "Debug")
else()
    set(SKIA_BUILD_TYPE "Release")
endif()

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(SKIA_ARCH_TYPE "arm64")
else()
    set(SKIA_ARCH_TYPE "x64")
endif()


if(WIN32)
    set(SKIA_OS_NAME "windows")
    set(SKIA_LIBS
        skia
        user32
        Usp10
        opengl32)
else()
    if(APPLE)
        set(SKIA_OS_NAME "macos")
        set(SKIA_LIBS skia
            "-framework CoreFoundation -framework CoreGraphics -framework CoreText -framework CoreServices"
            "-framework AppKit  -framework Metal"
        z)
        # its crashing with debug binaries on mac, TODO check reason
        set(SKIA_BUILD_TYPE "Release")
    else()
        set(SKIA_OS_NAME "linux")
        set(SKIA_LIBS
            skia
            png
            z
            fontconfig
            freetype
            jpeg
        )
    endif()
endif()

set(SKIA_LIBS_PATH "out/${SKIA_BUILD_TYPE}-${SKIA_ARCH_TYPE}")
SET(SKIA_PREBUILT_FILE_NAME "Skia-${SKIA_TAG_NAME}-${SKIA_OS_NAME}-${SKIA_BUILD_TYPE}-${SKIA_ARCH_TYPE}.zip")
message(STATUS "SKIA_PATH ${SKIA_PATH}")
message(STATUS "SKIA_LIBS_PATH ${SKIA_LIBS_PATH}")
message(STATUS "SKIA_LIBS ${SKIA_LIBS}")

#SKIA DOWNLOAD
if(EXISTS ${SKIA_PATH}/include)
    message(STATUS "skia library exist!")
else()
    file(MAKE_DIRECTORY ${SKIA_PATH})
    set(SKIA_PREBUILT_LOCAL_PATH ${SKIA_PATH}/${SKIA_PREBUILT_FILE_NAME})

    if (NOT EXISTS ${SKIA_PREBUILT_LOCAL_PATH})
        set(SKIA_PREBUILT_DOWNLOAD_PATH ${SKIA_PREBUILT_DOWNLOAD_BASE_PATH}/${SKIA_TAG_NAME}/${SKIA_PREBUILT_FILE_NAME})
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
