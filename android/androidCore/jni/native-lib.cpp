#include <jni.h>

#include <string>

#include "GLES3/gl3.h"
#include "android/log.h"
#include "android/native_window.h"
#include "android/native_window_jni.h"
#include "egl/EglThread.h"
#include "nova_android_opengl_NationOpenGL.h"

void callBackOnCreate() { LOGE("callBackOnCreate"); }

void callBackOnChange(int width, int height) {
  glViewport(0, 0, width, height);
  LOGE("callBackOnChange");
}

void callBackOnDraw() {
  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  LOGE("callBackOnDraw");
}
EglThread *eglThread = NULL;
/*
 * Class:     nova_android_opengl_NationOpenGL
 * Method:    nativeSurfaceCreate
 * Signature: (Landroid/view/Surface;)V
 */
extern "C" JNIEXPORT void JNICALL
Java_nova_android_opengl_NationOpenGL_nativeSurfaceCreate(JNIEnv *env,
                                                          jobject instance,
                                                          jobject surface) {
  eglThread = new EglThread();
  eglThread->callBackOnCreate(callBackOnCreate);
  eglThread->callBackOnChange(callBackOnChange);
  eglThread->callBackOnDraw(callBackOnDraw);
  eglThread->setRenderModule(RENDER_MODULE_MANUAL);

  ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
  eglThread->onSurfaceCreate(nativeWindow);
}

/*
 * Class:     nova_android_opengl_NationOpenGL
 * Method:    nativeSurfaceChanged
 * Signature: (II)V
 */
extern "C" JNIEXPORT void JNICALL
Java_nova_android_opengl_NationOpenGL_nativeSurfaceChanged(JNIEnv *env,
                                                           jobject instance,
                                                           jint width,
                                                           jint height) {
  if (eglThread) {
    eglThread->onSurfaceChange(width, height);
  }
}

/*
 * Class:     nova_android_opengl_NationOpenGL
 * Method:    nativeSurfaceDestroyed
 * Signature: ()V
 */
extern "C" JNIEXPORT void JNICALL
Java_nova_android_opengl_NationOpenGL_nativeSurfaceDestroyed(JNIEnv *env,
                                                             jobject instance) {
  if (eglThread) {
    eglThread->isExit = true;
    pthread_join(eglThread->mEglThread, NULL);
    delete (eglThread);
    eglThread = NULL;
  }
}

