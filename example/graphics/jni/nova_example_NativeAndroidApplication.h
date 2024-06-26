/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class nova_example_NativeAndroidApplication */

#ifndef _Included_nova_example_NativeAndroidApplication
#define _Included_nova_example_NativeAndroidApplication
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     nova_example_NativeAndroidApplication
 * Method:    nativeSurfaceCreate
 * Signature: (Landroid/view/Surface;Lnova/example/opengl/GLSurfaceView;)V
 */
JNIEXPORT void JNICALL Java_nova_example_NativeAndroidApplication_nativeSurfaceCreate
  (JNIEnv *, jclass, jobject, jobject);

/*
 * Class:     nova_example_NativeAndroidApplication
 * Method:    nativeSurfaceChanged
 * Signature: (IILnova/example/opengl/GLSurfaceView;)V
 */
JNIEXPORT void JNICALL Java_nova_example_NativeAndroidApplication_nativeSurfaceChanged
  (JNIEnv *, jclass, jint, jint, jobject);

/*
 * Class:     nova_example_NativeAndroidApplication
 * Method:    nativeSurfaceDestroyed
 * Signature: (Lnova/example/opengl/GLSurfaceView;)V
 */
JNIEXPORT void JNICALL Java_nova_example_NativeAndroidApplication_nativeSurfaceDestroyed
  (JNIEnv *, jclass, jobject);

/*
 * Class:     nova_example_NativeAndroidApplication
 * Method:    initJNI
 * Signature: (Lnova/example/AndroidApplication;)V
 */
JNIEXPORT void JNICALL Java_nova_example_NativeAndroidApplication_initJNI
  (JNIEnv *, jclass, jobject);

/*
 * Class:     nova_example_NativeAndroidApplication
 * Method:    onDestory
 * Signature: (Lnova/example/AndroidApplication;)V
 */
JNIEXPORT void JNICALL Java_nova_example_NativeAndroidApplication_onDestory
  (JNIEnv *, jclass, jobject);

#ifdef __cplusplus
}
#endif
#endif
