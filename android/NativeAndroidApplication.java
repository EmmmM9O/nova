package nova.example;

import android.view.Surface;
import nova.example.opengl.GLSurfaceView;
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


    public static native void nativeSurfaceCreate(Surface surface, GLSurfaceView thiz);

    public static native void nativeSurfaceChanged(int width, int height, GLSurfaceView thiz);

    public static native void nativeSurfaceDestroyed(GLSurfaceView thiz);

    public static native void initJNI(AndroidApplication thiz);

    public static native void onDestory(AndroidApplication thiz);
}
