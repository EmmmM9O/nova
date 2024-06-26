#pragma once
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "core/ASync.hpp"
#include "function.hpp"
namespace nova {
enum class systemType { Android, Linux, Window, Undefined };
enum class runType { Desktop, Headless };
class Disposable {
 public:
  virtual void dispose() = 0;
  virtual bool isDisposed() const;
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
  virtual void fileDropped(std::filesystem::path file) = 0;
};
typedef std::vector<std::shared_ptr<ApplicationListener>> listenersType;
class Application : public Disposable {
 protected:
  static std::mutex mt;

 public:
  async::Context asyncContext;
  virtual bool running() = 0;
  virtual listenersType &getListeners() = 0;
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
  virtual int getVersion();
  virtual long getNativeHeap();
  virtual long getJavaHeap();
  virtual std::string getClipboardText() = 0;
  virtual void post(Runnable runnable) = 0;
  virtual void setClipboardText(std::string text) = 0;
  virtual bool openFolder(std::string path);
  virtual bool openURI(std::string url);
  virtual void dispose() override;
  virtual void exit() = 0;
  virtual int getJavaVersion();
  virtual long getMaxMemory();
};
class ApplicationCore : public ApplicationListener {
 protected:
  listenersType modules;

 public:
  void add(std::shared_ptr<ApplicationListener> module);
  virtual void setup() = 0;
  void init() override;
  void resize(int width, int height) override;
  void update() override;
  void pause() override;
  void resume() override;
  void dispose() override;
  void fileDropped(std::filesystem::path file) override;
};
}  // namespace nova
