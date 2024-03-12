#include "ASync.hpp"
#include "core/Log.hpp"
#include "source_location"
#include <memory>
#include <mutex>
namespace nova {
namespace async {
void Context::onError(std::exception *exception,
                      std::shared_ptr<Basic_Task> task,
                      std::source_location source_location) {
  nova::Log::my_logger.log(source_location, LogLevel::Error,
                           "Context Task {} error:{}", task->taskType().name(),
                           exception->what());
}
void Context::run_once() {
  std::unique_lock<std::mutex> lock(task_list_mutex);
  lock.lock();
  auto top = taskList.front();
  taskList.pop();
  lock.unlock();
  if (top->if_run()) {
    top->run();
    top->finish();
    if (top->if_delete()) {
      top->on_destroy();
    } else {
      lock.lock();
      taskList.push(top);
      lock.unlock();
    }
  } else {
    lock.lock();
    taskList.push(top);
    lock.unlock();
  }
}
void Context::post_task(std::shared_ptr<Basic_Task> task) {
  std::unique_lock<std::mutex> lock(task_list_mutex);
  task->init(this);
  lock.lock();
  taskList.push(task);
  lock.unlock();
}
std::any Context::post_any(std::shared_ptr<Basic_Task> task) {
  post_task(task);
  return task->return_post_any();
}
} // namespace async
} // namespace nova
