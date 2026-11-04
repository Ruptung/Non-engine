# Non-engine

옛날 하드웨어의 제약을 느끼면서 엔진없이 개발을 하고 싶어 시작한 프로젝트이다.

엔진 없이 C++17과 SDL3로 만드는 타일 기반 2D 게임 엔진과, 그 위에서 돌아가는 좀비 생존 게임.

GBA/SNES 그래픽 칩처럼 동작하도록 설계했다. 4x4 타일, 팔레트 인덱스, 레이어,
좌우·상하 반전을 지원하고, 36x36 가상 화면에 그린 뒤 마지막 단계에서 실제 색으로 바꾼다.

파이프라인 : 
- script.start -> ( EventListen -> GameRule -> script.update -> Render ) loop

렌더 파이프라인 :
- TilePainter (타일 -> 팔레트 인덱스) -> VirtualScreen (36x36) -> Painter (팔레트 -> RGB) -> SDL_Wizard (출력)

## 빌드

필요한 것: C++17 컴파일러, CMake 3.16 이상, SDL3

### 1. SDL3 설치

**macOS**

```
xcode-select --install
brew install cmake sdl3
```

**Windows**

```
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
C:\vcpkg\vcpkg install sdl3:x64-windows
```

**Linux** (Ubuntu / Debian)

```
sudo apt update
sudo apt install -y build-essential cmake git pkg-config \
    libasound2-dev libpulse-dev libaudio-dev \
    libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev \
    libxinerama-dev libxss-dev libxkbcommon-dev libwayland-dev \
    libegl1-mesa-dev libgl1-mesa-dev libgles2-mesa-dev

cd ~
git clone --depth 1 --branch release-3.2.x https://github.com/libsdl-org/SDL.git
cmake -S SDL -B SDL/build -DCMAKE_BUILD_TYPE=Release
cmake --build SDL/build -j$(nproc)
sudo cmake --install SDL/build
sudo ldconfig
```

### 2. 빌드와 실행

```
git clone https://github.com/Ruptung/Non-engine.git
cd Non-engine
cmake -S MyProject -B build
cmake --build build
./build/MyProject
```

Windows

```
cmake -S MyProject -B build -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
.\build\Debug\MyProject.exe
```

## 조작

| 키 | 동작 |
| --- | --- |
| 방향키 | 이동 |
| Q | 종료 |

## 구조

- `Headers/` — 엔진 코어
- `Headers/Addons/Tile_Addon/` — 타일 렌더링 에드온
- `Implements/` — 게임 구현

## 현재 상태

- [x] sparse set 기반 ECS
- [x] 타일·팔레트·레이어 렌더링
- [x] 타일맵 충돌, 좀비 추적
- [ ] 게임 상태 (타이틀, 게임 오버, 재시작)
- [ ] ESP32 + OLED 이식