#pragma once
#include <fmt/args.h>
#include <fmt/core.h>

#include <filesystem>
#include <source_location>
#include <string>
#include <string_view>
namespace nova {
enum class LogLevel {
  Debug,
  Info,
  Warn,
  Error,
  None,
};
std::string to_string(LogLevel level);
class format_placeHolder {
 public:
  std::string place;
  format_placeHolder();
  format_placeHolder(std::string p);
};
extern format_placeHolder placeholder;
extern format_placeHolder colorPlaceholder;
class logger {
 public:
  std::time_t time = std::time(nullptr);
  const bool useColors = true;
  std::string_view formatTime = "[{time:%H:%M:%S %Y-%m-%d}]{context}";
  std::string_view formatStyle =
      "[{level}][File:[{file}] Func:[{function}] "
      "Line:[{line}]]\n>{context}";
  std::filesystem::path logDir;
  std::string_view fileFormat = "log-{time:%Y-%m-%d}.log";

  std::string formatOutput(const std::source_location &location,
                           const LogLevel &level, std::string text);
  std::string timeFormat(std::string str);
  void _log(const std::source_location &location, const LogLevel &level,
            std::string text);
  template <typename... Args>
  void log(const std::source_location &location, const LogLevel &level,
           std::string str, Args &&...args) {
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    ((store.push_back(args)), ...);
    store.push_back(fmt::arg("color", colorPlaceholder));
    _log(location, level, fmt::vformat(str, store));
  }
  std::string getFormatFle(LogLevel level);
  void writeFile(std::string str, LogLevel level);
  // to build in different
  void printCosnole(std::string str, LogLevel level);
};
class Log {
 public:
  static logger my_logger;
  template <typename... Args>
  static void log(const std::source_location &location, const LogLevel &level,
                  fmt::format_string<Args...> str, Args &&...args) {
    my_logger.log(location, level, str, args...);
  }
  template <typename... Args>
  static void info(const std::source_location &location,
                   fmt::format_string<Args...> str, Args &&...args) {
    log(location, LogLevel::Info, str, args...);
  }
  template <typename... Args>
  static void error(const std::source_location &location,
                    fmt::format_string<Args...> str, Args &&...args) {
    log(location, LogLevel::Error, str, args...);
  }
  template <typename... Args>
  static void warn(const std::source_location &location,
                   fmt::format_string<Args...> str, Args &&...args) {
    log(location, LogLevel::Warn, str, args...);
  }
  template <typename... Args>
  static void debug(const std::source_location &location,
                    fmt::format_string<Args...> str, Args &&...args) {
    log(location, LogLevel::Debug, str, args...);
  }
};
}  // namespace nova
//
template <>
struct fmt::formatter<nova::LogLevel> : formatter<string_view> {
  auto format(nova::LogLevel level, format_context &ctx) const;
};
template <>
class fmt::formatter<nova::format_placeHolder> {
 public:
  constexpr format_parse_context::iterator parse(
      format_parse_context &context) {
    auto iter{context.begin()};
    const auto end{context.end()};
    place = "";
    while ((iter != end) && *iter != '}') {
      place += *iter;
      iter++;
    }
    return iter;
  }
  fmt::format_context::iterator format(const nova::format_placeHolder &,
                                       format_context &ctx) const;

 private:
  std::string place;
};

#define Log_log(level, format, ...)                                \
  nova::Log::my_logger.log(std::source_location::current(), level, \
                           format __VA_OPT__(, ) __VA_ARGS__)
#define Log_info(format, ...)                               \
  nova::Log::my_logger.log(std::source_location::current(), \
                           nova::LogLevel::Info,            \
                           format __VA_OPT__(, ) __VA_ARGS__)
#define Log_debug(format, ...)                              \
  nova::Log::my_logger.log(std::source_location::current(), \
                           nova::LogLevel::Debug,           \
                           format __VA_OPT__(, ) __VA_ARGS__)
#define Log_warn(format, ...)                               \
  nova::Log::my_logger.log(std::source_location::current(), \
                           nova::LogLevel::Warn,            \
                           format __VA_OPT__(, ) __VA_ARGS__)
#define Log_error(format, ...)                              \
  nova::Log::my_logger.log(std::source_location::current(), \
                           nova::LogLevel::Error,           \
                           format __VA_OPT__(, ) __VA_ARGS__)
