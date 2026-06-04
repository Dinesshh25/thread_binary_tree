cmake_minimum_required(VERSION 3.15)
project(ThreadBinaryTree C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    main.c
    body_threadbinary.c
)

# Header files
set(HEADERS
    threadbinary.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})