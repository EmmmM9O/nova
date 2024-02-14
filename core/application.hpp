#pragma once
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "function.hpp"
namespace nova {
enum class systemType { Android, Linux, Window ,Undefined};
enum class runType { Desktop, Headless };
class Disposable {
 public:
  virtual void dispose() = 0;
  virtual bool isDisposed() = 0;
};
class ApplicationListener {
 public:
  virtual void init() = 0;
  virtual void resize(int width, int height) = 0;
  virtual void update() = 0;
  virtual void pause() = 0;
  virtual void resume() = 0;
  virtual void dispose() = 0;
  virtual void exit() = 0;
  virtual void fileDropped(std::filesystem::path path);
};
class Application : public Disposable {
 protected:
  static std::mutex mt;

 public:
  using listenersType = std::vector<std::shared_ptr<ApplicationListener>>;
  virtual listenersType& getListeners() = 0;
  virtual void addListener(std::shared_ptr<ApplicationListener> listener);
  virtual void removeListener(std::shared_ptr<ApplicationListener> listener);
  virtual void defaultUpdate();
  virtual systemType getSystem() = 0;
  virtual runType getType() = 0;
  virtual bool isDesktop();
  virtual bool isHeadless();
  virtual bool isWindow();
  virtual bool isLinux();
  virtual bool isAndroid();
  virtual int getVersion() = 0;
  virtual std::string getClipboardText() = 0;
  virtual void post(Runnable runnable) = 0;
  virtual void setClipboardText(std::string text) = 0;
  virtual bool openFolder(std::string path);
  virtual bool openURI(std::string url);
  virtual void dispose() override;
  virtual void exit() = 0;
};
}  // namespace nova
