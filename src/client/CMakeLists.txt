set(CLIENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(
  CLIENT_SOURCES
  "${CLIENT_DIR}/main.cpp"
  "${CLIENT_DIR}/Game.cpp"
)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SFML)


include_directories(${CLIENT_DIR})

add_executable(bomberman-client ${CLIENT_SOURCES})
add_subdirectory(gui)
add_subdirectory(scene)

target_link_libraries(bomberman-client PRIVATE common messages SFML::Graphics)
