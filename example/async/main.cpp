#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "core/ASync.hpp"
#include "core/Log.hpp"
int main() {
  nova::Log::my_logger.logDir = std::filesystem::current_path() / "logs";
  Log_info("example : async");
  nova::async::Context context;
  auto timer = context.post(nova::async::Timer(
      [](auto, auto, auto times) -> void {
        std::cout << "1 s timer :" << times << std::endl;
      },
      std::chrono::milliseconds(0), std::chrono::milliseconds(1000), 10));
  context.post(nova::async::Timer(
      [timer](auto, auto, auto times) -> void {
        std::cout << "2 s timer :" << times << std::endl;
        if (times == 3) {
          timer->cancel();
        }
      },
      0.0_asecond, 2.0_asecond, 5));
  auto promise =
      context
          .post(nova::async::Promise<int, int>([](auto* context, auto,
                                                  auto helper) -> void {
            context->post(nova::async::Timer(
                [helper](auto, auto, auto) -> void { helper->resolve(114514); },
                2.0_asecond, 0.0_asecond, 1));
          }))
          ->Then([](auto res, auto) -> void {
            std::cout << "promise test" << res << std::endl;
          });
  while (!context.isEmpty()) {
    context.run_once();
  }
}
