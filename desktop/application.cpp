#include <glad/gl.h>

#include <string>

#include "core/OS.hpp"
#include "core/function.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <memory>

#include "application.hpp"
#include "core/Core.hpp"
#include "core/application.hpp"

namespace nova {
DesktopApplication::DesktopApplication(
    std::shared_ptr<ApplicationListener> lintener, DesktopConfig config) {
  this->config = config;
  this->listeners.push_back(lintener);
  init();
  Core::app = std::shared_ptr<Application>(this);
}
void DesktopApplication::exit() { running = false; }
void DesktopApplication::post(Runnable runnable) { runnables.post(runnable); }
Application::listenersType& DesktopApplication::getListeners() {
  return listeners;
}
runType DesktopApplication::getType() { return runType::Desktop; }
systemType DesktopApplication::getSystem() { return OS::getSystem(); }
GLFWwindow* DesktopApplication::getWindow() { return window; }
std::string DesktopApplication::getClipboardText() {return OS::getClipboardText();}
void DesktopApplication::setClipboardText(std::string text) {OS::setClipboardText(text);}
void DesktopApplication::init(){

}
bool DesktopApplication::openFolder(std::string path){
    return OS::openFolder(path);
}
bool DesktopApplication::openURI(std::string url){
    return OS::openURI(url);
}
}  // namespace nova
