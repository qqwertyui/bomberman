set(SCENE_MENU_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(
  SCENE_MENU_SOURCES
  "${SCENE_MENU_DIR}/Menu.cpp"
)

target_sources(bomberman-client PUBLIC ${SCENE_MENU_SOURCES})
