#include "application.hpp"

#include <algorithm>
#include <iterator>
#include <memory>
#include <mutex>
namespace nova {
void Application::addListener(std::shared_ptr<ApplicationListener> listener) {
  std::lock_guard<std::mutex> guard(mt);
  getListeners().push_back(listener);
}
void Application::removeListener(
    std::shared_ptr<ApplicationListener> listener) {
  std::lock_guard<std::mutex> guard(mt);
  auto v = getListeners();
  v.erase(std::remove(v.begin(), v.end(), listener), v.end());
}
void Application::defaultUpdate() {}
int Application::getVersion() { return 0; }
bool Application::isDesktop() { return getType() == runType::Desktop; }
bool Application::isHeadless() { return getType() == runType::Headless; }
bool Application::isWindow() { return getSystem() == systemType::Window; }
bool Application::isLinux() { return getSystem() == systemType::Linux; }
bool Application::isAndroid() { return getSystem() == systemType::Android; }
bool Application::openFolder(std::string path) { return false; }
bool Application::openURI(std::string url) { return false; }
void Application::dispose() {}
std::mutex Application::mt;
void ApplicationCore::add(std::shared_ptr<ApplicationListener> listener) {
  modules.push_back(listener);
}
void ApplicationCore::init() {
  setup();
  for (auto l : modules) l->init();
}
void ApplicationCore::resize(int w, int h) {
  for (auto l : modules) l->resize(w, h);
}
void ApplicationCore::dispose() {
  for (auto l : modules) l->dispose();
}
void ApplicationCore::pause() {
  for (auto l : modules) l->pause();
}
void ApplicationCore::resume() {
  for (auto l : modules) l->resume();
}
void ApplicationCore::update() {
  for (auto l : modules) l->update();
}

}  // namespace nova
