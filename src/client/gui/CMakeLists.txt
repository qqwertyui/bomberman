set(GUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(
  GUI_SOURCES
  "${GUI_DIR}/Button.cpp"
  "${GUI_DIR}/Checkbox.cpp"
  "${GUI_DIR}/TextBox.cpp"
)

target_sources(bomberman-client PUBLIC ${GUI_SOURCES})
