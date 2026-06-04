@echo off
echo Building ThreadBinaryTree...

mkdir build 2>nul
cd build
cmake ..
cmake --build . --config Debug

if exist Debug\ThreadBinaryTree.exe (
    echo.
    echo Running ThreadBinaryTree...
    echo ==============================
    Debug\ThreadBinaryTree.exe
    echo.
    echo ==============================
    pause
) else (
    echo Build failed!
    pause
)

cd ..