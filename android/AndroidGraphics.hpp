#pragma once
#include "EGL/egl.h"
#include "core/Graphics.hpp"
#include "core/application.hpp"
#include "jni.h"
namespace nova {
class AndroidGraphics : public Graphics {
 protected:
  EGLDisplay mEglDisplay;
  EGLContext mEglContext;
  EGLSurface mEglSurface;
  ANativeWindow *mANativeWindow = nullptr;
  bool running_b = false;

 public:
  GLVersion glVersion;
  AndroidGraphics();
  GLVersion getGLVersion() override;
  void init(JNIEnv *env, jobject instance, jobject surface);
  void destory() override;
  void update() override;
  void dispose() override;
  void change_surface(int width, int height) override;
  void swapBuffers();
  bool running() override;
};
}  // namespace nova
