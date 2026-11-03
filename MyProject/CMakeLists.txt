cmake_minimum_required(VERSION 4.0)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(SDL3 REQUIRED CONFIG)

add_executable(MyProject main.cpp
        Headers/BasicStructures.h
        Headers/Interfaces/IEventListener.h
        Headers/Interfaces/IRender.h
        Headers/Interfaces/IRule.h
        Headers/Object.h
        Headers/Painter.h
        Headers/SDL_Wizard.h
        Headers/VirtualScreen.h
        Implements/EventListeners/BasicKeyTest.h
        Headers/Addons/Tile_Addon/TileMapRenderer.h
        Headers/Addons/Tile_Addon/TileObjectRender.h
        Headers/Storage.h
        Headers/World.h
        Implements/Rules/ColliderRule.h
        Implements/PersonalScript/PlayerMoveScript.h
        Headers/Interfaces/IScript.h
        Headers/Interfaces/ICollide.h
        Implements/Colliders/PrintOnCollide.h
        Implements/Colliders/HurtOnCollide.h
        Headers/Addons/Tile_Addon/TileSystem.h
        Headers/Addons/Tile_Addon/TilePainter.h
        Headers/InterfaceView.h
        Implements/PersonalScript/HordeBehavior.h
        Implements/Rules/PhysicsRule.h
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
