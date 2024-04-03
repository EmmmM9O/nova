#pragma once
#include <filesystem>
#include <string>
#include <vector>
namespace nova {
std::vector<std::string> split(const std::string &str, char delimiter);
int parseInt(std::string str, int def);
float intBitsToFloat(int bits);
int floatToRawIntBits(float value);
std::string readFile(const std::filesystem::path &);
}  // namespace nova
