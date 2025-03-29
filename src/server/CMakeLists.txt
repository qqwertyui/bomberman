set(SERVER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(
  SERVER_SOURCES
  "${SERVER_DIR}/main.cpp"
)

add_executable(bomberman-server ${SERVER_SOURCES})
target_link_libraries(bomberman-server pthread common messages)
