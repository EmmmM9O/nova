#include "Color.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "core/Util.hpp"
#include "fmt/core.h"
#include "fmt/format.h"
namespace nova {
std::string to_string(Color c) { return c.toConsole(); }
Color::Color() {}
int Color::rgba8888() const {
  return ((int)(r * 255) << 24) | ((int)(g * 255) << 16) |
         ((int)(b * 255) << 8) | (int)(a * 255);
}
int Color::rgba8888(float r, float g, float b, float a) {
  return ((int)(r * 255) << 24) | ((int)(g * 255) << 16) |
         ((int)(b * 255) << 8) | (int)(a * 255);
}
Color::Color(unsigned int rgba8888) {
  r = ((rgba8888 & 0xff000000) >> 24) / 255.0f;
  g = ((rgba8888 & 0x00ff0000) >> 16) / 255.0f;
  b = ((rgba8888 & 0x0000ff00) >> 8) / 255.0f;
  a = ((rgba8888 & 0x000000ff)) / 255.0f;
  clamp();
}
float Color::toFloatBits() const {
  int color = ((int)(255 * a) << 24) | ((int)(255 * b) << 16) |
              ((int)(255 * g) << 8) | ((int)(255 * r));
  return intToFloatColor(color);
}
float Color::intToFloatColor(int value) {
  return intBitsToFloat(value & 0xfeffffff);
}
int Color::floatToIntColor(float value) { return floatToRawIntBits(value); }
int Color::rgba() const { return rgba8888(); }
void Color::set(const Color& color) {
  r = color.r;
  b = color.b;
  g = color.g;
  a = color.a;
}
void Color::set(float r, float g, float b, float a) {
  this->r = r;
  this->g = g;
  this->b = b;
  this->a = a;
}
void Color::abgr8888(float value) {
  int c = floatToIntColor(value);
  a = ((c & 0xff000000) >> 24) / 255.0f;
  b = ((c & 0x00ff0000) >> 16) / 255.0f;
  g = ((c & 0x0000ff00) >> 8) / 255.0f;
  r = ((c & 0x000000ff)) / 255.0f;
}
std::string Color::toString() const {
  std::stringstream ss;
  ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(r)
     << std::setw(2) << static_cast<int>(g) << std::setw(2)
     << static_cast<int>(b) << std::setw(2) << static_cast<int>(a);
  return ss.str();
}
Color::Color(const std::string& hex) {
  auto str = hex;
  if (hex.starts_with("#")) str = hex.substr(1);
  std::stringstream iss;
  iss << std::hex << str;
  unsigned int color;
  iss >> color;
  if (str.length() == 6) {
    r = (color >> 16) & 0xFF;
    g = (color >> 8) & 0xFF;
    b = color & 0xFF;
  } else if (str.length() == 8) {
    a = (color >> 24) & 0xFF;
    r = (color >> 16) & 0xFF;
    g = (color >> 8) & 0xFF;
    b = color & 0xFF;
  } else {
    throw std::invalid_argument(
        "Invalid hex color code. It should be 6 or 8 characters long.");
  }
  r /= 255.0f;
  g /= 255.0f;
  b /= 255.0f;
  a /= 255.0f;

  clamp();
}
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
  clamp();
}
void Color::clamp() {
  if (r < 0)
    r = 0;
  else if (r > 1)
    r = 1;

  if (g < 0)
    g = 0;
  else if (g > 1)
    g = 1;

  if (b < 0)
    b = 0;
  else if (b > 1)
    b = 1;

  if (a < 0)
    a = 0;
  else if (a > 1)
    a = 1;
}
ConsoleColorManager colorManager;
namespace Colors {
Color black = Color(0, 0, 0, 1), darkGray = Color(0x3f3f3fff),
      gray = Color(0x7f7f7fff), lightGray = Color(0xbfbfbfff),
      white = Color(1, 1, 1, 1), clear = Color(0, 0, 0, 0),
      blue = Color(0, 0, 1, 1), navy = Color(0, 0, 0.5f, 1),
      royal = Color(0x4169e1ff), slate = Color(0x708090ff),
      sky = Color(0x87ceebff), cyan = Color(0, 1, 1, 1),
      teal = Color(0, 0.5f, 0.5f, 1)

          ,
      green = Color(0x00ff00ff), acid = Color(0x7fff00ff),
      lime = Color(0x32cd32ff), forest = Color(0x228b22ff),
      olive = Color(0x6b8e23ff)

          ,
      yellow = Color(0xffff00ff), gold = Color(0xffd700ff),
      goldenrod = Color(0xdaa520ff), orange = Color(0xffa500ff)

                                         ,
      brown = Color(0x8b4513ff), tan = Color(0xd2b48cff),
      brick = Color(0xb22222ff)

          ,
      red = Color(0xff0000ff), scarlet = Color(0xff341cff),
      crimson = Color(0xdc143cff), coral = Color(0xff7f50ff),
      salmon = Color(0xfa8072ff), pink = Color(0xff69b4ff),
      magenta = Color(1, 0, 1, 1)

          ,
      purple = Color(0xa020f0ff), violet = Color(0xee82eeff),
      maroon = Color(0xb03060ff), betterGreen = Color("38d667"),
      betterRed = Color("e55454");
int whiteRgba = white.rgba(), clearRgba = clear.rgba(),
    blackRgba = black.rgba();
float blackFloatBits = white.toFloatBits(),
      clearFloatBits = clear.toFloatBits(),
      whiteFloatBits = white.toFloatBits();
std::map<std::string, Color> colors = {
    {"clear", clear},
    {"black", black},

    {"white", white},
    {"lightGray", lightGray},
    {"gray", gray},
    {"dark_gray", darkGray},
    {"lightGrey", lightGray},
    {"grey", gray},
    {"darkGrey", darkGray},

    {"blue", royal},
    {"navy", navy},
    {"royal", royal},
    {"slate", slate},
    {"sky", sky},
    {"cyan", cyan},
    {"teal", teal},

    {"green", betterGreen},
    {"acid", acid},
    {"lime", lime},
    {"forest", forest},
    {"olive", olive},

    {"yellow", yellow},
    {"gold", gold},
    {"goldenrod", goldenrod},
    {"orange", orange},

    {"brown", brown},
    {"tan", tan},
    {"brick", brick},

    {"red", betterRed},
    {"scarlet", scarlet},
    {"crimson", crimson},
    {"coral", coral},
    {"salmon", salmon},
    {"pink", pink},
    {"magenta", magenta},

    {"purple", purple},
    {"violet", violet},
    {"maroon", maroon},
};
}  // namespace Colors
}  // namespace nova
fmt::format_context::iterator fmt::formatter<nova::Color>::format(
    const nova::Color& color, format_context& ctx) const {
  return fmt::format_to(ctx.out(), "{}", color.toString());
}
fmt::format_context::iterator fmt::formatter<nova::ConsoleColorManager>::format(
    const nova::ConsoleColorManager& manager, format_context& ctx) const {
  return fmt::format_to(
      ctx.out(), "{}",
      (clear ? nova::Color::clearConsoleColor() : color.toString()));
}
