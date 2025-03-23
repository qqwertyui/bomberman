set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(COMMON_SOURCES 
  "${COMMON_DIR}/ArgumentParser.cpp"
  "${COMMON_DIR}/Log.cpp"
)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_subdirectory(messages)

add_library(common STATIC ${COMMON_SOURCES})
if (WIN32)
  target_link_libraries(common PUBLIC ws2_32)
endif (WIN32)

add_subdirectory(ut)
