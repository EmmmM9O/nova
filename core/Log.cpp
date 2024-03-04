#include "Log.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
namespace nova {
std::string logger::formatOutput(const std::source_location& location,
                                 const LogLevel& level, std::string text) {
  return fmt::format(fmt::runtime(formatStyle), fmt::arg("level", level),
                     fmt::arg("time", fmt::localtime(time)),
                     fmt::arg("line", location.line()),
                     fmt::arg("column", location.column()),
                     fmt::arg("file", location.file_name()),
                     fmt::arg("function", location.function_name()),
		     fmt::arg("context",text));
}
void logger::log(const std::source_location& location, const LogLevel& level,
                 std::string text) {
  auto str = formatOutput(location, level, text);
  std::cout << str << std::endl;
}
logger Log::my_logger;
}  // namespace nova
auto fmt::formatter<nova::LogLevel>::format(nova::LogLevel level,
                                            format_context& ctx) const {
  string_view name = "none";
  switch (level) {
    case nova::LogLevel::Info:
      name = "info";
      break;
    case nova::LogLevel::Debug:
      name = "debug";
      break;
    case nova::LogLevel::Error:
      name = "error";
      break;
    case nova::LogLevel::Warn:
      name = "warn";
      break;
    case nova::LogLevel::None:
      name = "none";
      break;
  }
  return formatter<string_view>::format(name, ctx);
}
