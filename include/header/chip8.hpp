#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>
#include <string>

// Include External Libraies
#include <SFML/Graphics.hpp>

class Chip8
{
public:
  /**
   * @brief Opens the native file picker and loads the selected CHIP-8 ROM.
   *
   * Opens windows native file picker, allowing the user to choose a valid
   * CHIP-8 ROM (.ch8). The ROM is then loaded into memory starting at
   * address 0x200 (512).
   *
   * @return true if the ROM was loaded successfully, false if file selection
   *         was cancelled or ROM couldn't be loaded into memory.
   */
  bool loadRom();

  /**
   * @brief Creates a window and starts the main application loop.
   *
   * Initializes SFML window along with Dear ImGui, processes user input,
   * updates emulator state, renders the CHIP-8 display, and manages ROM
   * loading and emulator shortcut controls.
   */
  void loadWindow();

  /**
   * @brief Processes window and keyboard events
   *
   * Handles SFML window events, closes window when requested,
   * updates CHIP-8 keypad state, and forwards events related to Dear ImGui.
   *
   * @param window The window application to poll events from.
   */
  void handleInput(sf::RenderWindow &window);

  /**
   * @brief Processes emulator keyboard shortcuts.
   *
   * Checks for registered hotkeys and performs the corresponding emulator
   * action, such as loading a ROM, controlling execution, or closing the application.
   *
   * @param window The application window.
   * @param romLoaded Indicates whether a ROM is currently loaded.
   *                  Updated when a ROM is successfully loaded.
   * @param isEmuPaused Indicates whether emulation is paused.
   *                    Updated when the user resumes or pauses execution.
   */
  void listenForHotkeys(sf::RenderWindow &window, bool &romLoaded, bool &isEmuPaused);

  /**
   * @brief Renders and processes the main menu bar.
   *
   * Displays the emulator's main menu and handles user actions such as loading a ROM,
   * controlling execution, and exiting the application.
   *
   * @param window The application window.
   * @param romLoaded Indicates whether a ROM is currently loaded.
   *                  Updated when a ROM is successfully loaded.
   * @param isEmuPaused Indicates whether emulation is paused.
   *                    Updated when the user resumes or pauses execution.
   */
  void updateMenuBar(sf::RenderWindow &window, bool &romLoaded, bool &isEmuPaused);

  /**
   * @brief Renders the emulator home screen.
   *
   * Displays the emulator title and available keyboard shortcuts while no ROM
   * is loaded.
   *
   * @param window The target render window.
   * @param romLoaded Indicates whether a ROM has been loaded.
   *                  Updated when a ROM is successfully loaded.
   */
  void displayHome(sf::RenderWindow &window, bool &romLoaded);

  /**
   * @brief Advances the emulator by one frame.
   *
   * Executes multiple CPU cycles and updates the delay and sound timers while
   * emulation is running.
   *
   * @param romLoaded Indicates whether a ROM is currently loaded.
   *                  Updated when a ROM is successfully loaded.
   * @param isEmuPaused Indicates whether emulation is paused.
   *                    Updated when the user resumes or pauses execution.
   */
  void updateEmulator(bool &isEmuPaused, bool &romLoaded);

  /**
   * @brief Renders the CHIP-8 display.
   *
   * Draws each active pixel from the emulator's framebuffer to the application
   * window using the specified scale.
   *
   * @param window The target render window.
   * @param pixel Rectangle used to render individual pixels.
   * @param menuHeight Height of the menu bar in pixels.
   * @param scale Scale factor applied to each CHIP-8 pixel.
   * @param width Width of the CHIP-8 display.
   * @param height Height of the CHIP-8 display.
   */
  void renderEmulatorDisplay(sf::RenderWindow &window, sf::RectangleShape &pixel, int menuHeight, int scale, int width, int height);

  /**
   * @brief Draws a string centered horizontally in the window.
   *
   * Calculates the width of the string at the given scale and renders
   * it so that it is horizontally centered.
   *
   * @param window Target render window.
   * @param text Text to render.
   * @param y Vertical position of the text.
   * @param scale Scale factor applied to the font.
   */
  void drawCenteredString(sf::RenderWindow &window, const std::string &text, float y, float scale);

  /**
   * @brief Draws a string at the specified position.
   *
   * Renders each supported character using the built-in bitmap font.
   *
   * @param window Target render window.
   * @param text Text to render.
   * @param position Top-left position of the string.
   * @param scale Scale factor applied to the font.
   */
  void drawString(sf::RenderWindow &window, const std::string &text, sf::Vector2f position, float scale);

  /**
   * @brief Map an SFML keyboard key to its corresponding CHIP-8 key
   *
   * Converts SFML keyboard key into equivalent CHIP-8 key, if the key
   * is not mapped to the CHIP-8 keypad, the key is disregarded and -1 is
   * returned.
   *
   * @param key SFML keyboard key to map
   *
   * @return The CHIP-8 keypad index corresponding to SFML keyboard key or -1
   * if the key is not mapped.
   */
  int mapSFMLKeyToChip8(sf::Keyboard::Key key);

  /**
   * @brief Executes a single CHIP-8 instruction.
   *
   * Fetches the next 2 bytes from memory, combines them into a 16-bit
   * opcode, decodes the instructions(opcode) and executes it.
   */
  void cycle();

  Chip8()
  {
    // Set up font in first 0x50 (80) in memory
    for (uint8_t i = 0; i < 0x50; ++i)
    {
      memory[i] = fontset[i];
    }
  }

  uint8_t display[64 * 32] = {0};
  uint8_t keypad[16] = {0};

private:
  uint8_t memory[0x1000] = {0};
  uint8_t registers[16] = {0};
  uint16_t indexRegister = 0;
  uint16_t programCounter = 0x200;

  uint16_t stack[16] = {0};
  uint8_t stackPointer = 0;

  uint8_t delayTimer = 0;
  uint8_t soundTimer = 0;

  bool drawFlag = false;

  const uint8_t fontset[80] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0,
      0x20, 0x60, 0x20, 0x20, 0x70,
      0xF0, 0x10, 0xF0, 0x80, 0xF0,
      0xF0, 0x10, 0xF0, 0x10, 0xF0,
      0x90, 0x90, 0xF0, 0x10, 0x10,
      0xF0, 0x80, 0xF0, 0x10, 0xF0,
      0xF0, 0x80, 0xF0, 0x90, 0xF0,
      0xF0, 0x10, 0x20, 0x40, 0x40,
      0xF0, 0x90, 0xF0, 0x90, 0xF0,
      0xF0, 0x90, 0xF0, 0x10, 0xF0,
      0xF0, 0x90, 0xF0, 0x90, 0x90,
      0xE0, 0x90, 0xE0, 0x90, 0xE0,
      0xF0, 0x80, 0x80, 0x80, 0xF0,
      0xE0, 0x90, 0x90, 0x90, 0xE0,
      0xF0, 0x80, 0xF0, 0x80, 0xF0,
      0xF0, 0x80, 0xF0, 0x80, 0x80};
};

#endif