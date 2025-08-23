add_library(core INTERFACE)
target_include_directories(core
  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)
set_target_properties(core PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_ARGS}" LINKER_LANGUAGE CXX)

# target_link_libraries(core PUBLIC)
if("${COMPILE_WITH_COVERAGE}")
  append_coverage_compiler_flags_to_target(core)
endif()
