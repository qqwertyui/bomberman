set(CLIENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(
  CLIENT_SOURCES
  "${CLIENT_DIR}/main.cpp"
  "${CLIENT_DIR}/Game.cpp"
)

set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)

include_directories(${CLIENT_DIR})

add_executable(bomberman-client ${CLIENT_SOURCES})
add_subdirectory(gui)
add_subdirectory(scene)

target_link_libraries(bomberman-client PRIVATE common messages SFML::Graphics)
