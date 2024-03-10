//
// Created by zeng on 2019/4/22.
//

#ifndef NATIVEOPNEGLDEMO_EGLHELPER_H
#define NATIVEOPNEGLDEMO_EGLHELPER_H

#include "../log/JniLog.h"
#include "EGL/egl.h"

class EglHelper {
 public:
  EGLDisplay mEglDisplay;
  EGLContext mEglContext;
  EGLSurface mEglSurface;

 public:
  EglHelper();

  ~EglHelper();

  int initEgl(EGLNativeWindowType surface);
  int swapBuffers();
  void destroyEgl();
};

#endif  // NATIVEOPNEGLDEMO_EGLHELPER_H
