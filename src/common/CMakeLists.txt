set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(COMMON_SOURCES 
  "${COMMON_DIR}/ArgumentParser.cpp"
)

add_library(common STATIC ${COMMON_SOURCES})
add_subdirectory(ut)
