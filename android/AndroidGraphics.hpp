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

public:
  GLVersion glVersion;
  AndroidGraphics();
  GLVersion getGLVersion() override;
  void init(JNIEnv *env, jobject instance, jobject surface);
  void destory() override;
  void update() override;
  void create_window() override;
  void dispose() override;
  void swapBuffers();
};
} // namespace nova
