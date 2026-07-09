// Include Chip8 class from path include/header/chip8.hpp
#include "chip8.hpp"

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