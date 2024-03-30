#include "OS.hpp"

#include "core/application.hpp"
namespace nova {
systemType OS::getSystem() {
#ifdef __linux__
#ifdef __ANDOROID__
  return systemType::Andorid
#else
  return systemType::Linux;
#endif
#else
#ifdef _Win32
  return systemType::Window;
#endif
#endif
      return systemType::Undefined;
}
bool OS::isWindow() { return getSystem() == systemType::Window; }
bool OS::isLinux() { return getSystem() == systemType::Linux; }
bool OS::isAndroid() { return getSystem() == systemType::Android; }
}  // namespace nova
