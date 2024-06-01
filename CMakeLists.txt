cmake_minimum_required(VERSION 3.26)

project(
  inae_core
  VERSION 0.0.1
  DESCRIPTION "core library for alive, it contains non gui code which can be reused"
  HOMEPAGE_URL "https://github.com/sanjuchopracool/inae_core"
  LANGUAGES CXX C)

# prevent in source build
include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(Dependencies.cmake)
inae_setup_dependencies()
add_subdirectory(configured_files)
add_subdirectory(src)
