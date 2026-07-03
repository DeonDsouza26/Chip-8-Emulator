# CHIP-8 Emulator

A simple CHIP-8 emulator written in **C++** using the **SFML (Simple and Fast Multimedia Library)** for graphics, input, and timing.

## Features

- CHIP-8 CPU implementation
- 4096 bytes of memory
- 16 general-purpose registers (V0-VF)
- Index register (indexRegister)
- Program counter (programCounter)
- Stack and stack pointer
- Delay and sound timers
- 64 × 32 monochrome display
- Hexadecimal keypad (16 keys)
- ROM loading
- Instruction fetch-decode-execute cycle
- SFML-based rendering and keyboard input

## Project Structure

```
Chip-8-Emulator/
  ├── include/
  │   ├── header/
  │   │   └── chip8.hpp
  │   └── SFML/
  │       ├── include/
  │       ├── lib/
  │       ├── bin/
  │       └── .gitkeep
  ├── main.cpp
  ├── CMakeLists.txt
  ├── .gitignore
  ├── README>md
  └── src/
      ├── cycle.cpp
      ├── loadRom.cpp
      └── window.cpp
```

## Requirements

- **Compiler:** A C++ compiler supporting **C++17** or higher (e.g., GCC 9+, Clang 10+, or MSVC 2019+).
- **Build System:** [CMake](https://cmake.org/) (version 3.10 or higher recommended).
- **Graphics Library:** [SFML 3.1.0](https://github.com/sfml/sfml) or higher installed on your system.

## Installing SFML 3.1.0

Since the repository does not include the SFML binaries, you must install or provide **SFML 3.1.0** before building the project.

### Windows (Manual Installation)

1. Download **SFML 3.1.0** from the official SFML website or their github page.
2. Extract the downloaded archive.
3. Copy the extracted `include`, `lib`, and `bin` folders into your project's `include/SFML/` directory so that it matches the following structure:

```text
Chip-8-Emulator/
├── include/
│   └── SFML/
│       ├── include/   <-- SFML headers (SFML/Graphics.hpp, etc.)
│       ├── lib/       <-- .lib or .a files
│       └── bin/       <-- .dll files
├── src/
├── roms/
└── CMakeLists.txt
```

### macOS (Homebrew)

Install SFML using Homebrew:

```bash
brew install sfml
```

### Ubuntu / Debian

Install the development package using `apt`:

```bash
sudo apt update
sudo apt install libsfml-dev
```

> **Note:** This project is developed and tested with **SFML 3.1.0**. Using a different version may require changes to the build configuration or source code.

## Building and Running

### 1. Clone the Repository

```bash
git clone https://github.com/DeonDsouza26/Chip-8-Emulator.git
cd chip8-emulator
```

### 2. Create a Build Directory

```bash
mkdir build
cd build
```

### 3. Configure the Project

```bash
cmake ..
```

### 4. Build the Project

```bash
cmake --build .
```

### 5. Run the Emulator

**Windows**

```powershell
.\Chip8-Emulator.exe
```

**Linux / macOS**

```bash
./Chip8-Emulator
```

> **Note:** ROM files must be placed inside /build folder along with the generated executable file (emulator.exe).

## Controls

| CHIP-8 | Keyboard |
| ------ | -------- |
| 1      | 1        |
| 2      | 2        |
| 3      | 3        |
| C      | 4        |
| 4      | Q        |
| 5      | W        |
| 6      | E        |
| D      | R        |
| 7      | A        |
| 8      | S        |
| 9      | D        |
| E      | F        |
| A      | Z        |
| 0      | X        |
| B      | C        |
| F      | V        |

## Supported Instructions

The emulator implements the standard CHIP-8 instruction set, including:

- Memory operations
- Arithmetic and logic
- Conditional branching
- Stack operations
- Timers
- Display drawing
- Keyboard input
- Random number generation

## Display

- Resolution: **64 × 32 pixels**
- Monochrome graphics
- Pixel scaling handled by SFML

## Timers

The emulator includes:

- Delay Timer (60 Hz)
- Sound Timer (60 Hz)

The timers decrement independently of the CPU execution speed.

## ROMs

Place CHIP-8 ROMs inside the build folder and load them when launching the emulator.

Example ROMs:

- IBM Logo
- Space Invaders
- Brick
- Maze

## Future Improvements

- Super CHIP support
- Configurable CPU speed
- Audio using SFML
- Debugger
- Configurable key bindings

## Screenshots

![Screenshot 1](screenshots/1.png)

![Screenshot 2](screenshots/2.png)

![Screenshot 3](screenshots/3.png)

## References

- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1)
- [CHIP-8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [Tobias V. I. Langhoff's CHIP-8 Guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
- [Johnearnest's Chip8 ROMs archive](https://johnearnest.github.io/chip8Archive/)

## License

This project is licensed under the MIT License.

## Author

Deon Dsouza

GitHub: https://github.com/DeonDsouza26
