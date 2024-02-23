#include "core/Core.hpp"
#include "core/application.hpp"
#include "desktop/application.hpp"
#include "core/Log.hpp"
class TestLauncher : public nova::ApplicationCore {
 public:
  void setup() override {}
  TestLauncher() {}
  void exit() override {}
};
int main() {
	
  nova::DesktopConfig config{.width = 900, .height = 700, .title = "Nova"};
  std::shared_ptr<nova::ApplicationListener> p(new TestLauncher());
  nova::Core::app = std::shared_ptr<nova::Application>(
      new nova::DesktopApplication(p, config));
  Log_info("window {}", "end");
  return 0;
}
