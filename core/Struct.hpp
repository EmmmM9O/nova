#pragma once
#include <mutex>
#include <vector>

#include "core/function.hpp"
namespace nova {
class TaskQueue {
 protected:
  static std::mutex mt;
  std::vector<Runnable> runnables, executedRunnables;

 public:
  void run();
  int size();
  void clear();
  void post(Runnable runnable);
};
}  // namespace nova
