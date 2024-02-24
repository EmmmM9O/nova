package nova.android.opengl;

import android.content.Context;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class GLSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
    private NationOpenGL mNationOpenGL;

    public GLSurfaceView(Context context) {
        this(context, null);
    }

    public GLSurfaceView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public GLSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mNationOpenGL = new NationOpenGL();
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        mNationOpenGL.nativeSurfaceCreate(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder arg0, int arg1, int width, int height) {
        mNationOpenGL.nativeSurfaceChanged(width, height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        mNationOpenGL.nativeSurfaceDestroyed();
    }
}
