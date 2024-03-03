package nova.android;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.widget.Toast;

import java.lang.Thread.*;

public class AndroidApplication extends Activity {
    protected ClipboardManager clipboard;

    protected void init() {
        NativeAndroidApplication.init();
        UncaughtExceptionHandler handler = Thread.getDefaultUncaughtExceptionHandler();
        Thread.setDefaultUncaughtExceptionHandler((thread, error) -> {
            if (handler != null) {
                handler.uncaughtException(thread, error);
            } else {
                Log.e("nova", error.toString());
                System.exit(1);
            }
        });
        this.clipboard = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
        NativeAndroidApplication.androidApplication.initJNI(this);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        init();
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    boolean openURI(String uri) {
        try {
            startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(uri)));
            return true;
        } catch (ActivityNotFoundException e) {
            return false;
        }
    }

    public boolean openFolder(String file) {
        Uri selectedUri = Uri.parse(file);
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setDataAndType(selectedUri, "resource/folder");

        if (intent.resolveActivityInfo(getPackageManager(), 0) != null) {
            startActivity(intent);
            return true;
        } else {
            runOnUiThread(() -> {
                Toast.makeText(this, "Unable to open folder (missing valid file manager?)\n" + file, Toast.LENGTH_LONG)
                        .show();
            });
            return false;
        }
    }

    public void setClipboardText(String contents) {
        ClipData data = ClipData.newPlainText(contents, contents);
        clipboard.setPrimaryClip(data);
    }

    public String getClipboardText() {
        ClipData clip = clipboard.getPrimaryClip();
        if (clip == null)
            return null;
        CharSequence text = clip.getItemAt(0).getText();
        if (text == null)
            return null;
        return text.toString();
    }

    public int getVersion() {
        return android.os.Build.VERSION.SDK_INT;
    }

    public long getNativeHeap() {
        return Debug.getNativeHeapAllocatedSize();
    }

    public void exit() {
        if (Build.VERSION.SDK_INT < 21) {
            this.finish();
        } else {
            this.finishAndRemoveTask();
        }
    }

    @Override
    protected void onDestroy() {
        NativeAndroidApplication.androidApplication.onDestory(this);
        super.onDestroy();
        System.exit(0);
    }
}
