cmake_minimum_required(VERSION 3.10...3.21)

project(wolfenstein LANGUAGES CXX)

# Compiler Variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Configuração do SDL2 (Dynamic)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")


# Options
# 1. Configuração do SDL2 (Dynamic)
option(SDL_SHARED "Build shared lib" ON)
option(SDL_STATIC "Build static lib" OFF)
# 2. Configuração do SDL2 (Vendored)
option(WOLF3D_VENDORED "Use vendored SDL2 library" ON)
include(FetchContent)

# Check if we are using the vendored SDL2 library or 
# the system one and print a message accordingly
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

    # SDL True Type Font Library
    FetchContent_Declare(
            SDL_ttf
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
            GIT_TAG release-2.24.0
    )

    FetchContent_MakeAvailable(SDL_ttf)
    message(STATUS "Using vendored SDL_TTF library")

else()
    find_package(SDL2 REQUIRED)
    find_package(SDL_ttf REQUIRED)
    message(STATUS "Using system SDL2 library")
endif()

# --- Google Test ---
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Lib do jogo (sem main.cpp) ---
file(GLOB_RECURSE GAME_SOURCES "src/*.cpp")
list(REMOVE_ITEM GAME_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

add_library(wolfenstein_lib STATIC ${GAME_SOURCES})
target_include_directories(wolfenstein_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

if(WOLF3D_VENDORED)
    target_link_libraries(wolfenstein_lib PUBLIC SDL2 SDL2_ttf)
else()
    target_link_libraries(wolfenstein_lib PUBLIC SDL2::SDL2)
endif()

# --- Executável principal ---
add_executable(wolfenstein src/main.cpp)

if(WOLF3D_VENDORED)
    target_link_libraries(wolfenstein PRIVATE wolfenstein_lib SDL2main)
else()
    target_link_libraries(wolfenstein PRIVATE wolfenstein_lib SDL2::SDL2main)
endif()

if(WIN32)
    add_custom_command(TARGET wolfenstein POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:wolfenstein>
        $<TARGET_FILE_DIR:wolfenstein>
        COMMAND_EXPAND_LISTS
    )
endif()

# --- Testes ---
enable_testing()
include(GoogleTest)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
endif()

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(wolfenstein_tests ${TEST_SOURCES})
target_link_libraries(wolfenstein_tests PRIVATE wolfenstein_lib GTest::gtest_main)
gtest_discover_tests(wolfenstein_tests)

