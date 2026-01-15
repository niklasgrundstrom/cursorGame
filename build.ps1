# PowerShell build script for Defend The Castle

# Create build directory if it doesn't exist
if (-not (Test-Path build)) {
    New-Item -ItemType Directory -Path build | Out-Null
    Write-Host "Created build directory"
}

# Navigate to build directory
Set-Location build

# Run CMake
Write-Host "Running CMake..."
cmake ..

if ($LASTEXITCODE -eq 0) {
    Write-Host "CMake configuration successful!"
    Write-Host "Building project..."
    cmake --build . --config Release
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Build successful!"
        Write-Host "Executable location: build\Release\DefendTheCastle.exe (or build\DefendTheCastle.exe)"
    } else {
        Write-Host "Build failed!" -ForegroundColor Red
    }
} else {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    Write-Host ""
    Write-Host "If the error is about SFML, you need to install it:" -ForegroundColor Yellow
    Write-Host "1. Download SFML from: https://www.sfml-dev.org/download.php" -ForegroundColor Yellow
    Write-Host "2. Extract to C:\SFML (or another location)" -ForegroundColor Yellow
    Write-Host "3. Run: cmake .. -DSFML_ROOT=C:\SFML" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Or see SETUP.md for detailed instructions." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Make sure CMake is installed and in your PATH" -ForegroundColor Yellow
}

# Return to original directory
Set-Location ..
