cmake_minimum_required(VERSION 3.28)
project(tetris LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include(FetchContent)
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.0.0
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(SFML)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

include_directories(src)

set(SRC_FILES 
  src/main.cpp 
  src/game.cpp
  src/grid.cpp
  src/pieces.cpp
  src/utils.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Audio)

target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_STATIC)

target_compile_definitions(
  ${PROJECT_NAME}
  PRIVATE
    "ASSETS_PATH=$<IF:$<CONFIG:Debug>,\"${CMAKE_CURRENT_SOURCE_DIR}/assets\",\"./assets\">"
)
