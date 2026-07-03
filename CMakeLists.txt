cmake_minimum_required(VERSION 3.10)
project(chip-8-emulator)

# Compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependency paths
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/SFML/lib/cmake/SFML")

# Find external packages
find_package(SFML 3.1.0 REQUIRED COMPONENTS Graphics Window System)

# Build targets
add_executable(emulator main.cpp src/cycle.cpp src/loadRom.cpp  src/window.cpp )

# Link dependencies
target_link_libraries(emulator PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)