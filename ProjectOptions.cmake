include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(Selsiro_emulator_supports_sanitizers)
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

macro(Selsiro_emulator_setup_options)
  option(Selsiro_emulator_ENABLE_HARDENING "Enable hardening" ON)
  option(Selsiro_emulator_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    Selsiro_emulator_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    Selsiro_emulator_ENABLE_HARDENING
    OFF)

  Selsiro_emulator_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR Selsiro_emulator_PACKAGING_MAINTAINER_MODE)
    option(Selsiro_emulator_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(Selsiro_emulator_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(Selsiro_emulator_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(Selsiro_emulator_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(Selsiro_emulator_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(Selsiro_emulator_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(Selsiro_emulator_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(Selsiro_emulator_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(Selsiro_emulator_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(Selsiro_emulator_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(Selsiro_emulator_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(Selsiro_emulator_ENABLE_PCH "Enable precompiled headers" OFF)
    option(Selsiro_emulator_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(Selsiro_emulator_ENABLE_IPO "Enable IPO/LTO" ON)
    option(Selsiro_emulator_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(Selsiro_emulator_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(Selsiro_emulator_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(Selsiro_emulator_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(Selsiro_emulator_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(Selsiro_emulator_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(Selsiro_emulator_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(Selsiro_emulator_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(Selsiro_emulator_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(Selsiro_emulator_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(Selsiro_emulator_ENABLE_PCH "Enable precompiled headers" OFF)
    option(Selsiro_emulator_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      Selsiro_emulator_ENABLE_IPO
      Selsiro_emulator_WARNINGS_AS_ERRORS
      Selsiro_emulator_ENABLE_USER_LINKER
      Selsiro_emulator_ENABLE_SANITIZER_ADDRESS
      Selsiro_emulator_ENABLE_SANITIZER_LEAK
      Selsiro_emulator_ENABLE_SANITIZER_UNDEFINED
      Selsiro_emulator_ENABLE_SANITIZER_THREAD
      Selsiro_emulator_ENABLE_SANITIZER_MEMORY
      Selsiro_emulator_ENABLE_UNITY_BUILD
      Selsiro_emulator_ENABLE_CLANG_TIDY
      Selsiro_emulator_ENABLE_CPPCHECK
      Selsiro_emulator_ENABLE_COVERAGE
      Selsiro_emulator_ENABLE_PCH
      Selsiro_emulator_ENABLE_CACHE)
  endif()

  Selsiro_emulator_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (Selsiro_emulator_ENABLE_SANITIZER_ADDRESS OR Selsiro_emulator_ENABLE_SANITIZER_THREAD OR Selsiro_emulator_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(Selsiro_emulator_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(Selsiro_emulator_global_options)
  if(Selsiro_emulator_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    Selsiro_emulator_enable_ipo()
  endif()

  Selsiro_emulator_supports_sanitizers()

  if(Selsiro_emulator_ENABLE_HARDENING AND Selsiro_emulator_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR Selsiro_emulator_ENABLE_SANITIZER_UNDEFINED
       OR Selsiro_emulator_ENABLE_SANITIZER_ADDRESS
       OR Selsiro_emulator_ENABLE_SANITIZER_THREAD
       OR Selsiro_emulator_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${Selsiro_emulator_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${Selsiro_emulator_ENABLE_SANITIZER_UNDEFINED}")
    Selsiro_emulator_enable_hardening(Selsiro_emulator_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(Selsiro_emulator_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(Selsiro_emulator_warnings INTERFACE)
  add_library(Selsiro_emulator_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  Selsiro_emulator_set_project_warnings(
    Selsiro_emulator_warnings
    ${Selsiro_emulator_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(Selsiro_emulator_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(Selsiro_emulator_options)
  endif()

  include(cmake/Sanitizers.cmake)
  Selsiro_emulator_enable_sanitizers(
    Selsiro_emulator_options
    ${Selsiro_emulator_ENABLE_SANITIZER_ADDRESS}
    ${Selsiro_emulator_ENABLE_SANITIZER_LEAK}
    ${Selsiro_emulator_ENABLE_SANITIZER_UNDEFINED}
    ${Selsiro_emulator_ENABLE_SANITIZER_THREAD}
    ${Selsiro_emulator_ENABLE_SANITIZER_MEMORY})

  set_target_properties(Selsiro_emulator_options PROPERTIES UNITY_BUILD ${Selsiro_emulator_ENABLE_UNITY_BUILD})

  if(Selsiro_emulator_ENABLE_PCH)
    target_precompile_headers(
      Selsiro_emulator_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(Selsiro_emulator_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    Selsiro_emulator_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(Selsiro_emulator_ENABLE_CLANG_TIDY)
    Selsiro_emulator_enable_clang_tidy(Selsiro_emulator_options ${Selsiro_emulator_WARNINGS_AS_ERRORS})
  endif()

  if(Selsiro_emulator_ENABLE_CPPCHECK)
    Selsiro_emulator_enable_cppcheck(${Selsiro_emulator_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(Selsiro_emulator_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    Selsiro_emulator_enable_coverage(Selsiro_emulator_options)
  endif()

  if(Selsiro_emulator_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(Selsiro_emulator_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(Selsiro_emulator_ENABLE_HARDENING AND NOT Selsiro_emulator_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR Selsiro_emulator_ENABLE_SANITIZER_UNDEFINED
       OR Selsiro_emulator_ENABLE_SANITIZER_ADDRESS
       OR Selsiro_emulator_ENABLE_SANITIZER_THREAD
       OR Selsiro_emulator_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    Selsiro_emulator_enable_hardening(Selsiro_emulator_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
