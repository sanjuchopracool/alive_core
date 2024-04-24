cmake_minimum_required(VERSION 3.20)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

# Set the project name and language
project(
  inae_core
  VERSION 0.0.1
  DESCRIPTION ""
  HOMEPAGE_URL "https://github.com/sanjuchopracool/inae_core"
  LANGUAGES CXX C)

include(Dependencies.cmake)
inae_setup_dependencies()
add_subdirectory(configured_files)
add_subdirectory(src)
