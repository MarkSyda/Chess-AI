# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(ChessAI CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Grab all .cpp files from src/
file(GLOB SOURCES "src/*.cpp")

# Create the executable
add_executable(ChessAI ${SOURCES})

# Put all binaries into bin/ (inside build dir)
set_target_properties(ChessAI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)