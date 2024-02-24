#pragma once
#include <fmt/core.h>
#include <filesystem>
#include <functional>
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

class logger {
 public:
  std::time_t time = std::time(nullptr);
  const bool useColors = true;
  std::filesystem::path LogDir;
  std::string_view formatStyle =
      "[{level}][{time:%H:%M:%S %Y-%m-%d}][File:[{file}] Func:[{function}] "
      "Line:[{line}]]\n>{context}";
  void init(std::filesystem::path logDir);
  template <typename... Args>
  std::string format(fmt::format_string<Args...> str, Args&&... args) {
    return fmt::vformat(str, fmt::make_format_args(args...));
  }
  std::string formatOutput(const std::source_location& location,
                           const LogLevel& level, std::string text);
  void log(const std::source_location& location, const LogLevel& level,
           std::string text);

  template <typename... Args>
  void log(const std::source_location& location, const LogLevel& level,
           fmt::format_string<Args...> str, Args&&... args) {
    log(location, level, format(str, args...));
  }
};
class Log {
 public:
  static logger my_logger;
  static void init(std::filesystem::path path);
  template <typename... Args>
  static void log(const std::source_location& location, const LogLevel& level,
                  fmt::format_string<Args...> str, Args&&... args) {
    my_logger.log(location, level, format(str, args...));
  }
  template <typename... Args>
  static void info(const std::source_location& location,
                   fmt::format_string<Args...> str, Args&&... args) {
    log(location, LogLevel::Info, str, args...);
  }
  template <typename... Args>
  static void error(const std::source_location& location,
                    fmt::format_string<Args...> str, Args&&... args) {
    log(location, LogLevel::Error, str, args...);
  }
  template <typename... Args>
  static void warn(const std::source_location& location,
                   fmt::format_string<Args...> str, Args&&... args) {
    log(location, LogLevel::Warn, str, args...);
  }
  template <typename... Args>
  static void debug(const std::source_location& location,
                    fmt::format_string<Args...> str, Args&&... args) {
    log(location, LogLevel::Debug, str, args...);
  }
};
}  // namespace nova
template <>
struct fmt::formatter<nova::LogLevel> : formatter<string_view> {
  auto format(nova::LogLevel level, format_context& ctx) const;
};
#define Log_log(level, format, ...) \
  nova::Log::log(std::source_location::current(), level, format, __VA_ARGS__)
#define Log_info(format, ...) \
  nova::Log::info(std::source_location::current(), format, __VA_ARGS__)
#define Log_debug(format, ...) \
  nova::Log::debug(std::source_location::current(), format, __VA_ARGS__)
#define Log_warn(format, ...) \
  nova::Log::warn(std::source_location::current(), format, __VA_ARGS__)
#define Log_error(format, ...) \
  nova::Log::error(std::source_location::current(), format, __VA_ARGS__)
