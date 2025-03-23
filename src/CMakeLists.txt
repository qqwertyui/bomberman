include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(common)

if(DEFINED CLIENT_ONLY)
  add_subdirectory(client)
elseif(DEFINED SERVER_ONLY)
  add_subdirectory(server)
else()
  add_subdirectory(client)
  add_subdirectory(server)
endif()
