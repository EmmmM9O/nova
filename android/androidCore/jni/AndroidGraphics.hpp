#pragma once
#include "EGL/egl.h"
#include "core/Graphics.hpp"
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
  void setupGL();
  void init(JNIEnv *env, jobject instance, jobject surface);
  void destory();
};
}  // namespace nova
