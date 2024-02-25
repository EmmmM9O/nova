package nova.android.opengl;

import android.view.Surface;
import android.util.Log;

public class NationOpenGL {

    static {
        Log.i("nova", "loadLibrary");
        try {
            System.loadLibrary("native-lib");
        } catch (Exception e) {
            Log.e("nova", "loadLibrary error" + e.toString());
        }
    }

    public native void nativeSurfaceCreate(Surface surface);

    public native void nativeSurfaceChanged(int width, int height);

    public native void nativeSurfaceDestroyed();

}
