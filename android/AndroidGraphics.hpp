#pragma once
#include "EGL/egl.h"
#include "EGL/eglplatform.h"
#include "core/Graphics.hpp"
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
  void init(ANativeWindow *window);
  void destory() override;
  void update() override;
  void dispose() override;
  void change_surface(int width, int height) override;
  void swapBuffers();
  bool running() override;
};
}  // namespace nova
