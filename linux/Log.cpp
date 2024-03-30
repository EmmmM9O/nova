#include "core/Log.hpp"

#include <iostream>
#include <string>
namespace nova {
void logger::printCosnole(std::string str, LogLevel level) {
  std::cout << str << std::endl;
}
}  // namespace nova
