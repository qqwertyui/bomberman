set(CLIENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(
  CLIENT_SOURCES
  "${CLIENT_DIR}/main.cpp"
  "${CLIENT_DIR}/Game.cpp"
  "${CLIENT_DIR}/ConfigLoader.cpp"
)

include_directories(${CLIENT_DIR})

add_executable(bomberman-client ${CLIENT_SOURCES})
add_subdirectory(interface)
add_subdirectory(scene)

target_link_libraries(bomberman-client PUBLIC common sfml-window sfml-system sfml-graphics sfml-main)