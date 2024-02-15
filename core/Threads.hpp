#pragma once
#include <any>
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "core/function.hpp"
namespace nova {
enum class TaskState { undefined, waiting, running, finish ,error};
class Task {
 public:
  Runnable func;
  TaskState state = TaskState::undefined;
  Task(Runnable runnable);
  Task(Runnable runnable, uint8_t priority);
  Task();
  uint8_t taskPriority = 100;
};
struct TaskPriorityCmp {
  bool operator()(const Task t1, const Task t2) {
    return t1.taskPriority > t2.taskPriority;
  }
};
typedef std::function<void(size_t,std::runtime_error)> threadPoolErrorHandler;
typedef std::priority_queue<Task, std::vector<Task>, TaskPriorityCmp> Tasks;
typedef std::vector<std::thread*> ThreadsVec;
class ThreadPool {
 private:
  ThreadPool(const ThreadPool&);
  ThreadPool& operator=(const ThreadPool&);
  Tasks tasks;
  ThreadsVec threads;
  void threadLoop(size_t my_id);
  Task take();
  std::mutex mutex;
  bool running;
  void newThread();
    threadPoolErrorHandler handler;
 public:
  size_t maxSize = 10;
  ThreadPool(size_t size);
  ThreadPool(size_t size,threadPoolErrorHandler handler);
  ~ThreadPool();
  void start();
  void stop();
  void addTask(Task&);
  void addTask(Runnable);
  bool isBlocking();
    void setErrorHandler(threadPoolErrorHandler handler);
};
class Threads{
public:
    static ThreadPool threadPool;
    static void daemon(Runnable runnable);
};
}  // namespace nova
