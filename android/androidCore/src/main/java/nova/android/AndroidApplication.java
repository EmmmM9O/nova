package nova.android;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import java.lang.Thread.*;
import java.util.*;
import java.io.*;
public class AndroidApplication extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
	    UncaughtExceptionHandler handler = Thread.getDefaultUncaughtExceptionHandler();
	    Thread.setDefaultUncaughtExceptionHandler((thread, error) -> {
				if(handler != null){
					                handler.uncaughtException(thread, error);
							            }else{
									                    Log.e("nova",error.toString());
											                    System.exit(1);
													                }
	    });
        Log.i("nova", "nova init");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

}
