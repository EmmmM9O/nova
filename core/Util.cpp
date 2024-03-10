#include "Util.hpp"

#include <sstream>
#include <vector>

#include "Log.hpp"
namespace nova {
std::vector<std::string> split(const std::string& str, char delimiter) {
  std::vector<std::string> tokens;
  std::istringstream tokenStream(str);
  std::string token;

  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

int parseInt(std::string str, int def) {
  try {
    return std::stoi(str);
  } catch (const std::invalid_argument& e) {
    Log_error("parseInt error {}", e.what());
    return def;
  } catch (const std::out_of_range& e) {
    Log_error("parseInt error {}", e.what());
    return def;
  }
}

}  // namespace nova
