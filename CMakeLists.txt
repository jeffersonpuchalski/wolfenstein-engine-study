cmake_minimum_required(VERSION 3.15)

project(wolfenstein LANGUAGES CXX)

# Compiler Variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include libs
include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.32.10
)

# Make Avaliable SDL2
FetchContent_MakeAvailable(SDL2)

# Exectubales
add_executable(wolfenstein src/main.cpp)

# Windowns 32 Compile
if(WIN32)
    target_link_libraries(wolfenstein PRIVATE SDL::SDL2main)
else()
    # Add Target Link
    target_link_libraries(wolfenstein PRIVATE 
        SDL2::SDL2
        SDL2::SDL2main
    )
endif()

