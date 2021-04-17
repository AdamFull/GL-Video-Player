cmake_minimum_required(VERSION 3.1)
project(GL-VIDEO-PLAYER VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(GL-VIDEO-PLAYER main.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/render-system/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/video-decoding-system/)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(GL-VIDEO-PLAYER RenderSystem VideoDecoding)