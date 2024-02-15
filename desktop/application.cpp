#include <glad/gl.h>

#include <mutex>
#include <string>

#include "core/OS.hpp"
#include "core/function.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "application.hpp"
#include "core/Core.hpp"
#include "core/Threads.hpp"
#include "core/application.hpp"

namespace nova {
DesktopApplication::DesktopApplication(
    std::shared_ptr<ApplicationListener> lintener, DesktopConfig config) {
  this->config = config;
  this->listeners.push_back(lintener);
  if (init() == 0) {
    loop();
    glfwTerminate();
  } else {
    std::cout << "create window Error" << std::endl;
    return;
  }
  listen([](std::shared_ptr<ApplicationListener> l) -> void { l->exit(); });
  cleanup();
}
void DesktopApplication::exit() { running = false; }
void DesktopApplication::post(Runnable runnable) {
  Threads::threadPool.addTask(runnable);
}
listenersType& DesktopApplication::getListeners() { return listeners; }
runType DesktopApplication::getType() { return runType::Desktop; }
systemType DesktopApplication::getSystem() { return OS::getSystem(); }
GLFWwindow* DesktopApplication::getWindow() { return window; }
std::string DesktopApplication::getClipboardText() {
  return OS::getClipboardText();
}
void DesktopApplication::setClipboardText(std::string text) {
  OS::setClipboardText(text);
}
int DesktopApplication::init() {
  listen([](std::shared_ptr<ApplicationListener> l) -> void { l->init(); });
  if (!glfwInit()) return -1;
  window = glfwCreateWindow(config.width, config.height, config.title.c_str(),
                            NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);

  return 0;
}
void DesktopApplication::loop() {
  listen([](std::shared_ptr<ApplicationListener> l) -> void { l->init(); });

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(49.f / 255, 77.f / 255, 121.f / 255, 1.f);
    glfwSwapBuffers(window);
    glfwPollEvents();
    defaultUpdate();
    listen([](std::shared_ptr<ApplicationListener> l) -> void { l->update(); });
  }
}
bool DesktopApplication::openFolder(std::string path) {
  return OS::openFolder(path);
}
bool DesktopApplication::openURI(std::string url) { return OS::openURI(url); }
void DesktopApplication::listen(
    std::function<void(std::shared_ptr<ApplicationListener>)> func) {
  std::lock_guard<std::mutex> lock(mt);
  for (auto l : listeners) {
    func(l);
  }
}
void DesktopApplication::cleanup() {
  std::cout << "test4" << std::endl;
  listen([](std::shared_ptr<ApplicationListener> l) -> void {
    l->pause();
    l->dispose();
  });
  std::cout << "test5" << std::endl;
  dispose();
}
void DesktopApplication::addListener(
    std::shared_ptr<ApplicationListener> listener) {
  std::lock_guard<std::mutex> guard(mt);
  listeners.push_back(listener);
}
void DesktopApplication::removeListener(
    std::shared_ptr<ApplicationListener> listener) {
  std::lock_guard<std::mutex> guard(mt);
  listeners.erase(std::remove(listeners.begin(), listeners.end(), listener),
                  listeners.end());
}
}  // namespace nova
