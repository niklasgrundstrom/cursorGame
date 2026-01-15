@echo off
REM Batch file build script for Defend The Castle

if not exist build mkdir build
cd build

echo Running CMake...
cmake ..

if %ERRORLEVEL% EQU 0 (
    echo CMake configuration successful!
    echo Building project...
    cmake --build . --config Release
    
    if %ERRORLEVEL% EQU 0 (
        echo Build successful!
        echo Executable location: build\Release\DefendTheCastle.exe
    ) else (
        echo Build failed!
    )
) else (
    echo CMake configuration failed!
    echo Make sure CMake is installed and in your PATH
)

cd ..
