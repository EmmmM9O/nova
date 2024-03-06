#include "Log.hpp"

#include <filesystem>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include <fstream>
#include <ios>
#include <string>
namespace nova {
std::string logger::formatOutput(const std::source_location &location,
                                 const LogLevel &level, std::string text) {
  return fmt::format(fmt::runtime(formatStyle), fmt::arg("level", level),
                     fmt::arg("line", location.line()),
                     fmt::arg("column", location.column()),
                     fmt::arg("file", location.file_name()),
                     fmt::arg("function", location.function_name()),
                     fmt::arg("context", text));
}
std::string logger::timeFormat(std::string str) {
  return fmt::format(fmt::runtime(formatTime), fmt::arg("context", str),
                     fmt::arg("time", fmt::localtime(time)));
}
std::string logger::getFormatFle(LogLevel level) {
  return fmt::format(fmt::runtime(fileFormat), fmt::arg("level", level),
                     fmt::arg("time", fmt::localtime(time)));
}
void logger::log(const std::source_location &location, const LogLevel &level,
                 std::string text) {
  auto str = formatOutput(location, level, text);
#ifdef __ANDROID__

  printCosnole(str, level);
  writeFile(timeFormat(str), level);
#else
  auto res = timeFormat(str);
  printCosnole(res, level);
  writeFile(rea, level);
#endif
}
void logger::writeFile(std::string str, LogLevel level) {
  std::filesystem::path logfile = logDir / getFormatFle(level);
  std::ofstream stream(logfile, std::ios::app);
  if (stream.is_open()) {
    stream << str << std::endl;
    stream.close();
  } else {
    printCosnole("can not open file " + logDir.string(), LogLevel::Error);
  }
}
logger Log::my_logger;
} // namespace nova
auto fmt::formatter<nova::LogLevel>::format(nova::LogLevel level,
                                            format_context &ctx) const {
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
