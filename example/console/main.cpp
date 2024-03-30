#include <core/Log.hpp>

#include "fmt/core.h"
int main() {
  nova::Log::my_logger.logDir = std::filesystem::current_path() / "logs";
  Log_info("example : async {color}");
  fmt::print("{:a}", nova::placeholder);
}
