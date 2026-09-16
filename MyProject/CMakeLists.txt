cmake_minimum_required(VERSION 4.1)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_PREFIX_PATH "C:/myLibs")
find_package(SDL3 REQUIRED CONFIG)

add_executable(MyProject main.cpp
        Headers/Painter.h
        Implements/BasicKeyTest.h
        Headers/BasicStructures.h
        Headers/IEventListener.h
        Headers/IRender.h
        Implements/TileRender.h
        Headers/IRule.h
        Implements/MyRule.h)
target_link_libraries(MyProject PRIVATE SDL3::SDL3)
