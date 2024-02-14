#pragma once
#include <string>

#include "core/application.hpp"
#include <fmt/core.h>
namespace nova {
class OS {
 public:
  static systemType getSystem();
  static std::string getClipboardText();
  static void setClipboardText(std::string text);
  static bool isWindow();
  static bool isLinux();
  static bool isAndroid();
  static bool openURI(std::string url);
  static bool openFolder(std::string path);
  static bool exec(std::string string);
  template <typename... Args>
  static bool exec(std::string_view rt_fmt_str, Args&&... args) {
    return exec(fmt::vformat(rt_fmt_str, fmt::make_format_args(args...)));
  }
};

}  // namespace nova
