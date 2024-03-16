#include "ASync.hpp"

#include <any>
#include <exception>
#include <memory>
#include <mutex>

#include "core/Log.hpp"
#include "core/Threads.hpp"
#include "source_location"
namespace nova {
namespace async {
void Context::onError(std::exception *exception, Basic_Task *task,
                      std::source_location source_location) {
  nova::Log::my_logger.log(source_location, LogLevel::Error,
                           "Context Task {} error:{}", task->taskType().name(),
                           exception->what());
}
void Context::run_once() {
  std::unique_lock<std::mutex> lock(task_list_mutex);
  lock.lock();
  auto top = taskList.front();
  top->state = TaskState::running;
  taskList.pop();
  lock.unlock();
  if (top->if_run()) {
    top->run();
    top->finish();
  }
  if (top->if_delete()) {
    top->on_destroy();
    top->state = TaskState::finish;
  } else {
    lock.lock();
    taskList.push(top);
    lock.unlock();
  }
}
void Context::post_task(std::shared_ptr<Basic_Task> task) {
  std::unique_lock<std::mutex> lock(task_list_mutex);
  task->init(this);
  task->state = TaskState::waiting;
  lock.lock();
  taskList.push(task);
  lock.unlock();
}
std::any Context::post_any(std::shared_ptr<Basic_Task> task) {
  post_task(task);
  return task->return_post_any();
}
Runnable_Task::return_post_type Runnable_Task::return_post() { return this; }
std::any Runnable_Task::return_post_any() { return return_post(); }
void Runnable_Task::run() { runnable(context, this); }
bool Runnable_Task::stop() {
  if (stopped || state != TaskState::waiting)
    return false;
  stopped = true;
  return true;
}

const std::type_info &Runnable_Task::taskType() {
  return typeid(Runnable_Task);
}
bool Runnable_Task::if_delete() { return stopped; }
bool Runnable_Task::if_run() { return !stopped; }
void Runnable_Task::finish() {}
void Runnable_Task::on_destroy() {}
void Runnable_Task::init(Context *con) { this->context = con; }
Runnable_Task::Runnable_Task(Runnable_Task::runnable_function func) {
  this->runnable = func;
}
void Runnable_Task::throwError(std::exception *e,
                               std::source_location source_location) {
  context->onError(e, this, source_location);
}
} // namespace async
} // namespace nova
