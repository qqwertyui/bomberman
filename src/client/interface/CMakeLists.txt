set(INTERFACE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(
  INTERFACE_SOURCES
  "${INTERFACE_DIR}/Button.cpp"
)

target_sources(bomberman-client PUBLIC ${INTERFACE_SOURCES})