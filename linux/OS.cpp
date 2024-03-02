#include "core/OS.hpp"

#include <string>

#include "core/Threads.hpp"
namespace nova {
// TODO linux
std::string OS::getClipboardText() { return ""; }
void OS::setClipboardText(std::string text) { return; }
bool OS::exec(std::string str) { return system(str.c_str()) == 0; }
bool OS::openFolder(std::string path) {
  Threads::daemon(
      [path]() -> void { OS::exec(std::string("xdg-open ") + path); });
  return true;
}
bool OS::openURI(std::string url) {
  Threads::daemon(
      [url]() -> void { OS::exec(std::string("xdg-open ") + url); });

  return true;
}
}  // namespace nova
