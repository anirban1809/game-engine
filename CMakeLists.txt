cmake_minimum_required(VERSION 3.10)
project(release)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)  # Ensures Debug mode
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")  # Add debug symbols and disable optimizations

set(LOCAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vendor/glew-2.2.0/include")
set(GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/vendor/glew-2.2.0/build/cmake/lib/libGLEW.a")

# Define GLFW Paths
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vendor/glfw-3.4.bin.MACOS/include")
set(GLFW_LIBRARY "${CMAKE_SOURCE_DIR}/vendor/glfw-3.4.bin.MACOS/lib-arm64/libglfw.3.dylib")

# Include GLFW Headers
include_directories(${GLFW_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${LOCAL_INCLUDE_DIR})

# Add Executable
file(GLOB_RECURSE SOURCES "./src/*.cpp")
add_executable(release ${SOURCES} "main.cpp")

target_link_libraries(release
    #link GLEW and GLFW
    ${GLEW_LIBRARY}
    ${GLFW_LIBRARY}
    "-framework OpenGL"
)

