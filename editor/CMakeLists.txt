file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(release main.cpp Editor.cpp ${SOURCES})

target_include_directories(release PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    release 
    PRIVATE 
    lib 
)

