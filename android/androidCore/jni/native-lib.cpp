#include <jni.h>

#include <memory>
#include "AndroidApplication.hpp"
#include "nova_android_NativeAndroidApplication.h"
std::shared_ptr<nova::AndroidApplication> app(new nova::AndroidApplication());
JNIEXPORT void JNICALL
Java_nova_android_NativeAndroidApplication_nativeSurfaceCreate(JNIEnv *env,
                                                               jobject,
                                                               jobject surface,
                                                               jobject view) {
  app->createSurcafe(env, view, surface);
    
}
/*
 * Class:     nova_android_NativeAndroidApplication
 * Method:    nativeSurfaceChanged
 * Signature: (IILnova/android/opengl/GLSurfaceView;)V
 */
JNIEXPORT void JNICALL
Java_nova_android_NativeAndroidApplication_nativeSurfaceChanged(
    JNIEnv *env, jobject, jint width, jint height, jobject view) {
  app->surfaceChange(env, view, width, height);
}

/*
 * Class:     nova_android_NativeAndroidApplication
 * Method:    nativeSurfaceDestroyed
 * Signature: (Lnova/android/opengl/GLSurfaceView;)V
 */
JNIEXPORT void JNICALL
Java_nova_android_NativeAndroidApplication_nativeSurfaceDestroyed(
    JNIEnv *env, jobject, jobject view) {
  app->surfaceDestory(env, view);
}

/*
 * Class:     nova_android_NativeAndroidApplication
 * Method:    initJNI
 * Signature: (Lnova/android/AndroidApplication;)V
 */
JNIEXPORT void JNICALL Java_nova_android_NativeAndroidApplication_initJNI(
    JNIEnv *env, jobject, jobject activity) {
  app->initialize(env, activity, nullptr, {});
  LOGD("version :%d", app->getVersion());
}
JNIEXPORT void JNICALL Java_nova_android_NativeAndroidApplication_onDestory(
    JNIEnv *env, jobject, jobject activity) {
  app->onDestroy(env, activity);
}
