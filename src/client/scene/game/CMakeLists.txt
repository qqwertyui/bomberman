set(SCENE_GAME_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(
  SCENE_GAME_SOURCES
  "${SCENE_GAME_DIR}/Scene.cpp"
)

target_sources(bomberman-client PUBLIC ${SCENE_GAME_SOURCES})
