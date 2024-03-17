#include "ASync.hpp"

#include <any>
#include <chrono>
#include <exception>
#include <memory>
#include <mutex>

#include "core/Log.hpp"
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
  if(!taskList.empty()){
  auto top =std::move(taskList.front());
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
    taskList.push(std::move(top));
    lock.unlock();
  }
  }
}
bool Context::isEmpty(){
	return taskList.empty();
}
void Context::post_task(std::shared_ptr<Basic_Task> task) {
  task->init(this);
  task->state = TaskState::waiting;
  {
  std::unique_lock<std::mutex> lock(task_list_mutex);
  taskList.push(task);
  }
}
std::any Context::post_any(std::shared_ptr<Basic_Task> task) {
  post_task(task);
  return task->return_post_any();
}
Runnable_Task::return_post_type Runnable_Task::return_post() { return this; }
std::any Runnable_Task::return_post_any() { return return_post(); }
void Runnable_Task::run() { runnable(context, this); }
bool Runnable_Task::stop() {
  if (stopped || state != TaskState::waiting) return false;
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

int Timer::getRepactCount() { return repeatCount; }
int Timer::getRunTimes() { return times; }
std::chrono::milliseconds Timer::getInterval() { return interval; }
std::chrono::milliseconds Timer::getDelay() { return delay; }
bool Timer::cancel() {
  if (stopped) return false;
  stopped = true;
  return true;
}
void Timer::init(Context *c) { this->context = c; }
void Timer::run() { func(context, this, times); }
bool Timer::if_run() {
  return (!stopped) && (times <= repeatCount) &&
         (times == 1
              ? ((std::chrono::steady_clock::now() - lastRun) >= delay)
              : ((std::chrono::steady_clock::now() - lastRun) >= interval));
}
bool Timer::if_delete() { return (stopped) || (times > repeatCount); }
void Timer::on_destroy() {}
void Timer::finish() { times++;lastRun=std::chrono::steady_clock::now(); }
std::any Timer::return_post_any() { return return_post(); }
void Timer::throwError(std::exception *e,
                       std::source_location source_location) {
  context->onError(e, this, source_location);
}
const std::type_info &Timer::taskType() {
  return typeid(Timer);
}
Timer::return_post_type Timer::return_post() { return this; }
Timer::Timer(runnable_function func, std::chrono::milliseconds delay,
             std::chrono::milliseconds interval, int repeatCount)
    : func(func),
      delay(delay),
      interval(interval),
      repeatCount(repeatCount),
      times(1) {
  lastRun = std::chrono::steady_clock::now();
}
}  // namespace async
}  // namespace nova
