cmake_minimum_required(VERSION 3.16.3)
project(bomberman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_CXX_FLAGS "-DENABLE_LOGS")
set(BUILD_UNIT_TESTS "TRUE")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(common)
if(DEFINED CLIENT_ONLY)
  add_subdirectory(client)
elseif(DEFINED SERVER_ONLY)
  add_subdirectory(server)
else()
  add_subdirectory(client)
  add_subdirectory(server)
endif()