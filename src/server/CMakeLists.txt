set(SERVER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(
  SERVER_SOURCES
  "${SERVER_DIR}/main.cpp"
  "${SERVER_DIR}/Application.cpp"
  "${SERVER_DIR}/ClientManager.cpp"
  "${SERVER_DIR}/Client.cpp"
  "${SERVER_DIR}/Database.cpp"
  "${SERVER_DIR}/Lobby.cpp"
)

add_executable(bomberman-server ${SERVER_SOURCES})
target_link_libraries(bomberman-server pthread common messages)
