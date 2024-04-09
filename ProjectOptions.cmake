include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(inae_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(inae_setup_options)
  option(inae_ENABLE_HARDENING "Enable hardening" OFF)
  option(inae_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    inae_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    inae_ENABLE_HARDENING
    OFF)

  inae_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR inae_PACKAGING_MAINTAINER_MODE)
    option(inae_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(inae_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(inae_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(inae_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(inae_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(inae_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(inae_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(inae_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(inae_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(inae_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(inae_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(inae_ENABLE_PCH "Enable precompiled headers" OFF)
    option(inae_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(inae_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(inae_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(inae_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(inae_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(inae_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(inae_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(inae_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(inae_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(inae_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(inae_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(inae_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(inae_ENABLE_PCH "Enable precompiled headers" OFF)
    option(inae_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      inae_ENABLE_IPO
      inae_WARNINGS_AS_ERRORS
      inae_ENABLE_USER_LINKER
      inae_ENABLE_SANITIZER_ADDRESS
      inae_ENABLE_SANITIZER_LEAK
      inae_ENABLE_SANITIZER_UNDEFINED
      inae_ENABLE_SANITIZER_THREAD
      inae_ENABLE_SANITIZER_MEMORY
      inae_ENABLE_UNITY_BUILD
      inae_ENABLE_CLANG_TIDY
      inae_ENABLE_CPPCHECK
      inae_ENABLE_COVERAGE
      inae_ENABLE_PCH
      inae_ENABLE_CACHE)
  endif()

  inae_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (inae_ENABLE_SANITIZER_ADDRESS OR inae_ENABLE_SANITIZER_THREAD OR inae_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(inae_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(inae_global_options)
  if(inae_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    inae_enable_ipo()
  endif()

  inae_supports_sanitizers()

  if(inae_ENABLE_HARDENING AND inae_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR inae_ENABLE_SANITIZER_UNDEFINED
       OR inae_ENABLE_SANITIZER_ADDRESS
       OR inae_ENABLE_SANITIZER_THREAD
       OR inae_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${inae_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${inae_ENABLE_SANITIZER_UNDEFINED}")
    inae_enable_hardening(inae_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(inae_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(inae_warnings INTERFACE)
  add_library(inae_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  inae_set_project_warnings(
    inae_warnings
    ${inae_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(inae_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(inae_options)
  endif()

  include(cmake/Sanitizers.cmake)
  inae_enable_sanitizers(
    inae_options
    ${inae_ENABLE_SANITIZER_ADDRESS}
    ${inae_ENABLE_SANITIZER_LEAK}
    ${inae_ENABLE_SANITIZER_UNDEFINED}
    ${inae_ENABLE_SANITIZER_THREAD}
    ${inae_ENABLE_SANITIZER_MEMORY})

  set_target_properties(inae_options PROPERTIES UNITY_BUILD ${inae_ENABLE_UNITY_BUILD})

  if(inae_ENABLE_PCH)
    target_precompile_headers(
      inae_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(inae_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    inae_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(inae_ENABLE_CLANG_TIDY)
    inae_enable_clang_tidy(inae_options ${inae_WARNINGS_AS_ERRORS})
  endif()

  if(inae_ENABLE_CPPCHECK)
    inae_enable_cppcheck(${inae_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(inae_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    inae_enable_coverage(inae_options)
  endif()

  if(inae_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(inae_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(inae_ENABLE_HARDENING AND NOT inae_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR inae_ENABLE_SANITIZER_UNDEFINED
       OR inae_ENABLE_SANITIZER_ADDRESS
       OR inae_ENABLE_SANITIZER_THREAD
       OR inae_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    inae_enable_hardening(inae_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
