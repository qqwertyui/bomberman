set(SCENE_RUNNING_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(
  SCENE_RUNNING_SOURCES
  "${SCENE_RUNNING_DIR}/Running.cpp"
)

target_sources(bomberman-client PUBLIC ${SCENE_RUNNING_SOURCES})
