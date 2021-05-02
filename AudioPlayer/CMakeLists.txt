file(GLOB AL_PLAYER_FILES *.cpp *.hpp)
add_library(AL_PLAYER SHARED ${AL_PLAYER_FILES})

find_package(OpenAL REQUIRED)
include_directories(${OpenAL_INCLUDE_DIRS})

target_link_libraries(AL_PLAYER ${OPENAL_LIBRARY})

target_include_directories(AL_PLAYER PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../
)