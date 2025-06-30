cmake_minimum_required(VERSION 3.31)
project(hello_glfw)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(external/glfw)
add_subdirectory(external/glew/build/cmake)

add_executable(hello_glfw main.cpp)

target_include_directories(hello_glfw PRIVATE external/glfw/include)
target_link_libraries(hello_glfw PRIVATE glfw)

target_include_directories(hello_glfw PRIVATE external/glew/include)
target_link_libraries(hello_glfw PRIVATE glew)


if(APPLE)
    target_link_libraries(hello_glfw PRIVATE "-framework Cocoa" "-framework OpenGL" "-framework IOKit" "-framework CoreVideo")
elseif(WIN32)
    # Usually nothing extra is needed if glfw is built correctly.
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(hello_glfw PRIVATE ${OPENGL_gl_LIBRARY} dl pthread X11 Xrandr Xinerama Xcursor Xxf86vm Xi)
endif()
