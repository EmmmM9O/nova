#pragma once
#include "concepts"
#include "source_location"
#include <any>
#include <chrono>
#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <vector>
namespace nova {
namespace async {
enum class TaskState { finish, error, running, unPost, waiting };
class Context;
class Basic_Task {
protected:
  Context *context;

public:
  TaskState state = TaskState::unPost;
  virtual const std::type_info &taskType() = 0;
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
class WhileUtilTask : public Basic_Task {
private:
  bool stopped = false;

public:
  using runnable_function = std::function<void(Context *, WhileUtilTask *)>;
  using bool_function = std::function<bool(Context *, WhileUtilTask *)>;
  runnable_function runnable;
  bool_function util;
  using return_post_type = WhileUtilTask *;
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
  WhileUtilTask(runnable_function, bool_function);
};
template <typename Return, typename Error> class Promise;
template <typename Return, typename Error> class Promise_Return {
public:
  using resolve_function = std::function<void(Return, Context *)>;
  using reject_function =
      std::function<void(Error, std::source_location, Context *)>;
  using finish_function = std::function<void(Context *)>;
  resolve_function resolve_function_;
  reject_function reject_function_;
  finish_function finish_function_;
  Promise_Return();
  Promise_Return<Return, Error> Then(resolve_function);
  Promise_Return<Return, Error> Catch(resolve_function);
  Promise_Return<Return, Error> Finally(resolve_function);
};

template <typename Return, typename Error> class Promise_Helper {
public:
  Promise_Return<Return, Error> return_promise;
  Context *context;
  Promise<Return, Error> *promise;
  Promise_Helper<Return, Error>(Context *, Promise<Return, Error> *);
  void resolve(Return res);
  void reject(Error err);
};
template <typename Return, typename Error> class Promise : public Basic_Task {
public:
  using promise_function =
      std::function<void(Context *, Promise *, Promise_Helper<Return, Error>)>;
  using return_post_type = Promise_Helper<Return, Error>;
  const std::type_info &taskType() override final;
  std::shared_ptr<return_post_type> return_promise;
  void init(Context *) override final;
  void run() override final;
  bool if_run() override final;
  bool if_delete() override final;
  void on_destroy() override final;
  void finish() override final;
  void throwError(Error error,
                  std::source_location source_location =
                      std::source_location::current()) override final;
  std::any return_post_any() override final;
  return_post_type return_post();
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
// Promise

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

} // namespace async
// Events
template <typename T>
concept EnumType = std::is_enum<T>();

class Events {
private:
  static std::map<std::type_index, std::vector<std::any>> events;
  static std::map<std::string, std::vector<std::function<void()>>> enums;

public:
  template <EnumType E, E V> static std::string getEnumHash() {
    return __PRETTY_FUNCTION__;
  }
  template <typename T> void static on(std::function<void(T)> func) {
    events[typeid(T)].push_back(func);
  }
  template <EnumType T> void static run(std::function<void()> func) {
    enums[getEnumHash<T>()].push_back(func);
  }
  template <typename T> void static fire_(T event) {
    for (auto f : events[typeid(T)]) {
      std::any_cast<std::function<void(T)>>(f)(event);
    }
  }
  template <typename T, typename... Args> void static fire(Args... args) {
    static_assert(std::is_constructible<T, Args...>::value,
                  "T must be able to constuct from Args...");
    fire_<T>(T(std::forward<Args>(args)...));
  }
  template <EnumType T> static void fireEnum() {
    for (auto f : enums[getEnumHash<T>()]) {
      std::any_cast<std::function<void()>>(f)();
    }
  }
  static void clear();
};
struct sigInt {
  int signum;
  sigInt(int signum);
};
} // namespace nova
