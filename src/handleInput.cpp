// Include Chip8 class from path include/header/chip8.hpp
#include "chip8.hpp"

// Include External Libraies
#include "imgui.h"
#include "imgui-SFML.h"

// Handles user input for ROMs, mapping host keystrokes to the Chip-8's 16-key array
void Chip8::handleInput(sf::RenderWindow &window)
{
  while (const std::optional event = window.pollEvent())
  {
    ImGui::SFML::ProcessEvent(window, *event);

    if (event->is<sf::Event::Closed>())
    {
      window.close();
    }

    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
      if (keyPressed->code == sf::Keyboard::Key::Escape)
      {
        window.close();
      }
      int key = mapSFMLKeyToChip8(keyPressed->code);

      if (key != -1)
      {
        keypad[key] = 1;
      }
    }

    if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>())
    {
      int key = mapSFMLKeyToChip8(keyReleased->code);

      if (key != -1)
      {
        keypad[key] = 0;
      }
    }
  }
}