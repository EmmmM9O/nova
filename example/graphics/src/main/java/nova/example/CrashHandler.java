package nova.android;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Date;

public class CrashHandler implements Thread.UncaughtExceptionHandler {

    private Thread.UncaughtExceptionHandler defaultHandler;
    private File crashDir;
    public CrashHandler(File dir) {
        crashDir=dir;
        defaultHandler = Thread.getDefaultUncaughtExceptionHandler();
        Thread.setDefaultUncaughtExceptionHandler(this);
    }

    @Override
    public void uncaughtException(Thread thread, Throwable throwable) {
        saveCrashInfoToFile(throwable);
        if (defaultHandler != null) {
            defaultHandler.uncaughtException(thread, throwable);
        }
    }

    private void saveCrashInfoToFile(Throwable throwable) {
        String fileName = "crash_" + new SimpleDateFormat("yyyyMMddHHmmss").format(new Date()) + ".txt";
        File file = new File(crashDir,fileName);

        try (PrintWriter printWriter = new PrintWriter(new FileWriter(file))) {
            throwable.printStackTrace(printWriter);
            printWriter.flush();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
