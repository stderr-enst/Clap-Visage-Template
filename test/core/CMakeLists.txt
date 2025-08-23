target_sources(doctest_main
  PRIVATE
  test_ParameterList.cpp
)

target_link_libraries(doctest_main
  PRIVATE
    core
)
