#include "Util.hpp"

#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "Log.hpp"
namespace nova {
std::vector<std::string> split(const std::string &str, char delimiter) {
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
  } catch (const std::invalid_argument &e) {
    Log_error("parseInt error {}", e.what());
    return def;
  } catch (const std::out_of_range &e) {
    Log_error("parseInt error {}", e.what());
    return def;
  }
}
float intBitsToFloat(int bits) {
  union {
    float f;
    uint32_t i;
  } converter;

  converter.i = static_cast<uint32_t>(bits);
  return converter.f;
}
int floatToRawIntBits(float value) {
  union {
    float f;
    uint32_t i;
  } converter;

  converter.f = value;
  return static_cast<int>(converter.i);
}
std::string readFile(const std::filesystem::path &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("no file " + path.string());
  }
  std::streamsize size = file.tellg();
  file.seekg(0, file.beg);
  std::string buffer(static_cast<std::size_t>(size), '\0');
  if (file.read(&buffer[0], size))
    return buffer;
  else
    throw std::runtime_error("file read error" + path.string());
}
}  // namespace nova
