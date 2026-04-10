cmake_minimum_required(VERSION 3.26)

project(
  ALIVE_CORE
  VERSION 0.0.1
  DESCRIPTION "core library for alive, it contains the code for composition and rendering"
  HOMEPAGE_URL "https://github.com/sanjuchopracool/alive.git"
  LANGUAGES CXX C)

# prevent in source build
include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# magnum related
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules/" ${CMAKE_MODULE_PATH})

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# Add corrade and magnum a subprojects
add_subdirectory(third_party/corrade EXCLUDE_FROM_ALL)
add_subdirectory(third_party/magnum EXCLUDE_FROM_ALL)

include(Dependencies.cmake)
alive_setup_dependencies()

set(CORE_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(CORE_THIRD_PARTY_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party)

include(skia.cmake)

add_subdirectory(third_party/lua)

set(ALIVE_CORE_INCLUDE_PATH
    ${SKIA_PATH}
    ${CORE_THIRD_PARTY_DIR}/lua
    ${CORE_THIRD_PARTY_DIR}/svgpp/include
    ${CORE_THIRD_PARTY_DIR}/svgpp/third_party
    ${CORE_THIRD_PARTY_DIR}/svgpp
    ${CORE_THIRD_PARTY_DIR}/svgpp/src/demo/render
    ${CORE_THIRD_PARTY_DIR}/minitrace
)

add_subdirectory(configured_files)
add_subdirectory(src)
