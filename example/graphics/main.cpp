
#include <csignal>
#include <iostream>

#include "core/Core.hpp"
#include "core/Log.hpp"
#include "core/application.hpp"
#include "desktop/application.hpp"
class TestLauncher : public nova::ApplicationCore {
 public:
  void setup() override {}
  TestLauncher() {}
  void exit() override {}
};
std::shared_ptr<nova::ApplicationListener> p(new TestLauncher());
void handleSignal(int signum) {
  Log_error("receive signum:{}", signum);
  p.reset();
  nova::Core::app->exit();
  Log_error("waiting app exit {}", "...");
  if (nova::Core::app->running()) {
  }
}
int main() {
  signal(SIGINT, handleSignal);
  nova::DesktopConfig config{.width = 900, .height = 700, .title = "Nova"};
  nova::DesktopApplication dapp(p, config);
  nova::Core::app = &dapp;
  Log_info("window {}", "end");
  return 0;
}
