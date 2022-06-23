include(CMakeParseArguments)

function(add_ut TEST_NAME)
  if(DEFINED BUILD_UNIT_TESTS)
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} PUBLIC gtest gtest_main pthread)
  endif()
endfunction(add_ut)
