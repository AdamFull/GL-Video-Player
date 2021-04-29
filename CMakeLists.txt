cmake_minimum_required(VERSION 3.9)
project(GL-VIDEO-PLAYER VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

file(GLOB CPP_FILES *.cpp *.c *.hpp *.h)
add_executable(GL-VIDEO-PLAYER ${CPP_FILES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glad/include/)

set(OpenGL_GL_PREFERENCE "LEGACY")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${OpenAL_INCLUDE_DIRS})

if(ANDROID)
find_library( log-lib log )
endif()

find_library(AVDEVICE_LIBRARY avdevice REQUIRED)
find_library(AVFILTER_LIBRARY avfilter REQUIRED)
find_library(AVFORMAT_LIBRARY avformat REQUIRED)
find_library(AVCODEC_LIBRARY avcodec REQUIRED)
find_library(AVUTIL_LIBRARY avutil REQUIRED)
find_library(ASWRESAMPLE_LIBRARY swresample REQUIRED)
find_library(SWSCALE_LIBRARY swscale REQUIRED)

set(FFMPEG_LIBRARIES
        ${AVCODEC_LIBRARY}
        ${AVFORMAT_LIBRARY}
        ${AVUTIL_LIBRARY}
        ${AVDEVICE_LIBRARY}
        ${AVFILTER_LIBRARY}
        ${ASWRESAMPLE_LIBRARY}
        ${SWSCALE_LIBRARY})

set(SHARED_LIBRARIES_FOR_OS ${OPENGL_LIBRARIES} glfw ${GLFW_LIBRARIES} ${OPENAL_LIBRARY} glad)

if(ANDROID)
    set(PROJECT_LIBRARIES ${SHARED_LIBRARIES_FOR_OS} ${FFMPEG_LIBRARIES} ${log-lib})
elseif(IOS)
    set(PROJECT_LIBRARIES ${SHARED_LIBRARIES_FOR_OS} ${FFMPEG_LIBRARIES})
else()
    set(PROJECT_LIBRARIES ${SHARED_LIBRARIES_FOR_OS} ${FFMPEG_LIBRARIES})
endif()

target_link_libraries(GL-VIDEO-PLAYER ${PROJECT_LIBRARIES})