cmake_minimum_required(VERSION 4.0)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(SDL3 REQUIRED CONFIG)

add_executable(MyProject main.cpp
        Headers/BasicStructures.h
        Headers/IEventListener.h
        Headers/IRender.h
        Headers/IRule.h
        Headers/Object.h
        Headers/Painter.h
        Headers/SDL_Wizard.h
        Headers/VirtualScreen.h
        Implements/EventListeners/BasicKeyTest.h
        Implements/Renders/MapRenderer.h
        Implements/Renders/ObjectRender.h
        Implements/Rules/MoveRule.h
        Headers/Storage.h
        Headers/World.h
        Implements/Rules/ColliderRule.h
        Implements/PersonalScript/PersonalScriptTest.h
        Headers/IScript.h
        Headers/ICollide.h
        Implements/PrintOnCollide.h
        Implements/HurtOnCollide.h
)

target_include_directories(MyProject PRIVATE
    "./"
)

if(MSVC)
    target_compile_options(MyProject PRIVATE /W4 /w15038)
else ()
    target_compile_options(MyProject PRIVATE -Wall -Wextra)
endif ()

target_link_libraries(MyProject PRIVATE SDL3::SDL3)
