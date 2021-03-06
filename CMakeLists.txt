cmake_minimum_required(VERSION 3.9)
project(GL-VIDEO-PLAYER VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

file(GLOB CPP_FILES *.cpp *.c *.hpp *.h)
add_executable(GL-VIDEO-PLAYER ${CPP_FILES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CVideoEncDec/)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CAudioPlayer/)

find_package(SFML COMPONENTS graphics audio REQUIRED)

if(ANDROID)
find_library( log-lib log )
endif()

set(SHARED_LIBRARIES_FOR_OS VIDEO_ENCODER_DECODER sfml-graphics sfml-audio)

if(ANDROID)
    set(PROJECT_LIBRARIES ${SHARED_LIBRARIES_FOR_OS} ${log-lib})
elseif(APPLE)
    set(PROJECT_LIBRARIES ${SHARED_LIBRARIES_FOR_OS})
elseif(UNIX AND NOT APPLE)
    set(PROJECT_LIBRARIES ${SHARED_LIBRARIES_FOR_OS})
else()
    set(PROJECT_LIBRARIES ${SHARED_LIBRARIES_FOR_OS})
endif()

target_link_libraries(GL-VIDEO-PLAYER PRIVATE ${PROJECT_LIBRARIES})