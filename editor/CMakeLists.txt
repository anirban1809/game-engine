file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE SOURCES_MM "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm")
set(EXTERNAL "${CMAKE_CURRENT_SOURCE_DIR}/include/UI/MacOS/FileOpenDialog.mm")


add_executable(release main.cpp Editor.cpp ${EXTERNAL} ${SOURCES} )
target_compile_definitions(release PRIVATE PLATFORM_MAC)

set(ASSIMP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/assimp/include")
set(ASSIMP_BUILD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/assimp/build/include")
set(ASSIMP_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/assimp/build/bin/libassimp.5.dylib")

target_include_directories(release PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/include
${ASSIMP_INCLUDE_DIR}
${ASSIMP_BUILD_INCLUDE_DIR}
)



target_link_libraries(
    release 
    PRIVATE 
    ${ASSIMP_LIBRARY}
    lib
"-framework Cocoa"
)

