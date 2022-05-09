set(SCENE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(
  SCENE_SOURCES
  "${SCENE_DIR}/SceneManager.cpp"
  "${SCENE_DIR}/Scene.cpp"
  "${SCENE_DIR}/Menu.cpp"
  "${SCENE_DIR}/Running.cpp"
)

add_library(scene STATIC ${SCENE_SOURCES})