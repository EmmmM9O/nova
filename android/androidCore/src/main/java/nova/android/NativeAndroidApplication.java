package nova.android;

import android.view.Surface;
import nova.android.opengl.GLSurfaceView;
import android.util.Log;

public class NativeAndroidApplication {

    static {
        Log.i("nova", "loadLibrary");
        try {
            System.loadLibrary("native-lib");
        } catch (Exception e) {
            Log.e("nova", "loadLibrary error" + e.toString());
        }
    }
    public static NativeAndroidApplication androidApplication;

    public static void init() {
        androidApplication = new NativeAndroidApplication();
    }

    public native void nativeSurfaceCreate(Surface surface, GLSurfaceView thiz);

    public native void nativeSurfaceChanged(int width, int height, GLSurfaceView thiz);

    public native void nativeSurfaceDestroyed(GLSurfaceView thiz);

    public native void initJNI(AndroidApplication thiz);

    public native void onDestory(AndroidApplication thiz);
}
