cmake_minimum_required(VERSION 3.9)
project(CRENDER_ENGINE VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)

file(GLOB GL_RENDER_FILES *.c *.h)
add_library(CRENDER_ENGINE ${GL_RENDER_FILES})

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glad/include/)

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/freetype/)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/freetype/include/) freetype

target_link_libraries(CRENDER_ENGINE ${OPENGL_LIBRARIES} glfw ${GLFW_LIBRARIES} glad)