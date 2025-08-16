add_library(adapters INTERFACE)
target_include_directories(adapters
  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)
set_target_properties(adapters PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_ARGS}" LINKER_LANGUAGE CXX)

# target_link_libraries(adapters PUBLIC)
if("${COMPILE_WITH_COVERAGE}")
  append_coverage_compiler_flags_to_target(adapters)
endif()
