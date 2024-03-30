#include "AndroidApplication.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <thread>

#include "AndroidCore.hpp"
#include "android/AndroidGraphics.hpp"
#include "android/native_window.h"
#include "android/native_window_jni.h"
#include "core/Core.hpp"
#include "core/Graphics.hpp"
#include "core/Log.hpp"
#include "core/Threads.hpp"
#include "core/application.hpp"
#include "jni.h"
#include "log/JniLog.h"
namespace nova {

bool AndroidApplication::running() { return running_; }
AndroidApplication::AndroidApplication() : graphics(), context() {
  nova::android::app = this;
  nova::Core::app = this;
  nova::Core::graphics = &graphics;
  nova::Core::context = &context;
}
runType AndroidApplication::getType() { return runType::Desktop; }
systemType AndroidApplication::getSystem() { return systemType::Android; }
void AndroidApplication::initialize(
    JNIEnv *env, jobject activity,
    std::shared_ptr<ApplicationListener> listener,
    AndroidApplicationConfiguration config) {
  env->GetJavaVM(&javaVM);
  coreActivity = env->NewGlobalRef(activity);
  auto str = getFilesDirJava();
  logJava("jni" + str);
  filesDir = std::filesystem::path(str);
  Log::my_logger.logDir = filesDir / "logs";
  Log_info("setup log:{}", Log::my_logger.logDir.string());
  nova::Log::my_logger.log(std::source_location::current(),
                           nova::LogLevel::Info, "sdk version {}",
                           getVersion());
  addListener(listener);
}
void AndroidApplication::createSurcafe(JNIEnv *env, jobject view,
                                       jobject surface) {
  coreView = env->NewGlobalRef(view);
  window = ANativeWindow_fromSurface(env, surface);
  //  graphics.setupTask();
  ContextThread = std::thread([this]() -> void {
    Log_info("setup async threads");
    graphics.init(window);
    Log_info("{}", graphics.getGLVersion());
    graphics.setupTask();
    while (running()) {
      nova::Core::context->run_once();
    }
  });
}
void AndroidApplication::surfaceChange(JNIEnv *env, jobject instance,
                                       jint width, jint height) {
  graphics.change_surface(width, height);
}
void AndroidApplication::surfaceDestory(JNIEnv *env, jobject instance) {
  running_ = false;
  ContextThread.join();
  graphics.destory();
  graphics.dispose();
}
int32_t AndroidApplication::getVersion() {
  JNIEnv *env;
  javaVM->AttachCurrentThreadAsDaemon(&env, nullptr);
  jclass javaClass = env->GetObjectClass(coreActivity);
  if (javaClass == nullptr) {
    LOGE("fail to find activity class");
    return 0;
  }
  jmethodID javaMethod = env->GetMethodID(javaClass, "getVersion", "()I");
  if (javaMethod == nullptr) {
    LOGE("fail to find method getVersion");
    return 0;
  }
  int res = env->CallIntMethod(coreActivity, javaMethod);
  return res;
}
long AndroidApplication::getNativeHeap() {
  JNIEnv *env;
  javaVM->AttachCurrentThreadAsDaemon(&env, nullptr);
  jclass javaClass = env->GetObjectClass(coreActivity);
  if (javaClass == nullptr) {
    LOGE("fail to find activity class");
    return 0;
  }
  jmethodID javaMethod = env->GetMethodID(javaClass, "getNativeHeap", "()L");
  if (javaMethod == nullptr) {
    LOGE("fail to find method getNativeHeap");
    return 0;
  }
  long res = env->CallLongMethod(coreActivity, javaMethod);
  return res;
}
void AndroidApplication::setClipboardText(std::string text) {
  JNIEnv *env;
  javaVM->AttachCurrentThreadAsDaemon(&env, nullptr);
  jclass javaClass = env->GetObjectClass(coreActivity);
  if (javaClass == nullptr) {
    LOGE("fail to find activity class");
    return;
  }
  jmethodID javaMethod =
      env->GetMethodID(javaClass, "setClipboardText", "(Ljava/lang/String;)V");
  if (javaMethod == nullptr) {
    LOGE("fail to find method setClipboardText");
    return;
  }
  env->CallVoidMethod(coreActivity, javaMethod,
                      env->NewStringUTF(text.c_str()));
}
void AndroidApplication::logJava(std::string text) {
  JNIEnv *env;
  javaVM->AttachCurrentThreadAsDaemon(&env, nullptr);
  jclass javaClass = env->GetObjectClass(coreActivity);
  if (javaClass == nullptr) {
    LOGE("fail to find activity class");
    return;
  }
  jmethodID javaMethod =
      env->GetMethodID(javaClass, "saveInfoToFile", "(Ljava/lang/String;)V");
  if (javaMethod == nullptr) {
    LOGE("fail to find method saveInfoToFile");
    return;
  }
  env->CallVoidMethod(coreActivity, javaMethod,
                      env->NewStringUTF(text.c_str()));
}
std::string AndroidApplication::getClipboardText() {
  JNIEnv *env;
  javaVM->AttachCurrentThreadAsDaemon(&env, nullptr);
  jclass javaClass = env->GetObjectClass(coreActivity);
  if (javaClass == nullptr) {
    LOGE("fail to find activity class");
    return "error";
  }
  jmethodID javaMethod =
      env->GetMethodID(javaClass, "getClipboardText", "()Ljava/lang/String;");
  if (javaMethod == nullptr) {
    LOGE("fail to find method getClipboardText");
    return "error";
  }
  jstring jstr = (jstring)env->CallObjectMethod(coreActivity, javaMethod);
  const char *tmp = env->GetStringUTFChars(jstr, nullptr);
  std::string str = "";
  str += tmp;
  env->ReleaseStringUTFChars(jstr, tmp);
  return str;
}
bool AndroidApplication::openURI(std::string uri) {
  JNIEnv *env;
  javaVM->AttachCurrentThreadAsDaemon(&env, nullptr);
  jclass javaClass = env->GetObjectClass(coreActivity);
  if (javaClass == nullptr) {
    LOGE("fail to find activity class");
    return false;
  }
  jmethodID javaMethod =
      env->GetMethodID(javaClass, "openURI", "(Ljava/lang/String;)Z");
  if (javaMethod == nullptr) {
    LOGE("fail to find method openURI");
    return false;
  }
  bool res = env->CallBooleanMethod(coreActivity, javaMethod,
                                    env->NewStringUTF(uri.c_str()));
  return res;
}
bool AndroidApplication::openFolder(std::string path) {
  JNIEnv *env;
  javaVM->AttachCurrentThreadAsDaemon(&env, nullptr);
  jclass javaClass = env->GetObjectClass(coreActivity);
  if (javaClass == nullptr) {
    LOGE("fail to find activity class");
    return false;
  }
  jmethodID javaMethod =
      env->GetMethodID(javaClass, "openFolder", "(Ljava/lang/String;)Z");
  if (javaMethod == nullptr) {
    LOGE("fail to find method openFolder");
    return false;
  }
  bool res = env->CallBooleanMethod(coreActivity, javaMethod,
                                    env->NewStringUTF(path.c_str()));
  return res;
}
void AndroidApplication::onDestroy(JNIEnv *env, jobject activity) {
  javaVM->DestroyJavaVM();
}
void AndroidApplication::addListener(
    std::shared_ptr<ApplicationListener> listener) {
  std::lock_guard<std::mutex> guard(mt);
  listeners.push_back(listener);
}
void AndroidApplication::removeListener(
    std::shared_ptr<ApplicationListener> listener) {
  std::lock_guard<std::mutex> guard(mt);
  listeners.erase(std::remove(listeners.begin(), listeners.end(), listener),
                  listeners.end());
}
void AndroidApplication::exit() {
  JNIEnv *env;
  javaVM->AttachCurrentThreadAsDaemon(&env, nullptr);
  jclass javaClass = env->GetObjectClass(coreActivity);
  if (javaClass == nullptr) {
    LOGE("fail to find activity class");
    return;
  }
  jmethodID javaMethod = env->GetMethodID(javaClass, "exit", "()V");
  if (javaMethod == nullptr) {
    LOGE("fail to find method exit");
    return;
  }
  env->CallVoidMethod(coreActivity, javaMethod);
  running_ = false;
}

void AndroidApplication::post(Runnable runnable) {
  Threads::threadPool.addTask(runnable);
}
listenersType &AndroidApplication::getListeners() { return listeners; }
std::filesystem::path AndroidApplication::getFilesDir() { return filesDir; }
std::string AndroidApplication::getFilesDirJava() {
  JNIEnv *env;
  javaVM->AttachCurrentThreadAsDaemon(&env, nullptr);
  jclass javaClass = env->GetObjectClass(coreActivity);
  if (javaClass == nullptr) {
    LOGE("fail to find activity class");
    return "";
  }
  jmethodID javaMethod =
      env->GetMethodID(javaClass, "getFilesDirString", "()Ljava/lang/String;");
  if (javaMethod == nullptr) {
    LOGE("fail to find method getFilesDirString");
    return "";
  }
  jstring jstr = (jstring)env->CallObjectMethod(coreActivity, javaMethod);
  const char *tmp = env->GetStringUTFChars(jstr, nullptr);
  std::string str = tmp;
  env->ReleaseStringUTFChars(jstr, tmp);
  return str;
}
int AndroidApplication::getJavaVersion() {
  JNIEnv *env;
  javaVM->AttachCurrentThreadAsDaemon(&env, nullptr);
  return env->GetVersion();
}
long AndroidApplication::getMaxMemory() {
  JNIEnv *env;
  javaVM->AttachCurrentThreadAsDaemon(&env, nullptr);
  jclass javaClass = env->GetObjectClass(coreActivity);
  if (javaClass == nullptr) {
    LOGE("fail to find activity class");
    return 0;
  }
  jmethodID javaMethod = env->GetMethodID(javaClass, "getMaxMemory", "()L");
  if (javaMethod == nullptr) {
    LOGE("fail to find method getMaxMemory");
    return 0;
  }
  long res = env->CallLongMethod(coreActivity, javaMethod);
  return res;
}
}  // namespace nova
