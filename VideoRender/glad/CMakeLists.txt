cmake_minimum_required(VERSION 3.0)
project(glad)

add_library(glad SHARED src/glad.c)

target_include_directories(glad PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                           $<INSTALL_INTERFACE:include>)

target_link_libraries(glad ${CMAKE_DL_LIBS})