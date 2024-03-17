#pragma once
#include <string>
#include <vector>
namespace nova {
std::vector<std::string> split(const std::string &str, char delimiter);
int parseInt(std::string str, int def);
}  // namespace nova
