add_library(gui)
target_sources(gui
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
      gui.h
)
target_include_directories(gui
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
set_target_properties(gui PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_ARGS}" LINKER_LANGUAGE CXX)

target_link_libraries(gui
  PUBLIC
    adapters

    visage
)

if("${COMPILE_WITH_COVERAGE}")
  append_coverage_compiler_flags_to_target(gui)
endif()
