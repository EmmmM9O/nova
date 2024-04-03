#pragma once
#include <cctype>
#include <compare>
#include <iostream>
#include <map>
#include <string>

#include "core/Util.hpp"
#include "fmt/core.h"
#include "fmt/format.h"
namespace nova {
class Color {
 public:
  float r, g, b, a;
  Color();
  Color(unsigned int rgba8888);
  Color(const std::string &hex);
  Color(float r, float g, float b, float a);
  void clamp();
  int rgba8888() const;
  int rgba() const;
  static int rgba8888(float r, float g, float b, float a);
  std::string toString() const;
  std::string toConsole() const;
  static float intToFloatColor(int value);
  static int floatToIntColor(float value);
  static std::string clearConsoleColor();
  float toFloatBits() const;
  void set(const Color &color);
  void set(float r, float g, float b, float a);
  void abgr8888(float value);
};
/*
 * to parse color in fmt
 * like {color} to clear f
 * {color:255;255;255} to fill with rgb color
 * {color:white} to use a speacil kind
 * {color:#colors}
 */
enum class colorManagerState {
  finding,
  start,
  rgb,
  word,
  hex,
};
class ConsoleColorManager {
 public:
  Color lastColor;
  // finding -> (find :) start
};
extern ConsoleColorManager colorManager;
std::string to_string(Color);
namespace Colors {
extern Color black, darkGray, gray, lightGray, white, clear, blue, navy, royal,
    slate, sky, cyan, teal

    ,
    green, acid, lime, forest, olive

    ,
    yellow, gold, goldenrod, orange

    ,
    brown, tan, brick

    ,
    red, scarlet, crimson, coral, salmon, pink, magenta

    ,
    purple, violet, maroon;
extern int whiteRgba, clearRgba, blackRgba;
extern float blackFloatBits, clearFloatBits, whiteFloatBits;
extern std::map<std::string, Color> colors;
}  // namespace Colors
}  // namespace nova
template <>
class fmt::formatter<nova::ConsoleColorManager> {
 public:
  using stateType = nova::colorManagerState;
  constexpr format_parse_context::iterator parse(
      format_parse_context &context) {
    auto iter{context.begin()};
    const auto end{context.end()};
    while ((iter != end) && *iter != '}') {
      if (state == nova::colorManagerState::finding) {
        if (*iter == '$') state = stateType::start;
      } else if (state == stateType::start) {
        if (*iter == '#')
          state = stateType::hex;
        else if (std::isdigit(*iter))
          state = stateType::rgb, data += *iter;
        else if (std::isalpha(*iter))
          state = stateType::word, data += *iter;
        else
          throw fmt::format_error{"Invalid Color Type"};
      } else if (state == stateType::rgb) {
        if (*iter == ';') {
          int num = nova::parseInt(data, 0);
          if (num >= 256) {
            throw fmt::format_error{"Invaild rgb Number"};
          }
          if (r == -1) {
            r = num;
          } else if (g == -1) {
            g = num;
          } else if (b == -1) {
            b = num;
          } else {
            break;
          }
          data = "";
        } else {
          if (std::isdigit(*iter))
            data += *iter;
          else {
            if (b == -1) throw fmt::format_error{"Invaild Number"};
            break;
          }
        }
      } else if (state == stateType::hex || state == stateType::word) {
        if (*iter == ' ') break;
        if (!std::isdigit(*iter) && !std::isalpha(*iter) && !(*iter == '_')) {
          throw fmt::format_error{"Invaild string"};
        }
        data += *iter;
      }
      iter++;
    }
    if (state == stateType::rgb) {
      if (data != "") {
        int num = nova::parseInt(data, 0);
        if (num >= 256) {
          throw fmt::format_error{"Invaild rgb Number"};
        }
        if (r == -1) {
          r = num;
        } else if (g == -1) {
          g = num;
        } else if (b == -1) {
          b = num;
        } else {
        }
        data = "";
      }

      if (b == -1) throw fmt::format_error{"RGB lose some"};
      color = nova::Color((float)r / 255.0f, (float)g / 255.0f,
                          (float)b / 255.0f, 1);
    }
    if (state == stateType::hex) {
      color = nova::Color(data);
    }
    if (state == stateType::word) {
      findM = nova::Colors::colors.find(data);
      if (findM == nova::Colors::colors.end()) {
        throw fmt::format_error{"no color " + data};
      }
      color = findM->second;
    }
    if (state == stateType::start) {
      throw fmt::format_error{"Unexpectedly End"};
    }
    clear = (state == stateType::finding);
    return iter;
  }
  fmt::format_context::iterator format(const nova::ConsoleColorManager &manager,
                                       format_context &ctx) const;

 private:
  std::map<std::string, nova::Color>::iterator findM;
  int r = -1, g = -1, b = -1;
  stateType state = stateType::finding;
  std::string data = "";
  nova::Color color;
  bool clear = false;
};
template <>
struct fmt::formatter<nova::Color> : formatter<string_view> {
  format_context::iterator format(const nova::Color &,
                                  format_context &ctx) const;
};
