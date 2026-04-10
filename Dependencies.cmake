include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(alive_setup_dependencies)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

   find_package(Boost)
   include_directories(${Boost_INCLUDE_DIRS})

   find_package(Magnum REQUIRED)

  # For each dependency, see if it's
  # already been provided to us by a parent project

  if(NOT TARGET nlohmann_json::nlohmann_json)
    CPMAddPackage("gh:nlohmann/json#v3.11.3")
  endif()

  if(NOT TARGET cxxopts)
    CPMAddPackage("gh:jarro2783/cxxopts#v3.2.0")
  endif()

  if(NOT TARGET spdlog::spdlog)
      CPMAddPackage("gh:gabime/spdlog#v1.15.0")
  endif()

  if(NOT TARGET googletest)
    CPMAddPackage("gh:google/googletest#v1.15.2")
  endif()


  if(NOT TARGET Catch2::Catch2WithMain)
    CPMAddPackage("gh:catchorg/Catch2@3.3.2")
  endif()

endfunction()
