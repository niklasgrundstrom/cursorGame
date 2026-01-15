# Defend The Castle

A classic tower defense-style game built with C++ and SFML.

## Features

- **Castle Defense**: Protect your castle on the right side of the screen
- **Enemy Waves**: Enemies spawn from the left with progressive difficulty
- **Multiple Enemy Types**: Basic, Fast, Tank, and Flying enemies with different stats
- **Click-to-Damage**: Click enemies to deal damage
- **Defensive Units**: Purchase units that automatically attack nearby enemies
- **Upgrade System**: Upgrade click damage, unit damage, and unit range
- **Currency System**: Earn gold by defeating enemies, spend it on upgrades

## Building

### Prerequisites
- CMake 3.15 or higher
- C++17 compatible compiler
- SFML 2.5 or higher

### Windows (PowerShell)
**Easy way:** Run the provided build script:
```powershell
.\build.ps1
```

**Manual way:**
```powershell
if (-not (Test-Path build)) { New-Item -ItemType Directory -Path build }
cd build
cmake ..
cmake --build . --config Release
```

### Windows (Command Prompt / Batch)
**Easy way:** Run the provided build script:
```cmd
build.bat
```

**Manual way:**
```cmd
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Windows (Visual Studio)
```powershell
mkdir build; cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

### Windows (MinGW)
```powershell
mkdir build; cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

### Linux/Mac
```bash
mkdir build
cd build
cmake ..
make
```

## Running

After building, run the executable from the build directory. The game will attempt to load fonts from:
- `assets/fonts/arial.ttf` (if you create this directory)
- `C:/Windows/Fonts/arial.ttf` (Windows fallback)

## Gameplay

- **Left Click**: Damage enemies or interact with shop
- **Shop**: Located on the left side - buy units and upgrades
- **Waves**: New wave starts every 30 seconds with increasing difficulty
- **Goal**: Survive as long as possible without letting your castle HP reach 0

## Adding New Enemy Types

To add a new enemy type, simply:

1. Create a new class inheriting from `Enemy` in `EnemyTypes.h`:
```cpp
class MyNewEnemy : public Enemy {
public:
    MyNewEnemy(float x, float y);
};
```

2. Implement it in `EnemyTypes.cpp`:
```cpp
MyNewEnemy::MyNewEnemy(float x, float y)
    : Enemy(x, y, speed, hp, damage, flying) {
    // Customize appearance, reward, etc.
}
```

3. Add it to the `createEnemy()` function's spawn logic.

## Project Structure

```
├── CMakeLists.txt
├── include/
│   ├── Castle.h
│   ├── Enemy.h
│   ├── EnemyTypes.h
│   ├── Game.h
│   ├── ResourceManager.h
│   ├── Shop.h
│   └── Unit.h
├── src/
│   ├── Castle.cpp
│   ├── Enemy.cpp
│   ├── EnemyTypes.cpp
│   ├── Game.cpp
│   ├── main.cpp
│   ├── ResourceManager.cpp
│   ├── Shop.cpp
│   └── Unit.cpp
└── README.md
```
