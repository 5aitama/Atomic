cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE S_FILES ${CMAKE_CURRENT_SOURCE_DIR}/**.c)
file(GLOB_RECURSE H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/**.h)

add_definitions(-DSHADERS_FOLDER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders")
add_definitions(-DTEXTURES_FOLDER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/textures")

add_executable (sample-window ${S_FILES} ${H_FILES} ${M_FILES})
target_link_libraries(sample-window atomic)