# SFML Setup Guide

## Option 1: Automatic Download (Easiest)
The CMakeLists.txt will automatically download SFML if it's not found. Just run:
```powershell
.\build.ps1
```
Note: This requires Git and may take a few minutes on first build.

## Option 2: Manual Installation (Recommended for Windows)

### Step 1: Download SFML
1. Go to https://www.sfml-dev.org/download.php
2. Download "SFML 2.6.x for Visual C++" (match your compiler version)
   - Visual Studio 2019: VC++ 16
   - Visual Studio 2022: VC++ 17
   - MinGW: GCC

### Step 2: Extract SFML
Extract to a location like `C:\SFML` or `C:\Libraries\SFML`

### Step 3: Configure CMake
When running CMake, specify the SFML path:
```powershell
cd build
cmake .. -DSFML_DIR="C:\SFML\lib\cmake\SFML"
cmake --build . --config Release
```

Or set it as an environment variable:
```powershell
$env:SFML_DIR = "C:\SFML\lib\cmake\SFML"
cd build
cmake ..
cmake --build . --config Release
```

## Option 3: Using vcpkg (Best for Development)

### Install vcpkg
```powershell
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat
```

### Install SFML
```powershell
.\vcpkg install sfml:x64-windows
```

### Configure CMake with vcpkg
```powershell
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release
```

## Option 4: Quick Test (No Installation)
If you just want to test, the CMakeLists.txt will try to download SFML automatically. Make sure you have Git installed.
