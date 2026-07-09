#include <iostream>

// Include External Libraies
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

// Include Chip8 class from path include/header/chip8.hpp
#include "chip8.hpp"

// Initializes the application window, renders the current frame pixels, and displays the updated frame.
void Chip8::loadWindow()
{
  static constexpr int width = 64;
  static constexpr int height = 32;
  static constexpr int scale = 16;
  const int menuHeight = 20;
  bool romLoaded = false;
  sf::Clock deltaClock;
  bool isEmuPaused = false;

  sf::RenderWindow window(sf::VideoMode({width * scale, (height * scale) + menuHeight}), "Chip-8-Emulator");
  ImGui::SFML::Init(window);
  window.requestFocus();

  window.setFramerateLimit(60);

  sf::RectangleShape pixel(sf::Vector2f(scale, scale));
  while (window.isOpen())
  {
    handleInput(window);

    ImGui::SFML::Update(window, deltaClock.restart());

    listenForHotkeys(window, romLoaded, isEmuPaused);

    updateMenuBar(window, romLoaded, isEmuPaused);

    window.clear(sf::Color::Black);

    displayHome(window, romLoaded);

    updateEmulator(isEmuPaused, romLoaded);

    renderEmulatorDisplay(window, pixel, menuHeight, scale, width, height);

    ImGui::SFML::Render(window);
    window.display();
    drawFlag = false;
  }
  ImGui::SFML::Shutdown();
}

/*
============================================= HELPER FUNCTIONS =============================================
*/

void Chip8::listenForHotkeys(sf::RenderWindow &window, bool &romLoaded, bool &isEmuPaused)
{
  if (ImGui::IsKeyPressed((ImGuiKey)ImGuiKey_L, false))
  {
    std::cout << "L key pressed\nSelecting ROM file\n";
    romLoaded = loadRom();
  }
  if (ImGui::IsKeyPressed((ImGuiKey)ImGuiKey_F1, false))
  {
    isEmuPaused = true;
    std::cout << "F1 key pressed\nGame resumed\n";
  }
  if (ImGui::IsKeyPressed((ImGuiKey)ImGuiKey_F2, false))
  {
    isEmuPaused = false;
    std::cout << "F2 key pressed\nGame paused\n";
  }
  if (ImGui::IsKeyPressed((ImGuiKey)ImGuiKey_Escape, false))
  {
    window.close();
  }
}

void Chip8::updateMenuBar(sf::RenderWindow &window, bool &romLoaded, bool &isEmuPaused)
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::MenuItem("Load Rom (L)"))
    {
      romLoaded = loadRom();
    }
    if (ImGui::MenuItem("Play (F2)"))
    {
      isEmuPaused = false;
      std::cout << "Game resumed\n";
    }
    if (ImGui::MenuItem("Pause (F1)"))
    {
      isEmuPaused = true;
      std::cout << "Game paused\n";
    }
    if (ImGui::MenuItem("Exit (ESC)"))
    {
      window.close();
    }
    ImGui::EndMainMenuBar();
  }
}

void Chip8::displayHome(sf::RenderWindow &window, bool &romLoaded)
{
  if (!romLoaded)
  {
    drawCenteredString(window, "EMULATOR", 170.f, 5.f);
    drawCenteredString(window, "CHIP-8", 80.f, 10.f);

    float keyX = 50.f;
    float textX = 200.f;

    drawString(window, "L", {keyX, 280.f}, 4.f);
    drawString(window, "LOAD ROM", {textX, 280.f}, 4.f);

    drawString(window, "F2", {keyX, 315.f}, 4.f);
    drawString(window, "PLAY", {textX, 315.f}, 4.f);

    drawString(window, "F1", {keyX, 350.f}, 4.f);
    drawString(window, "PAUSE", {textX, 350.f}, 4.f);

    drawString(window, "ESC", {keyX, 385.f}, 4.f);
    drawString(window, "EXIT", {textX, 385.f}, 4.f);
  }
}

void Chip8::updateEmulator(bool &isEmuPaused, bool &romLoaded)
{
  if (!isEmuPaused && romLoaded)
  {
    for (int i = 0; i < 10; i++)
    {
      cycle();
    }
    if (delayTimer > 0)
      delayTimer--;
    if (soundTimer > 0)
      soundTimer--;
  }
}

void Chip8::renderEmulatorDisplay(sf::RenderWindow &window, sf::RectangleShape &pixel, int menuHeight, int scale, int width, int height)
{
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      if (display[y * width + x])
      {
        pixel.setPosition(sf::Vector2f(x * scale, (y * scale) + menuHeight));
        window.draw(pixel);
      }
    }
  }
}