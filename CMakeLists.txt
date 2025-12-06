cmake_minimum_required(VERSION 3.16)

# Set Ninja as the default generator
set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)

project(tienlen)

add_subdirectory(thirdparty)

include_directories(include)
set(SOURCES 
    src/main.cpp
    src/renderer.cpp
    src/scene_manager.cpp
    src/menu_scene.cpp
    src/game_scene.cpp
    src/font.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} thirdparty)