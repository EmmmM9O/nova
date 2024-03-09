#pragma once
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

#include "android/androidCore/jni/egl/EglThread.h"
#include "core/application.hpp"
#include "jni.h"
namespace nova {
struct AndroidApplicationConfiguration {};
class AndroidApplication : public Application {
protected:
  listenersType listeners;
  std::filesystem::path filesDir;

public:
  std::shared_ptr<EglThread> eglThread = NULL;
  JavaVM *javaVM;
  jobject coreActivity, coreView;
  void initialize(JNIEnv *env, jobject activity,
                  std::shared_ptr<ApplicationListener> listener,
                  AndroidApplicationConfiguration config);
  void createSurcafe(JNIEnv *env, jobject instance, jobject surface);
  void surfaceChange(JNIEnv *env, jobject instance, jint width, jint height);
  void surfaceDestory(JNIEnv *env, jobject instance);
  void onDestroy(JNIEnv *env, jobject activity);
  runType getType() override;
  systemType getSystem() override;
  int32_t getVersion() override;
  long getNativeHeap() override;
  void setClipboardText(std::string text) override;
  std::string getClipboardText() override;
  bool openURI(std::string url) override;
  bool openFolder(std::string path) override;
  AndroidApplication();
  void addListener(std::shared_ptr<ApplicationListener> listener) override;
  void removeListener(std::shared_ptr<ApplicationListener> listener) override;
  void exit() override;
  void post(Runnable runnable) override;
  listenersType &getListeners() override;
  std::filesystem::path getFilesDir();
  std::string getFilesDirJava();
  void logJava(std::string text) ;
};
} // namespace nova
