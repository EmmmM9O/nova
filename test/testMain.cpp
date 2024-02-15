#include <iostream>
#include <memory>

#include "core/Core.hpp"
#include "core/application.hpp"
#include "desktop/application.hpp"
class TestLauncher : public nova::ApplicationCore {
 public:
  void setup() override {}
  TestLauncher() {}
  void exit() override {}
};
int main() {
  nova::DesktopConfig config;
  config.title = "nova";
  config.width = 900;
  config.height = 700;
  std::shared_ptr<nova::ApplicationListener> p(new TestLauncher());
  nova::Core::app = std::shared_ptr<nova::Application>(
      new nova::DesktopApplication(p, config));
  std::cout << "window end" << std::endl;
  return 0;
}
