#include "fmt/base.h"
#include <core/Log.hpp>
int main() {
  nova::Log::my_logger.logDir = std::filesystem::current_path() / "logs";
  Log_info("example : async");
  fmt::print("{holder test}", fmt::arg("holder", nova::placeholder));
}
