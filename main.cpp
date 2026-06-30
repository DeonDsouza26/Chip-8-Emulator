#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <random>

#include <SFML/Graphics.hpp>

class Chip8
{
public:
  bool loadRom(std::string fileName);
  void loadWindow();
  void cycle();

  uint8_t display[64 * 32];
  uint8_t keypad[16];

  Chip8()
  {
    // Initialise default values
    for (uint16_t i = 0; i < 64 * 32; ++i)
      display[i] = 0;
    for (uint8_t i = 0; i < 16; ++i)
      keypad[i] = 0;
    for (uint16_t i = 0; i < 0x1000; ++i)
      memory[i] = 0;
    for (uint8_t i = 0; i < 16; ++i)
      registers[i] = 0;
    for (uint8_t i = 0; i < 16; ++i)
      stack[i] = 0;

    indexRegister = 0;
    programCounter = 0x200;
    stackPointer = 0;

    delayTimer = 0;
    soundTimer = 0;

    // Set up font in first 0x50 (80) in memory
    for (uint8_t i = 0; i < 0x50; ++i)
    {
      memory[i] = fontset[i];
    }
  }

private:
  uint8_t memory[0x1000];
  uint8_t registers[16];
  uint16_t indexRegister;
  uint16_t programCounter;

  uint16_t stack[16];
  uint8_t stackPointer;

