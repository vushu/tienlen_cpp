cmake_minimum_required(VERSION 3.16)

project(thirdparty)

# Disable X11 screensaver extension dependency
set(SDL_X11_XSCRNSAVER OFF CACHE BOOL "" FORCE)

add_subdirectory(SDL)
add_subdirectory(SDL_image)
add_subdirectory(SDL_mixer)
add_subdirectory(SDL_ttf)

# Create an interface library that groups all third-party libraries
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE 
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    SDL3_ttf::SDL3_ttf
)