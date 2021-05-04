file(GLOB GL_RENDER_FILES *.cpp *.hpp)
add_library(GL_RENDER SHARED ${GL_RENDER_FILES})

set(OpenGL_GL_PREFERENCE "LEGACY")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glad/include/)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/freetype/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/freetype/include/)

target_link_libraries(GL_RENDER ${OPENGL_LIBRARIES} glfw ${GLFW_LIBRARIES} glad freetype)