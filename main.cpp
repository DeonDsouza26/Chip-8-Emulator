#include <iostream>
#include <string>

// Include Chip8 class from path include/header/chip8.hpp
#include "./include/header/chip8.hpp"

int main()
{
  Chip8 vm;
  while (true)
  {
    std::string fileName;
    std::cout << "Enter the file name of your Chip-8 Rom with extention(Example: pong.ch8): ";
    std::cin >> fileName;
    std::cout << "\n";

    bool loaded = vm.loadRom(fileName);
    if (loaded)
      break;
  }

  vm.loadWindow();

  return 0;
}