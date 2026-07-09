#include <random>

// Include Chip8 class from path include/header/chip8.hpp
#include "chip8.hpp"

// Executes a single Chip-8 CPU cycle: fetches, decodes, and executes the next opcode.
void Chip8::cycle()
{
  // Combining 2 8-bits into 1 single 16-bit, because CHIP-8 opcodes are 16-bit long
  uint16_t opcode = (memory[programCounter] << 8) | memory[programCounter + 1];

  programCounter += 2;

  // Decode
  uint8_t type = (opcode & 0xF000) >> 12;
  uint8_t x = (opcode & 0xF00) >> 8;
  uint8_t y = (opcode & 0x0F0) >> 4;
  uint8_t n = (opcode & 0x00F);
  uint8_t nn = (opcode & 0x0FF);
  uint16_t nnn = (opcode & 0x0FFF);

  // Execute
  switch (type)
  {
  case 0x0:
  {
    // Clear the Display
    if (nn == 0xE0)
    {
      for (uint16_t i = 0; i < 64 * 32; ++i)
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
      // Bitwise OR: Sets bit to 1 if it's 1 in either register.
      registers[x] = registers[x] | registers[y];
      break;
    }
    case 0x02:
    {
      // Bitwise AND: Sets bit to 1 only if it's 1 in both registers
      registers[x] = registers[x] & registers[y];
      break;
    }

    case 0x03:
    {
      // Bitwise XOR: Sets bit to 1 if bits are different; 0 if they are identical
      registers[x] = registers[x] ^ registers[y];
      break;
    }

    case 0x04:
    {
      // ADD with carry: Set VF (registers[15]) to 1 if result overflows (> 255)
      uint16_t temp = registers[x] + registers[y];
      temp > 0xFF ? registers[15] = 1 : registers[15] = 0;
      registers[x] = temp;
      break;
    }

    case 0x05:
    {
      // SUB (X - Y): Set VF to 1 if there is NO borrow (X > Y)
      registers[x] > registers[y] ? registers[15] = 1 : registers[15] = 0;
      registers[x] = registers[x] - registers[y];
      break;
    }

    case 0x06:
    {
      // Shift Right: Store the least significant bit in VF, then divide X by 2
      (registers[x] & 1) == 0 ? registers[15] = 0 : registers[15] = 1;
      registers[x] >>= 1;
      break;
    }

    case 0x07:
    {
      // SUBN (Y - X): Set VF to 1 if there is NO borrow (Y > X)
      registers[y] > registers[x] ? registers[15] = 1 : registers[15] = 0;
      registers[x] = registers[y] - registers[x];
      break;
    }

    case 0x0E:
    {
      // Shift Left: Store the most significant bit in VF, then multiply X by 2
      ((registers[x] >> 7) & 1) == 1 ? registers[15] = 1 : registers[15] = 0;
      registers[x] <<= 1;
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
    // CXNN: Generate a random number (0-255) and mask it with the NN byte
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint16_t> ran(0, 255);

    uint8_t randByte = ran(gen);
    registers[x] = randByte & nn;
    break;
  }

  case 0x0D:
  {
    // DXYN: Draw an 8-pixel wide sprite at (X, Y) coordinates for N rows
    uint8_t startX = registers[x] % 64; // Wrap start coordinate to screen width
    uint8_t startY = registers[y] % 32; // Wrap start coordinate to screen height

    registers[15] = 0; // Clear collision flag (VF = 0)

    for (uint16_t i = 0; i < n; ++i)
    {
      // Stop drawing if sprite goes off the bottom of the screen
      if (startY + i >= 32)
        break;

      uint8_t spriteByte = memory[indexRegister + i]; // Fetch 1 byte (row) of sprite data

      for (uint8_t j = 0; j < 8; ++j)
      {
        // Skip current pixel if it goes off the right of the screen
        if (startX + j >= 64)
          continue;

        // Isolate the specific bit (pixel) from the current sprite byte
        uint8_t bit = (spriteByte >> (7 - j)) & 1;

        if (bit == 1)
        {
          uint16_t index = (startX + j) + ((startY + i) * 64); // 1D array index mapping

          // If screen pixel is already on, a collision occurs (XOR will turn it off)
          if (display[index] == 1)
          {
            registers[15] = 1; // Set collision flag (VF = 1)
          }
          display[index] ^= 1; // XOR the sprite pixel onto the display
        }
      }
    }

    drawFlag = true; // Trigger screen refresh loop
    break;
  }

  case 0x0E:
  {
    if (nn == 0x9E)
    {
      // EX9E: Skip the next instruction if the key stored in VX is currently pressed
      if (keypad[registers[x]] == 1)
        programCounter += 2;
    }
    else if (nn == 0xA1)
    {
      // EXA1: Skip the next instruction if the key stored in VX is NOT pressed
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