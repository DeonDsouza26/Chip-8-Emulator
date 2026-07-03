#include <SFML/Graphics.hpp>

// Include Chip8 class from path include/header/chip8.hpp
#include "../header/chip8.hpp"

// Initializes the application window, renders the current frame pixels, and displays the updated frame.
void Chip8::loadWindow()
{
  static constexpr int width = 64;
  static constexpr int height = 32;
  static constexpr int scale = 16;

  sf::RenderWindow window(sf::VideoMode({width * scale, height * scale}), "Chip-8-Emulator");

  sf::RectangleShape pixel(sf::Vector2f(scale, scale));

  while (window.isOpen())
  {
    window.setFramerateLimit(60);

    handleInput(window);

    for (int i = 0; i < 10; i++)
    {
      cycle(window);
    }

    if (delayTimer > 0)
      delayTimer--;

    if (soundTimer > 0)
      soundTimer--;

    if (drawFlag)
    {
      window.clear(sf::Color::Black);

      for (int y = 0; y < height; ++y)
      {
        for (int x = 0; x < width; ++x)
        {
          if (display[y * width + x])
          {
            pixel.setPosition(
                sf::Vector2f(x * scale, y * scale));

            window.draw(pixel);
          }
        }
      }
      window.display();
      drawFlag = false;
    }
  }
}

// Handles user input for ROMs, mapping host keystrokes to the Chip-8's 16-key array
void Chip8::handleInput(sf::RenderWindow &window)
{
  while (const std::optional event = window.pollEvent())
  {
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

// Translates a host system keystroke into a Chip-8 key value (0x0 - 0xF).
int Chip8::mapSFMLKeyToChip8(sf::Keyboard::Key key)
{
  switch (key)
  {
  case sf::Keyboard::Key::Num1:
    return 0x1;
    break;

  case sf::Keyboard::Key::Num2:
    return 0x2;
    break;

  case sf::Keyboard::Key::Num3:
    return 0x3;
    break;

  case sf::Keyboard::Key::Num4:
    return 0xC;
    break;

  case sf::Keyboard::Key::Q:
    return 0x4;
    break;

  case sf::Keyboard::Key::W:
    return 0x5;
    break;

  case sf::Keyboard::Key::E:
    return 0x6;
    break;

  case sf::Keyboard::Key::R:
    return 0xD;
    break;

  case sf::Keyboard::Key::A:
    return 0x7;
    break;

  case sf::Keyboard::Key::S:
    return 0x8;
    break;

  case sf::Keyboard::Key::D:
    return 0x9;
    break;

  case sf::Keyboard::Key::F:
    return 0xE;
    break;

  case sf::Keyboard::Key::Z:
    return 0xA;
    break;

  case sf::Keyboard::Key::X:
    return 0x0;
    break;

  case sf::Keyboard::Key::C:
    return 0xB;
    break;

  case sf::Keyboard::Key::V:
    return 0xF;
    break;

  default:
    return -1;
  }
}