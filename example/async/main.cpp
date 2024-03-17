#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "core/ASync.hpp"
#include "core/Log.hpp"
int main() {
  nova::Log::my_logger.logDir = std::filesystem::current_path() / "logs";
  Log_info("example : {}", "async");
  nova::async::Context context;
  auto timer = context.post(nova::async::Timer(
      [](nova::async::Context*, nova::async::Timer*, int times) -> void {
        std::cout << "1 s timer :" << times << std::endl;
      },
      std::chrono::milliseconds(0), std::chrono::milliseconds(1000), 10));
  context.post(nova::async::Timer(
      [timer](nova::async::Context*, nova::async::Timer*, int times) -> void {
        std::cout << "2 s timer :" << times << std::endl;
        if (times == 3) {
          timer->cancel();
        }
      },
      std::chrono::milliseconds(0), std::chrono::milliseconds(2000), 5));
  while (!context.isEmpty()) {
    context.run_once();
  }
}
