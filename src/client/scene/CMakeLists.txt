set(SCENE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(
  SCENE_SOURCES
  "${SCENE_DIR}/SceneManager.cpp"
  "${SCENE_DIR}/Scene.cpp"
)

add_subdirectory(menu)
add_subdirectory(running)
add_subdirectory(settings)

target_sources(bomberman-client PUBLIC ${SCENE_SOURCES})