cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE S_FILES ${CMAKE_CURRENT_SOURCE_DIR}/**.c)
file(GLOB_RECURSE H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/**.h)

add_executable (sample-window ${S_FILES} ${H_FILES} ${M_FILES})
target_link_libraries(sample-window atomic)