#include <jni.h>

#include "AndroidApplication.hpp"
#include "core/Log.hpp"
#include "nova_android_NativeAndroidApplication.h"
#include "source_location"
#include <memory>
std::shared_ptr<nova::AndroidApplication> app(new nova::AndroidApplication());
JNIEXPORT void JNICALL
Java_nova_android_NativeAndroidApplication_nativeSurfaceCreate(JNIEnv *env,
                                                               jclass,
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
    JNIEnv *env, jclass, jint width, jint height, jobject view) {
  app->surfaceChange(env, view, width, height);
}

/*
 * Class:     nova_android_NativeAndroidApplication
 * Method:    nativeSurfaceDestroyed
 * Signature: (Lnova/android/opengl/GLSurfaceView;)V
 */
JNIEXPORT void JNICALL
Java_nova_android_NativeAndroidApplication_nativeSurfaceDestroyed(
    JNIEnv *env, jclass, jobject view) {
  app->surfaceDestory(env, view);
}

/*
 * Class:     nova_android_NativeAndroidApplication
 * Method:    initJNI
 * Signature: (Lnova/android/AndroidApplication;)V
 */
JNIEXPORT void JNICALL Java_nova_android_NativeAndroidApplication_initJNI(
    JNIEnv *env, jclass, jobject activity) {
  app->initialize(env, activity, nullptr, {});
  nova::Log::my_logger.log(std::source_location::current(),
                           nova::LogLevel::Info, "sdk version {}",
                           app->getVersion());
}
JNIEXPORT void JNICALL Java_nova_android_NativeAndroidApplication_onDestory(
    JNIEnv *env, jclass, jobject activity) {
  app->onDestroy(env, activity);
}
