# lib/CMakeLists.txt

set(LOCAL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(GLEW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glew-2.2.0/include")
set(IMGUI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui")
set(GLEW_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glew-2.2.0/lib/libGLEW.a")
# Define GLFW Paths
set(GLFW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glfw-3.4.bin.MACOS/include")
set(GLFW_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glfw-3.4.bin.MACOS/lib-arm64/libglfw.3.dylib")

file(GLOB_RECURSE SOURCES_LOCAL "${CMAKE_CURRENT_SOURCE_DIR}/../lib/src/*.cpp")
file(GLOB_RECURSE SOURCES_DEPS "${IMGUI_INCLUDE_DIR}/*.cpp")

# Create a static library instead of shared
add_library(lib STATIC ${SOURCES_LOCAL} ${SOURCES_DEPS})

# Public headers
target_include_directories(lib PUBLIC
 ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(lib PUBLIC
    ${GLEW_LIBRARY}
    ${GLFW_LIBRARY}
    "-framework OpenGL"
)