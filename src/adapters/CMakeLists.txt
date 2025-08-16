add_library(adapters)
target_sources(adapters
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
      parameter.h
)
target_include_directories(adapters
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
set_target_properties(adapters PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_ARGS}" LINKER_LANGUAGE CXX)

# target_link_libraries(adapters PUBLIC)
if("${COMPILE_WITH_COVERAGE}")
  append_coverage_compiler_flags_to_target(adapters)
endif()
