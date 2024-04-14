include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(inae_setup_dependencies)

   find_package(Boost)
   include_directories(${Boost_INCLUDE_DIRS})

  # For each dependency, see if it's
  # already been provided to us by a parent project

  if(NOT TARGET fmtlib::fmtlib)
    cpmaddpackage("gh:fmtlib/fmt#10.1.0")
  endif()

  if(NOT TARGET bezier)
    set(BEZIER_TESTS OFF)
    cpmaddpackage("gh:oysteinmyrmo/bezier#v0.2.1")
  endif()

  if(NOT TARGET nlohmann_json::nlohmann_json)
    cpmaddpackage("gh:nlohmann/json#v3.11.3")
  endif()

  if(NOT TARGET cxxopts)
    cpmaddpackage("gh:jarro2783/cxxopts#v3.2.0")
  endif()

  if(NOT TARGET spdlog::spdlog)
    cpmaddpackage(
      NAME
      spdlog
      VERSION
      1.13.0
      GITHUB_REPOSITORY
      "gabime/spdlog"
      OPTIONS
      "SPDLOG_FMT_EXTERNAL ON")
  endif()

  if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage("gh:catchorg/Catch2@3.3.2")
  endif()

endfunction()
