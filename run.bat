@echo off
REM Compilation script for ThreadBinaryTree

REM Set compilation flags
set CC=gcc
set CFLAGS=-std=c11 -Wall -Wextra

REM Source files
set SOURCES=main.c body_threadbinary.c

REM Header files
set HEADERS=threadbinary.h

REM Output executable name
set EXECUTABLE=ThreadBinaryTree.exe

REM Compile
%CC% %CFLAGS% %SOURCES% -o %EXECUTABLE%

REM Check if compilation was successful
if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running %EXECUTABLE%...
    echo.
    %EXECUTABLE%
) else (
    echo Compilation failed!
    pause
)
