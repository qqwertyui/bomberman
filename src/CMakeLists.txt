set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(BUILD_UNIT_TESTS "TRUE")
set(SNAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(
  SNAKE_SOURCES
  "${SNAKE_DIR}/main.cpp"
  "${SNAKE_DIR}/Game.cpp"
  "${SNAKE_DIR}/ConfigLoader.cpp"
)

include_directories(${SNAKE_DIR})

add_executable(SimpleSnake ${SNAKE_SOURCES})
add_subdirectory(utils)
add_subdirectory(interface)
add_subdirectory(scene)

target_link_libraries(SimpleSnake PUBLIC utils sfml-window sfml-system sfml-graphics sfml-main) 