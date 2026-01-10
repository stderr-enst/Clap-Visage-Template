add_library(plugin-clap MODULE
  clap_entry.cpp
  plugin_clap.h
  plugin_clap.cpp
  guicoupling.cpp
)
set_target_properties(plugin-clap PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_ARGS}" SUFFIX ".clap" PREFIX "")

target_include_directories(plugin-clap PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(plugin-clap
  PRIVATE
    core
    gui

    clap
    clap-helpers
)

if("${COMPILE_WITH_COVERAGE}")
  append_coverage_compiler_flags_to_target(plugin-clap)
endif()

install(TARGETS plugin-clap DESTINATION lib)
