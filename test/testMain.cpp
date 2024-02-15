#define LogTest
#ifdef DesktopTest
#include "core/Core.hpp"
#include "core/application.hpp"
#include "desktop/application.hpp"

class TestLauncher : public nova::ApplicationCore {
 public:
  void setup() override {}
  TestLauncher() {}
  void exit() override {}
};
#endif
#ifdef LogTest
#include "core/Log.hpp"
#endif
int main() {
#ifdef DesktopTest
  nova::DesktopConfig config{.width = 900, .height = 700, .title = "Nova"};
  std::shared_ptr<nova::ApplicationListener> p(new TestLauncher());
  nova::Core::app = std::shared_ptr<nova::Application>(
      new nova::DesktopApplication(p, config));
  std::cout << "window end" << std::endl;
#endif
#ifdef LogTest
  Log_info("test {}", "log");
#endif
  return 0;
}