  uint8_t delayTimer;
  uint8_t soundTimer;

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

bool Chip8::loadRom(std::string fileName)
{
  std::ifstream file(fileName, std::ios::binary | std::ios::ate);

  if (!file.is_open())
  {
    std::cerr << "File not found or couldn't be opened\n";
    file.close();
    return false;
  }

  uint16_t fixedSize = 0x1000 - 0x200;

  std::streamsize size = file.tellg();

  if (size > fixedSize)
  {
    std::cerr << "Error type 1: File size is too large!\n";
    file.close();
    return false;
  }

  file.seekg(0, std::ios::beg);

  file.read(reinterpret_cast<char *>(&memory[0x200]), size);

  file.close();

  return true;
}

void Chip8::loadWindow()
{
  static constexpr int width = 64;
  static constexpr int height = 32;
  static constexpr int scale = 16;

  sf::RenderWindow window(sf::VideoMode({width * scale, height * scale}), "Chip-8-Emulator");

  sf::RectangleShape pixel(sf::Vector2f(scale, scale));

  while (window.isOpen())
  {
    while (const std::optional event = window.pollEvent())
    {
      if (event->is<sf::Event::Closed>())
      {
        window.close();
      }
    }

    window.setFramerateLimit(60);

    cycle();

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

void Chip8::cycle()
{
  // Combining 2 8-bits into 1 single 16-bit
  uint16_t opcode = (memory[programCounter] << 8) | memory[programCounter + 1];

  // std::cout << "PC: 0x" << std::hex << (int)programCounter << " | Opcode: 0x" << opcode << "\n";

  programCounter += 2;

  // Decode
  uint8_t type = (opcode & 0xF000) >> 12;
  uint8_t x = (opcode & 0xF00) >> 8;
  uint8_t y = (opcode & 0x0F0) >> 4;
  uint8_t n = (opcode & 0x00F);
  uint8_t nn = (opcode & 0x0FF);
  uint16_t nnn = (opcode & 0x0FFF);

  switch (type)
  {
  case 0x0:
  {
    if (nn == 0xE0)
    {
      for (uint8_t i = 0; i < 64 * 32; ++i)
      {
        display[i] = 0;
      }
    }
    else if (nn == 0xEE)
    {
      --stackPointer;
      programCounter = stack[stackPointer];
    }
    break;
  }

  case 0x1:
  {
    programCounter = nnn;
    break;
  }

  case 0x2:
  {
    stack[stackPointer] = programCounter;
    ++stackPointer;
    programCounter = nnn;
    break;
  }

  case 0x3:
  {
    if (registers[x] == nn)
    {
      programCounter += 2;
    }
    break;
  }

  case 0x4:
  {
    if (registers[x] != nn)
    {
      programCounter += 2;
    }
    break;
  }

  case 0x5:
  {
    if (registers[x] == registers[y])
    {
      programCounter += 2;
    }
    break;
  }

  case 0x6:
  {
    registers[x] = nn;
    break;
  }

  case 0x7:
  {
    registers[x] += nn;
    break;
  }

  case 0x8:
  {
    switch (n)
    {
    case 0x00:
    {
      registers[x] = registers[y];
      break;
    }

    case 0x01:
    {
      registers[x] = registers[x] | registers[y];
      break;
    }

    case 0x02:
    {
      registers[x] = registers[x] & registers[y];
      break;
    }

    case 0x03:
    {
      registers[x] = registers[x] ^ registers[y];
      break;
    }

    case 0x04:
    {
      uint16_t temp = registers[x] + registers[y];
      temp > 0xFF ? registers[15] = 1 : registers[15] = 0;
      registers[x] = temp;
      break;
    }

    case 0x05:
    {
      registers[x] > registers[y] ? registers[15] = 1 : registers[15] = 0;
      registers[x] = registers[x] - registers[y];
      break;
    }

    case 0x06:
    {
      (registers[x] & 1) == 0 ? registers[15] = 0 : registers[15] = 1; // or registers[15] = registers[x] & 1
      registers[x] >> 2;                                               // or registers[x] /= 2
      break;
    }

    case 0x07:
    {
      registers[y] > registers[x] ? registers[15] = 1 : registers[15] = 0;
      registers[x] = registers[y] - registers[x];
      break;
    }

    case 0x0E:
    {
      ((registers[x] >> 7) & 1) == 1 ? registers[15] = 1 : registers[15] = 0; // or registers[15] = (registers[x] >> 7) & 1
      registers[x] << 1;                                                      // or registers[x] *= 2
      break;
    }
    }
  }

  case 0x09:
  {
    if (registers[x] != registers[y])
      programCounter += 2;
    break;
  }

  case 0x0A:
  {
    indexRegister = nnn;
    break;
  }

  case 0x0B:
  {
    programCounter = nnn + registers[0];
    break;
  }

  case 0x0C:
  {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint16_t> ran(0, 255);

    uint8_t randByte = ran(gen);
    registers[x] = randByte & nn;
    break;
  }

  case 0x0D:
  {
    uint8_t startX = registers[x] % 64;
    uint8_t startY = registers[y] % 32;

    registers[15] = 0;

    for (uint16_t i = 0; i < n; ++i)
    {
      if (startY + i >= 32)
        break;

      uint8_t spriteByte = memory[indexRegister + i];

      for (uint8_t j = 0; j < 8; ++j)
      {
        if (startX + j >= 64)
          continue;

        uint8_t bit = (spriteByte >> (7 - j)) & 1;

        if (bit == 1)
        {
          uint16_t index = (startX + j) + ((startY + i) * 64);

          if (display[index] == 1)
          {
            registers[15] = 1;
          }
          display[index] ^= 1;
        }
      }
    }

    drawFlag = true;
    break;
  }

  case 0x0E:
  {
    if (nn == 0x9E)
    {
      if (keypad[registers[x]] == 1)
        programCounter += 2;
    }
    else if (nn == 0xA1)
    {
      if (keypad[registers[x]] == 0)
        programCounter += 2;
    }
    break;
  }

  case 0x0F:
  {
    switch (nn)
    {
    case 0x07:
    {
      registers[x] = delayTimer;
      break;
    }

    case 0x0A:
    {
      bool keyPressed = false;
      for (uint8_t i = 0; i < 16; ++i)
      {
        if (keypad[i] == 1)
        {
          registers[x] = i;
          keyPressed = true;
          break;
        }
      }
      if (!keyPressed)
        programCounter -= 2;
      break;
    }

    case 0x15:
    {
      delayTimer = registers[x];
      break;
    }

    case 0x18:
    {
      soundTimer = registers[x];
      break;
    }

    case 0x1E:
    {
      indexRegister += registers[x];
      break;
    }

    case 0x29:
    {
      indexRegister = 0 + registers[x] * 5;
      break;
    }

    case 0x33:
    {
      uint8_t temp = registers[x];
      memory[indexRegister] = (temp / 100) % 10;
      memory[indexRegister + 1] = (temp / 10) % 10;
      memory[indexRegister + 2] = temp % 10;
      break;
    }

    case 0x55:
    {
      for (uint16_t i = 0; i <= x; ++i)
      {
        memory[indexRegister + i] = registers[i];
      }
      break;
    }

    case 0x65:
    {
      for (uint16_t i = 0; i <= x; ++i)
      {
        registers[i] = memory[indexRegister + i];
      }
      break;
    }
    }
  }
  }
}

int main()
{
  Chip8 vm;
  while (true)
  {
    std::string fileName;
    std::cout << "Enter the file name of you Chip-8 Rom with extention(Example: pong.ch8): \t";
    std::cin >> fileName;
    std::cout << "\n";

    bool loaded = vm.loadRom(fileName);
    if (loaded)
      break;
  }

  vm.loadWindow();

  return 0;
}