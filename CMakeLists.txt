# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Atomic")

# Fetch the submodules recursivly...
add_custom_target(UPDATE_SUBMODULES ALL 
	COMMAND git submodule update --init --recursive 
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modules/wgpu
)

# Compile the wgpu library...
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/wgpu/dist)
	add_custom_target(BUILD_WGPU ALL 
		COMMAND make package 
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modules/wgpu
	)
endif()

# Dependencies
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if(NOT IOS AND NOT ANDROID)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/glfw)
endif()

# Include sub-projects.
add_subdirectory ("Atomic")
