cmake_minimum_required(VERSION 3.5)

project(example VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

add_executable(hello-world hello-world.cpp)
target_link_libraries(hello-world user32.lib)
set_property(TARGET hello-world PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_link_options(hello-world PRIVATE "/subsystem:console,3.10")
