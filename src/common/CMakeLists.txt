set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(COMMON_SOURCES 
  "${COMMON_DIR}/ArgumentParser.cpp"
)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_subdirectory(itf)

add_library(common STATIC ${COMMON_SOURCES})
if (WIN32)
  target_link_libraries(common PUBLIC ws2_32)
endif (WIN32)

add_subdirectory(logging)
add_subdirectory(ut)
