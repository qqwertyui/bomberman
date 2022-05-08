set(UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(UTILS_SOURCES 
  "${UTILS_DIR}/ArgumentParser.cpp"
)

add_subdirectory(ut)
add_library(utils STATIC ${UTILS_SOURCES})