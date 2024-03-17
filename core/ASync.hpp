#pragma once
#include <any>
#include <chrono>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <type_traits>
#include <typeinfo>

#include "concepts"
#include "source_location"
namespace nova {
namespace async {
enum class TaskState { finish, error, running, unPost, waiting };
class Context;
class Basic_Task {
 protected:
  Context *context;

 public:
  TaskState state = TaskState::unPost;
  virtual const std::type_info &taskType()=0;
  virtual void run() = 0;
  virtual bool if_run() = 0;
  virtual bool if_delete() = 0;
  virtual void on_destroy() = 0;
  virtual void init(Context *) = 0;
  virtual void finish() = 0;
  virtual std::any return_post_any() = 0;
  virtual void throwError(std::exception *,
                          std::source_location source_location =
                              std::source_location::current()) = 0;
};
class Runnable_Task : public Basic_Task {
 private:
  bool stopped = false;

 public:
  using runnable_function = std::function<void(Context *, Runnable_Task *)>;
  runnable_function runnable;
  using return_post_type = Runnable_Task *;
  const std::type_info &taskType() override final;
  void init(Context *) override final;
  void run() override final;
  bool stop();
  bool if_run() override final;
  bool if_delete() override final;
  void on_destroy() override final;
  void finish() override final;
  std::any return_post_any() override final;
  void throwError(std::exception *,
                  std::source_location source_location =
                      std::source_location::current()) override final;
  return_post_type return_post();
  Runnable_Task(runnable_function);
};
class Timer : public Basic_Task {
 public:
  using runnable_function = std::function<void(Context *, Timer *, int)>;
  using return_post_type = Timer *;

 private:
  runnable_function func;
  std::chrono::milliseconds delay, interval;
  int repeatCount, times;
  std::chrono::steady_clock::time_point lastRun;
  bool stopped = false;

 public:
  int getRepactCount();
  int getRunTimes();
  std::chrono::milliseconds getInterval();
  std::chrono::milliseconds getDelay();
  bool cancel();
  const std::type_info &taskType() override final;
  void init(Context *) override final;
  void run() override final;
  bool if_run() override final;
  bool if_delete() override final;
  void on_destroy() override final;
  void finish() override final;
  std::any return_post_any() override final;
  void throwError(std::exception *,
                  std::source_location source_location =
                      std::source_location::current()) override final;
  return_post_type return_post();
  Timer(runnable_function func,
        std::chrono::milliseconds delay = std::chrono::milliseconds(0),
        std::chrono::milliseconds interval = std::chrono::milliseconds(1000),
        int repeatCount = 1);
};
template <typename T>
concept Task_Type = requires(T t) {
  typename T::return_post_type;
  { t.return_post() } -> std::same_as<typename T::return_post_type>;
  std::is_base_of_v<Basic_Task, T>;
};
class Context {
 protected:
  std::queue<std::shared_ptr<Basic_Task>> taskList;
  std::mutex task_list_mutex;

 public:
  bool isEmpty();
  void onError(std::exception *exception, Basic_Task *,
               std::source_location source_location);
  void run_once();
  void post_task(std::shared_ptr<Basic_Task> task);
  std::any post_any(std::shared_ptr<Basic_Task> task);
  template <Task_Type Task, class Task_Return = typename Task::return_post_type>
  Task_Return post(Task task) {
    std::shared_ptr<Task> ptr = std::make_shared<Task>(task);
    post_task(std::static_pointer_cast<Basic_Task>(ptr));
    return ptr->return_post();
  }
};

}  // namespace async
}  // namespace nova
