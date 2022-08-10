# CMakeList.txt : CMake project for Atomic, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Fetch the submodules recursivly...
add_custom_target(UPDATE_SUBMODULES ALL 
	COMMAND git submodule update --init --recursive 
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modules/wgpu
)

# Compile the wgpu library...
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/wgpu/dist)
	add_custom_target(BUILD_WGPU ALL 
		COMMAND cargo build && cargo build --release
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modules/wgpu
	)
endif()

if(NOT IOS AND NOT ANDROID)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/glfw)
endif()

# Store into S_FILES variable all .cpp file that was on the src folder
file(GLOB_RECURSE S_FILES ${CMAKE_CURRENT_SOURCE_DIR}/private/**.c)
# Store all .h files in the src folder into a variable named "H_FILES"
file(GLOB_RECURSE H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/private/**.h)

if(APPLE)
    file(GLOB_RECURSE M_FILES ${CMAKE_CURRENT_SOURCE_DIR}/private/**.m)
endif()

# Add source to this project's executable.
# add_executable (AtomicExe ${S_FILES} ${H_FILES} ${M_FILES})
add_library(atomic ${S_FILES} ${H_FILES} ${M_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
#   set_property(TARGET atomic PROPERTY CXX_STANDARD 20)
#   set_property(TARGET atomic PROPERTY C_STANDARD 99)
endif()

if(APPLE)
    if(IOS)
        add_definitions(-DWGPU_TARGET=WGPU_TARGET_IOS)
        set(SYSTEM_LIBS UIKit CoreVideo QuartzCore Metal Security)
    else()
        add_definitions(-DWGPU_TARGET=WGPU_TARGET_MACOS)
        set(SYSTEM_LIBS Cocoa CoreVideo IOKit QuartzCore Metal)
    endif()
elseif(ANDROID)
    add_definitions(-DWGPU_TARGET=WGPU_TARGET_ANDROID)
    # TODO: Nedd to add the dependencies for android
elseif(MSVC)
    add_definitions(-DWGPU_TARGET=WGPU_TARGET_WINDOWS)
    set(SYSTEM_LIBS userenv ws2_32 dwmapi dbghelp d3dcompiler D3D12 D3D11 DXGI setupapi Bcrypt)
else()
    if(USE_WAYLAND)
        add_definitions(-DWGPU_TARGET=WGPU_TARGET_LINUX_WAYLAND)
        # TODO: Need to add the dependencies for linux wayland
    else()
        add_definitions(-DWGPU_TARGET=WGPU_TARGET_LINUX_X11)
        # TODO: Need to add the dependencies for linux X11
    endif()
endif()

foreach(lib IN LISTS SYSTEM_LIBS)
    if(MSVC)
        # Don't find the library for windows because
        # don't work 🙁 so if you have a solution feel
        # free to update this code !!
        list(APPEND SYSTEM_LIBS_PATH ${lib})
    else()
        find_library(${lib} ${lib} REQUIRED)
        list(APPEND SYSTEM_LIBS_PATH ${${lib}})
    endif()
endforeach()

# Add third party libs for desktop only.
if(NOT IOS AND NOT ANDROID)
    list(APPEND SYSTEM_LIBS_PATH glfw)
endif()

if(APPLE)
    target_compile_options(atomic PUBLIC -x objective-c++)
endif()

if(NOT MSVC)
    target_compile_options(atomic
        PRIVATE -Wno-error=address-of-temporary 
        PRIVATE -Wno-address-of-temporary
    )
endif()

target_include_directories(atomic 
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/modules/wgpu/ffi"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/modules/stb"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public"
)

target_link_libraries(atomic 
    PRIVATE glfw
    PRIVATE "${SYSTEM_LIBS_PATH}"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/modules/wgpu/target/release/${CMAKE_STATIC_LIBRARY_PREFIX}wgpu_native${CMAKE_STATIC_LIBRARY_SUFFIX}"
)

