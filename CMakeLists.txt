cmake_minimum_required(VERSION 3.23)
project(Tetris)

set(CMAKE_CXX_STANDARD 23)

# include opengl
find_package(OpenGL REQUIRED)

add_subdirectory(glfw)
include_directories(glfw/include)

add_subdirectory(glad)
include_directories(glad/include)

add_subdirectory(glm)
include_directories(glm)

file(GLOB_RECURSE SOURCES ./src/*.cpp)

add_executable(Tetris ${SOURCES})
target_link_libraries(Tetris glfw ${OPENGL_LIBRARIES} glad)