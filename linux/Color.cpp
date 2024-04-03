#include "core/Color.hpp"

#include <string>
namespace nova {
std::string Color::toConsole() const {
  std::string str = "\033[38;2;";
  str += std::to_string((int)(r * 255)) + ";";
  str += std::to_string((int)(g * 255)) + ";";
  str += std::to_string((int)(b * 255)) + "m";
  return str;
}
std::string Color::clearConsoleColor() { return "\033[0m"; }
}  // namespace nova
