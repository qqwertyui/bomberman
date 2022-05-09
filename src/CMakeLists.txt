set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(BUILD_UNIT_TESTS "TRUE")
set(
  SNAKE_SOURCES
  "main.cpp"
  "Game.cpp"
  "ConfigLoader.cpp"
)
add_subdirectory(utils)
add_subdirectory(scene)

add_executable(SimpleSnake ${SNAKE_SOURCES})

target_link_libraries(SimpleSnake PUBLIC utils scene sfml-window sfml-system sfml-graphics sfml-main) 