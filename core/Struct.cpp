#include "Struct.hpp"

#include <mutex>
#include "core/function.hpp"

namespace nova {
std::mutex TaskQueue::mt;
void TaskQueue::run() {
  {
    std::lock_guard<std::mutex> guard(mt);
    executedRunnables.clear();
    for (auto t : runnables) {
      executedRunnables.push_back(t);
    }
    runnables.clear();
  }
    for(auto runnable:runnables){
        runnable();
    }
}
int TaskQueue::size(){
    return runnables.size();
}
void TaskQueue::clear(){
    std::lock_guard<std::mutex> guard(mt);
    runnables.clear();
}
void TaskQueue::post(Runnable runnable){
    std::lock_guard<std::mutex> guard(mt);
    runnables.push_back(runnable);
}
}  // namespace nova
