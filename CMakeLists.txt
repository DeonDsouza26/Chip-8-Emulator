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
add_executable(emulator WIN32 
    main.cpp 
    src/cycle.cpp 
    src/handleInput.cpp
    src/loadRom.cpp  
    src/mapSFMLKeyToChip8.cpp
    src/menu.cpp
    src/window.cpp 
    
    include/ImGui/imgui_demo.cpp
    include/ImGui/imgui_draw.cpp
    include/ImGui/imgui_tables.cpp
    include/ImGui/imgui_widgets.cpp
    include/ImGui/imgui.cpp

    include/ImGui-SFML/imgui-SFML.cpp
)

# Link dependencies
target_include_directories(emulator PRIVATE
    include/ImGui
    include/Imgui-SFML
)

target_include_directories(emulator PRIVATE
    include/header
)

target_link_libraries(emulator PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

target_link_libraries(emulator PRIVATE opengl32)

if (WIN32)
    add_custom_command(TARGET emulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:emulator>
            $<TARGET_FILE_DIR:emulator>
        COMMAND_EXPAND_LISTS
    )
endif()