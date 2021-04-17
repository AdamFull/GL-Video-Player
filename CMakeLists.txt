cmake_minimum_required(VERSION 3.9)
project(GL-VIDEO-PLAYER VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

file(GLOB CPP_FILES *.cpp *.c *.hpp)
add_executable(GL-VIDEO-PLAYER ${CPP_FILES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glad/include/)

set(OpenGL_GL_PREFERENCE "LEGACY")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(GL-VIDEO-PLAYER ${OPENGL_LIBRARIES} glfw ${GLFW_LIBRARIES} ${OPENAL_LIBRARIES} glad PkgConfig::LIBAV)