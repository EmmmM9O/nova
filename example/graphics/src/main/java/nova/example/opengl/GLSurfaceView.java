package nova.example.opengl;

import android.content.Context;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import nova.example.NativeAndroidApplication;

public class GLSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
    public GLSurfaceView(Context context) {
        this(context, null);
    }

    public GLSurfaceView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public GLSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        NativeAndroidApplication.nativeSurfaceCreate(holder.getSurface(), this);
    }

    @Override
    public void surfaceChanged(SurfaceHolder arg0, int arg1, int width, int height) {
        NativeAndroidApplication.nativeSurfaceChanged(width, height, this);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        NativeAndroidApplication.nativeSurfaceDestroyed(this);
    }
}
