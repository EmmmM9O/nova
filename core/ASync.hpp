#pragma once
#include <any>
#include <exception>
#include <memory>
#include <mutex>
#include <queue>
#include <type_traits>
#include <typeinfo>

#include "concepts"
#include "source_location"
namespace nova {
namespace async {
enum class TaskState { finish, error, running, unPost };
class Context;
class Basic_Task {
 protected:
  Context *context;

 public:
  TaskState state = TaskState::unPost;
  virtual const std::type_info &taskType();
  virtual void stop() = 0;
  virtual void run() = 0;
  virtual bool if_run() = 0;
  virtual bool if_delete() = 0;
  virtual void on_destroy() = 0;
  virtual void init(Context *) = 0;
  virtual void finish() = 0;
  virtual std::any return_post_any() = 0;
  virtual void throwError(
      std::source_location source_location = std::source_location::current());
};
class DefaultTask : public Basic_Task {
 public:
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
  void onError(std::exception *exception, std::shared_ptr<Basic_Task>,
               std::source_location source_location);
  void run_once();
  void post_task(std::shared_ptr<Basic_Task> task);
  std::any post_any(std::shared_ptr<Basic_Task> task);
  template <Task_Type Task>
  typename Task::return_post_type post(Task task) {
    std::shared_ptr<Task> ptr = std::make_shared<Task>(task);
    post_task(std::static_pointer_cast<Basic_Task>(ptr));
    typename Task::return_post_type res = ptr->return_post();
    return res;
  }
};

}  // namespace async
}  // namespace nova
