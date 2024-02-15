#include <stdexcept>
#ifdef __cplusplus
extern "C" {
#endif
#include <assert.h>
#ifdef __cplusplus
} /* extern "C" */
#endif
#include <mutex>

#include "Threads.hpp"
namespace nova {
Task::Task() {}

Task::Task(Runnable runnable) : func(runnable), state(TaskState::waiting) {}
Task::Task(Runnable runnable, uint8_t priority)
    : func(runnable), state(TaskState::waiting), taskPriority(priority) {}
ThreadPool::ThreadPool(size_t size) : mutex(), running(false), maxSize(size) {}
ThreadPool::~ThreadPool() {
  if (running) stop();
}
void ThreadPool::start() {
  assert(threads.empty());
  running = true;
}
void ThreadPool::stop() {
  {
    std::unique_lock<std::mutex> lock(mutex);
    running = false;
  }
  for (auto it : threads) {
    it->join();
    delete it;
  }
  threads.clear();
}
void ThreadPool::threadLoop(size_t my_id) {
  while (running) {
    auto task = take();
    if (task.state != TaskState::undefined) {
      try {
        task.func();
      } catch (std::runtime_error err) {
        if (handler) {
          handler(my_id, err);
        }
        task.state = TaskState::error;
        continue;
      }
      task.state = TaskState::finish;
    }
  }
}
void ThreadPool::setErrorHandler(threadPoolErrorHandler hand) {
  this->handler = hand;
}
bool ThreadPool::isBlocking() {
  std::unique_lock<std::mutex> lock(mutex);
  return (!tasks.empty()) && tasks.top().state == TaskState::waiting;
}
void ThreadPool::addTask(Task& task) {
  std::unique_lock<std::mutex> lock(mutex);
  task.state = TaskState::waiting;
  tasks.push(task);
  if (isBlocking() && threads.size() < maxSize) newThread();
}
void ThreadPool::addTask(Runnable runnable) {
  std::unique_lock<std::mutex> lock(mutex);
  tasks.push(Task(runnable));
  if (isBlocking() && threads.size() < maxSize) newThread();
}
Task ThreadPool::take() {
  std::unique_lock<std::mutex> lock(mutex);
  while (tasks.empty() && running)
    ;
  Task task;
  Tasks::size_type size = tasks.size();
  if (!tasks.empty() && running) {
    task = tasks.top();
    tasks.pop();
    assert(size - 1 == tasks.size());
  }
  return task;
}
void ThreadPool::newThread() {
  if (threads.size() == maxSize) return;
  auto p = new std::thread(
      std::bind(&ThreadPool::threadLoop, this, std::placeholders::_1),
      threads.size());
  threads.push_back(p);
  (*threads.end())->detach();
}
ThreadPool::ThreadPool(size_t size, threadPoolErrorHandler hand)
    : handler(hand), maxSize(size) {}
ThreadPool::ThreadPool(const ThreadPool&) {}
ThreadPool& ThreadPool::operator=(const ThreadPool&) { return *this; }

ThreadPool Threads::threadPool =
    ThreadPool(10, [](size_t id, std::runtime_error error) -> void {

    });
void Threads::daemon(Runnable runnable){
    threadPool.addTask(runnable);
}
}  // namespace nova
