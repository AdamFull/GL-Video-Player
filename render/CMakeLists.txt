cmake_minimum_required(VERSION 3.1)
project(RenderSystem CXX)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

file(GLOB RenderSystemFiles *.cpp *.hpp *.c *.h)

add_library(RenderSystem SHARED ${RenderSystemFiles})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glad/include/)

set(OpenGL_GL_PREFERENCE "LEGACY")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

target_link_libraries(RenderSystem ${OPENGL_LIBRARIES} glfw ${GLFW_LIBRARIES} glad)