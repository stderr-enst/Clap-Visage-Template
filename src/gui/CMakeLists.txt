add_library(gui
  STATIC
    gui.h
    Lato-Regular.h
    Lato-Regular.cpp
)
target_include_directories(gui
  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)
set_target_properties(gui PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_ARGS}" LINKER_LANGUAGE CXX)

target_link_libraries(gui
  INTERFACE
    adapters

    visage
)

if("${COMPILE_WITH_COVERAGE}")
  append_coverage_compiler_flags_to_target(gui)
endif()
