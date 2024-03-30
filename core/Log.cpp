#include "Log.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include <filesystem>
#include <fstream>
#include <ios>
#include <string>
namespace nova {
std::string to_string(LogLevel level) {
  switch (level) {
  case nova::LogLevel::Info:
    return "info";
  case nova::LogLevel::Debug:
    return "debug";
  case nova::LogLevel::Error:
    return "error";
  case nova::LogLevel::Warn:
    return "warn";
  case nova::LogLevel::None:
    return "none";
  default:
    return "NoLevel";
  }
}
std::string logger::formatOutput(const std::source_location &location,
                                 const LogLevel &level, std::string text) {
  return fmt::format(fmt::runtime(formatStyle), fmt::arg("level", level),
                     fmt::arg("line", location.line()),
                     fmt::arg("column", location.column()),
                     fmt::arg("file", location.file_name()),
                     fmt::arg("function", location.function_name()),
                     fmt::arg("context", text), fmt::arg("color", placeholder));
}
std::string logger::timeFormat(std::string str) {
  return fmt::format(fmt::runtime(formatTime), fmt::arg("context", str),
                     fmt::arg("time", fmt::localtime(time)));
}
std::string logger::getFormatFle(LogLevel level) {
  return fmt::format(fmt::runtime(fileFormat), fmt::arg("level", level),
                     fmt::arg("time", fmt::localtime(time)));
}
void logger::_log(const std::source_location &location, const LogLevel &level,
                  std::string text) {
  auto str = formatOutput(location, level, text);
#ifdef __ANDROID__
  printCosnole(str, level);
  writeFile(timeFormat(str), level);
#else
  auto res = timeFormat(str);
  printCosnole(res, level);
  writeFile(res, level);
#endif
}
void logger::writeFile(std::string str, LogLevel level) {
  std::filesystem::create_directory(logDir.string());
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
  return formatter<string_view>::format(nova::to_string(level), ctx);
}
constexpr auto
fmt::formatter<nova::format_placeHolder>::parse(format_parse_context &context)
    -> format_parse_context::iterator {
  auto iter{context.begin()};
  const auto end{context.end()};
  place = "{";
  while ((iter != end) && *iter != '}') {
    place += *iter;
    iter++;
  }
  place += "}";
  return iter;
}
auto fmt::formatter<nova::format_placeHolder>::format(nova::format_placeHolder,
                                                      format_context &ctx) const
    -> format_context::iterator {
  return fmt::format_to(ctx.out(), "{}", place);
}
