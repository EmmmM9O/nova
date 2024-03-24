#include <jni.h>

#include <memory>

#include "android/AndroidApplication.hpp"
#include "nova_example_NativeAndroidApplication.h"
nova::AndroidApplication androidApp;
JNIEXPORT void JNICALL
Java_nova_example_NativeAndroidApplication_nativeSurfaceCreate(JNIEnv *env,
                                                               jclass,
                                                               jobject surface,
                                                               jobject view) {
  androidApp.createSurcafe(env, view, surface);
}
/*
 * Class:     nova_example_NativeAndroidApplication
 * Method:    nativeSurfaceChanged
 * Signature: (IILnova/android/opengl/GLSurfaceView;)V
 */
JNIEXPORT void JNICALL
Java_nova_example_NativeAndroidApplication_nativeSurfaceChanged(
    JNIEnv *env, jclass, jint width, jint height, jobject view) {
  androidApp.surfaceChange(env, view, width, height);
}

/*
 * Class:     nova_example_NativeAndroidApplication
 * Method:    nativeSurfaceDestroyed
 * Signature: (Lnova/android/opengl/GLSurfaceView;)V
 */
JNIEXPORT void JNICALL
Java_nova_example_NativeAndroidApplication_nativeSurfaceDestroyed(
    JNIEnv *env, jclass, jobject view) {
  androidApp.surfaceDestory(env, view);
}

/*
 * Class:     nova_example_NativeAndroidApplication
 * Method:    initJNI
 * Signature: (Lnova/android/AndroidApplication;)V
 */
JNIEXPORT void JNICALL Java_nova_example_NativeAndroidApplication_initJNI(
    JNIEnv *env, jclass, jobject activity) {
  androidApp.initialize(env, activity, nullptr, {});
}
JNIEXPORT void JNICALL Java_nova_example_NativeAndroidApplication_onDestory(
    JNIEnv *env, jclass, jobject activity) {
  androidApp.onDestroy(env, activity);
}
