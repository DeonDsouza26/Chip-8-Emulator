#include <string>
#include <fstream>
#include <iostream>

// Include Chip8 class from path include/header/chip8.hpp
#include "../header/chip8.hpp"

// Reads the specifed ROM file in to Chip-8's memory, logs errors incase the file cannot be opened or read.
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
    std::cerr << "Error: File size is too large!\n";
    file.close();
    return false;
  }

  file.seekg(0, std::ios::beg);

  file.read(reinterpret_cast<char *>(&memory[0x200]), size);

  file.close();

  return true;
}