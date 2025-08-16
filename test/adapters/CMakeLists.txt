target_sources(doctest_main
  PRIVATE
  test_parameter.cpp
)

target_link_libraries(doctest_main
  PRIVATE
    adapters
)
