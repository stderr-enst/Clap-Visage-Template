add_executable(headless main.cpp)
set_target_properties(headless PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_ARGS}")
if("${COMPILE_WITH_COVERAGE}")
  append_coverage_compiler_flags_to_target(headless)
endif()

install(TARGETS headless DESTINATION bin)
