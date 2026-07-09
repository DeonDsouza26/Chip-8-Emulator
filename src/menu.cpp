#include <array>
#include <unordered_map>
#include <string>
#include <cctype>
#include <iostream>

// Include External Libraies
#include <SFML/Graphics.hpp>

// Include Chip8 class from path include/header/chip8.hpp
#include "chip8.hpp"

float getStringWidth(const std::string &text, float scale);
void drawLetter(sf::RenderWindow &target, char c, sf::Vector2f position, float scale);

struct Glyph
{
  std::array<uint8_t, 7> bitmap;
  uint8_t advance;
};

const std::unordered_map<char, Glyph> font = {

    {'A', {{0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}, 6}},
    {'C', {{0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0F}, 6}},
    {'D', {{0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E}, 6}},
    {'E', {{0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F}, 6}},
    {'F', {{0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10}, 6}},
    {'H', {{0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}, 6}},
    {'I', {{0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1F}, 4}},
    {'L', {{0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F}, 6}},
    {'M', {{0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11}, 6}},
    {'O', {{0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}, 6}},
    {'P', {{0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10}, 7}},
    {'R', {{0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11}, 6}},
    {'S', {{0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E}, 6}},
    {'T', {{0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}, 6}},
    {'U', {{0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}, 6}},
    {'X', {{0x11, 0x0A, 0x04, 0x04, 0x04, 0x0A, 0x11}, 6}},
    {'Y', {{0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04}, 6}},
    {'1', {{0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E}, 5}},
    {'2', {{0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F}, 6}},
    {'8', {{0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E}, 6}},
    {'-', {{0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00}, 5}},
    {' ', {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 4}}};

void Chip8::drawCenteredString(sf::RenderWindow &window, const std::string &text, float y, float scale)
{
  float width = getStringWidth(text, scale);

  drawString(window, text, {(window.getSize().x - width) / 2.f, y}, scale);
}

void Chip8::drawString(sf::RenderWindow &window, const std::string &text, sf::Vector2f position, float scale)
{
  float cursor = position.x;

  for (char c : text)
  {
    auto it = font.find(std::toupper(static_cast<unsigned char>(c)));

    if (it == font.end())
      continue;

    drawLetter(window, c, {cursor, position.y}, scale);

    cursor += it->second.advance * scale;
  }
}

float getStringWidth(const std::string &text, float scale)
{
  float width = 0.f;

  for (char c : text)
  {
    auto it = font.find(std::toupper(static_cast<unsigned char>(c)));

    if (it == font.end())
      continue;

    width += it->second.advance * scale;
  }

  return width;
}

void drawLetter(sf::RenderWindow &window, char c, sf::Vector2f position, float scale)
{
  auto it = font.find(std::toupper(static_cast<unsigned char>(c)));

  if (it == font.end())
    return;

  sf::RectangleShape letter({scale, scale});
  letter.setFillColor(sf::Color::White);

  for (int row = 0; row < 7; row++)
  {
    uint8_t bits = it->second.bitmap[row];

    for (int col = 0; col < 5; col++)
    {
      if (bits & (0x10 >> col))
      {
        letter.setPosition({position.x + col * scale, position.y + row * scale});

        window.draw(letter);
      }
    }
  }
}
