#include <jni.h>

#include <memory>

#include "AndroidApplication.hpp"
#include "AndroidCore.hpp"
#include "core/Log.hpp"
#include "nova_example_NativeAndroidApplication.h"
#include "source_location"
std::shared_ptr<nova::AndroidApplication> nova::android::app =
    std::make_shared<AndroidApplication>(nova::AndroidApplication());
JNIEXPORT void JNICALL
Java_nova_example_NativeAndroidApplication_nativeSurfaceCreate(JNIEnv *env,
                                                               jclass,
                                                               jobject surface,
                                                               jobject view) {
  nova::android::app->createSurcafe(env, view, surface);
}
/*
 * Class:     nova_example_NativeAndroidApplication
 * Method:    nativeSurfaceChanged
 * Signature: (IILnova/android/opengl/GLSurfaceView;)V
 */
JNIEXPORT void JNICALL
Java_nova_example_NativeAndroidApplication_nativeSurfaceChanged(
    JNIEnv *env, jclass, jint width, jint height, jobject view) {
  nova::android::app->surfaceChange(env, view, width, height);
}

/*
 * Class:     nova_example_NativeAndroidApplication
 * Method:    nativeSurfaceDestroyed
 * Signature: (Lnova/android/opengl/GLSurfaceView;)V
 */
JNIEXPORT void JNICALL
Java_nova_example_NativeAndroidApplication_nativeSurfaceDestroyed(
    JNIEnv *env, jclass, jobject view) {
  nova::android::app->surfaceDestory(env, view);
}

/*
 * Class:     nova_example_NativeAndroidApplication
 * Method:    initJNI
 * Signature: (Lnova/android/AndroidApplication;)V
 */
JNIEXPORT void JNICALL Java_nova_example_NativeAndroidApplication_initJNI(
    JNIEnv *env, jclass, jobject activity) {
  nova::android::app->initialize(env, activity, nullptr, {});
  nova::Log::my_logger.log(std::source_location::current(),
                           nova::LogLevel::Info, "sdk version {}",
                           nova::android::app->getVersion());
}
JNIEXPORT void JNICALL Java_nova_example_NativeAndroidApplication_onDestory(
    JNIEnv *env, jclass, jobject activity) {
  nova::android::app->onDestroy(env, activity);
}
