#include "AndroidGraphics.hpp"

#include "EGL/egl.h"
#include "EGL/eglplatform.h"
#include "GLES2/gl2.h"
#include "android/native_window.h"
#include "android/native_window_jni.h"
#include "core/Graphics.hpp"
#include "core/Log.hpp"
#include "core/application.hpp"
#include "log/JniLog.h"
namespace nova {
AndroidGraphics::AndroidGraphics() {
  mEglDisplay = EGL_NO_DISPLAY;
  mEglSurface = EGL_NO_SURFACE;
  mEglContext = EGL_NO_CONTEXT;
}
void AndroidGraphics::init(ANativeWindow *native_window) {
  mANativeWindow = native_window;
  // 1.得到默认的显示设备（就是窗口） -- eglGetDisplay
  mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (mEglDisplay == EGL_NO_DISPLAY) {
    Log_error("eglGetDisplay error");
    return;
  }

  // 2. 初始化默认显示设备 -- eglInitialize
  EGLint majorVersion, minorVersion;
  if (!eglInitialize(mEglDisplay, &majorVersion, &minorVersion)) {
    Log_error("eglInitialize error");
    return;
  }
  // 3. 设置显示设备的属性
  const EGLint attrib_config_list[] = {EGL_RED_SIZE,
                                       8,
                                       EGL_GREEN_SIZE,
                                       8,
                                       EGL_BLUE_SIZE,
                                       8,
                                       EGL_ALPHA_SIZE,
                                       8,
                                       EGL_DEPTH_SIZE,
                                       8,
                                       EGL_STENCIL_SIZE,
                                       8,  // 眼睛屏幕的距离
                                       EGL_RENDERABLE_TYPE,
                                       EGL_OPENGL_ES2_BIT,  // 版本号
                                       EGL_NONE};

  // 3.1
  // 根据所需的参数获取符合该参数的config_size，主要是解决有些手机eglChooseConfig失败的兼容性问题
  EGLint num_config;

  if (!eglChooseConfig(mEglDisplay, attrib_config_list, NULL, 1, &num_config)) {
    Log_error("eglChooseConfig error");
    return;
  }
  // 3.2 根据获取到的config_size得到eglConfig
  EGLConfig eglConfig;
  if (!eglChooseConfig(mEglDisplay, attrib_config_list, &eglConfig, num_config,
                       &num_config)) {
    Log_error("eglChooseConfig error");
    return;
  }

  // 4. 创建egl上下文 eglCreateContext
  const EGLint attrib_ctx_list[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  mEglContext = eglCreateContext(mEglDisplay, eglConfig, NULL, attrib_ctx_list);
  if (mEglContext == EGL_NO_CONTEXT) {
    Log_error("eglCreateContext  error");
    return;
  }
  // 5.创建渲染的surface
  mEglSurface =
      eglCreateWindowSurface(mEglDisplay, eglConfig, mANativeWindow, NULL);
  if (mEglSurface == EGL_NO_SURFACE) {
    Log_error("eglCreateWindowSurface  error");
    return;
  }
  // 6. 绑定eglContext和surface到display
  if (!eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
    Log_error("eglMakeCurrent  error");
    return;
  }
  glVersion =
      nova::GLVersion(nova::systemType::Android,
                      reinterpret_cast<const char *>(glGetString(GL_VENDOR)),
                      reinterpret_cast<const char *>(glGetString(GL_RENDERER)),
                      reinterpret_cast<const char *>(glGetString(GL_VERSION)));
  // 7. 刷新数据，显示渲染场景 -- eglSwapBuffers
  running_b = true;
}
void AndroidGraphics::destory() {
  running_b = false;
  if (mEglDisplay != EGL_NO_DISPLAY) {
    // 解绑display上的eglContext和surface
    eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    // 销毁surface 和 eglContext
    if (mEglSurface != EGL_NO_SURFACE) {
      eglDestroySurface(mEglDisplay, mEglSurface);
      mEglSurface = EGL_NO_SURFACE;
    }

    if (mEglContext != EGL_NO_CONTEXT) {
      eglDestroyContext(mEglDisplay, mEglContext);
      mEglContext = EGL_NO_CONTEXT;
    }

    if (mEglDisplay != EGL_NO_DISPLAY) {
      eglTerminate(mEglDisplay);
      mEglDisplay = EGL_NO_DISPLAY;
    }
  }
}
void AndroidGraphics::swapBuffers() {
  if (mEglDisplay != EGL_NO_DISPLAY && mEglSurface != EGL_NO_SURFACE &&
      eglSwapBuffers(mEglDisplay, mEglSurface)) {
  }
}
GLVersion AndroidGraphics::getGLVersion() { return glVersion; }
void AndroidGraphics::update() {
  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  swapBuffers();
}
void AndroidGraphics::dispose() {}
void AndroidGraphics::change_surface(int width, int height) {
  glViewport(0, 0, width, height);
}
bool AndroidGraphics::running() { return running_b; }
}  // namespace nova
