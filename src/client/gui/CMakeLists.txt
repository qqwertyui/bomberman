set(gui_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(
  gui_SOURCES
  "${gui_DIR}/Button.cpp"
  "${gui_DIR}/TextBox.cpp"
)

target_sources(bomberman-client PUBLIC ${gui_SOURCES})
