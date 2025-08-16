add_executable(standalone main.cpp)
set_target_properties(standalone PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_ARGS}")
target_include_directories(standalone
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src/gui"
)
target_link_libraries(standalone PRIVATE gui)
if("${COMPILE_WITH_COVERAGE}")
  append_coverage_compiler_flags_to_target(standalone)
endif()

install(TARGETS standalone DESTINATION bin)
