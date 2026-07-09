#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <fstream>

// Include Chip8 class from path include/header/chip8.hpp
#include "chip8.hpp"

// Reads the specifed ROM file in to Chip-8's memory, logs errors incase the file cannot be opened or read.
bool Chip8::loadRom()
{
  OPENFILENAME ofn;
  char szFile[260] = {0}; // Buffer for file name

  // Initialize OPENFILENAME
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = "Chip-8 ROM(.ch8)\0*.ch8*\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  // Display the Open dialog box
  if (GetOpenFileName(&ofn) == TRUE)
  {
    std::cout << "Selected file: " << ofn.lpstrFile << std::endl;

    std::ifstream file(ofn.lpstrFile, std::ios::binary | std::ios::ate);

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
  }
  else
  {
    std::cout << "Selection canceled." << std::endl;
    return false;
  }
  return true;
}
