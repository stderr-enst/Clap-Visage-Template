# Set up doctest
include(CTest)

# Add generic lizard as ctest target to test for complexity metrics in the source code
find_program(LIZARD_COMMAND NAMES lizard)
if(LIZARD_COMMAND)
  add_test(NAME lizard COMMAND ${LIZARD_COMMAND} "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/test")
endif()

set(DOCTEST_NO_INSTALL ON CACHE BOOL "Disable doctest installation")
include(FetchContent)
FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest
  GIT_TAG master # or any SHA, Branch or tag like v2.4.9
)
FetchContent_MakeAvailable(doctest)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

# Include doctest.h as -isystem to silence clang-tidy warnings
get_property(DOCTEST_INCLUDE_DIR TARGET doctest PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
include_directories(SYSTEM ${DOCTEST_INCLUDE_DIR})

# Building tests
add_executable(doctest_main)
target_sources(doctest_main
  PRIVATE
    doctest_main.cpp
)
set_target_properties(doctest_main PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_ARGS}")
target_link_libraries(doctest_main
  PRIVATE
    doctest::doctest
)

# Add tests for components to the doctest_main target
add_subdirectory(adapters)
add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(headless)
add_subdirectory(plugin-clap)
add_subdirectory(standalone)
add_subdirectory(usecases)

doctest_discover_tests(doctest_main WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if("${COMPILE_WITH_COVERAGE}")
  setup_target_for_coverage_lcov(
      NAME coverage
      EXECUTABLE "${CMAKE_CTEST_COMMAND}" "--verbose"
      DEPENDENCIES
        doctest_main
      #EXCLUDE "/usr/*" "*/_deps/*" "test"
  )
endif()
