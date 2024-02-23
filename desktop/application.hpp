#pragma once
#include <include/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "core/application.hpp"
namespace nova {
struct DesktopConfig {
  int width = 640;
  int height = 480;
  int r = 8, g = 8, b = 8, a = 8;
  std::string title = "Nova Application";
};
class DesktopApplication : public Application {
 private:
  listenersType listeners;
  DesktopConfig config;
  int init();
  bool running = true;
  GLFWwindow* window;
  void listen(std::function<void(std::shared_ptr<ApplicationListener>)> func);
  void loop();

 public:
  void addListener(std::shared_ptr<ApplicationListener> listener) override;
  void removeListener(std::shared_ptr<ApplicationListener> listener) override;
  void cleanup();
  DesktopApplication(std::shared_ptr<ApplicationListener> listener,
                     DesktopConfig config);
  void exit() override;
  void post(Runnable runnable) override;
  void setClipboardText(std::string text) override;
  std::string getClipboardText() override;
  runType getType() override;
  systemType getSystem() override;
  listenersType& getListeners() override;
  bool openURI(std::string url) override;
  bool openFolder(std::string path) override;
  GLFWwindow* getWindow();
};
}  // namespace nova
