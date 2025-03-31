set(SCENE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(
  SCENE_SOURCES
  "${SCENE_DIR}/SceneManager.cpp"
  "${SCENE_DIR}/SceneBase.cpp"
)

add_subdirectory(lobby)
add_subdirectory(menu)
add_subdirectory(settings)
add_subdirectory(game)

target_sources(bomberman-client PUBLIC ${SCENE_SOURCES})
