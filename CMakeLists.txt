cmake_minimum_required(VERSION 3.21)

project(wolfenstein LANGUAGES CXX)

# Compiler Variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

# Options
# 1. Configuração do SDL2 (Dynamic)
option(SDL_SHARED "Build shared lib" ON)
option(SDL_STATIC "Build static lib" OFF)
# 2. Configuração do SDL2 (Vendored)
option(WOLF3D_VENDORED "Use vendored SDL2 library" ON)

include(FetchContent)

# Check if we are using the vendored SDL2 library or the system one and print a message accordingly
if(WOLF3D_VENDORED)    
    # Declare the SDL2 library to be fetched from its GitHub repository, specifying the desired release tag
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.2  # Stable release as of June 2024
    )

    # Make Avaliable SDL2
    FetchContent_MakeAvailable(SDL2)
    message(STATUS "Using vendored SDL2 library")
    
else()
    find_package(SDL2 REQUIRED)
    message(STATUS "Using system SDL2 library")
endif()

# Get Sources .cpp blobs from folder src
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Exectubales
add_executable(wolfenstein ${SOURCES})

# Includes
target_include_directories(wolfenstein PRIVATE ${CMAKE_SOURCE_DIR}/include)


# 3. Linkagem Dinâmica (Simples e Direta)
if(WOLF3D_VENDORED)
    # FetchContent creates raw targets without namespaces
    target_link_libraries(wolfenstein PRIVATE SDL2main SDL2)
else()
    # find_package creates aliased targets with namespaces
    target_link_libraries(wolfenstein PRIVATE SDL2::SDL2main SDL2::SDL2)
endif()

# 4. A MÁGICA: Copiar a DLL automaticamente para a pasta do build
if(WIN32)
    add_custom_command(TARGET wolfenstein POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:wolfenstein>
        $<TARGET_FILE_DIR:wolfenstein>
        COMMAND_EXPAND_LISTS
    )
endif()
