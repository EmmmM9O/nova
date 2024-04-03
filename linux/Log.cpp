#include "core/Log.hpp"

#include <iostream>
#include <string>

#include "core/Color.hpp"
#include "fmt/core.h"
namespace nova {
void logger::printCosnole(std::string str, LogLevel level) {
  fmt::print(fmt::runtime(str + "{color}\n"), fmt::arg("color", colorManager));
}
}  // namespace nova
