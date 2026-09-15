package com.pvr.exp001.harness;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public final class MainActivity extends Activity {
    private static final String TAG = "EXP-001-Harness";

    static {
        // Normal Android dynamic loading in this controlled application.
        System.loadLibrary("pvr_minecraft_bridge");
        System.loadLibrary("exp001_harness");
    }

    private static native int nativeExplicitInitialize();

    @Override
    protected void onCreate(Bundle state) {
        super.onCreate(state);
        int result = nativeExplicitInitialize();
        Log.i(TAG, "bridge_initialize result=" + result);

        TextView status = new TextView(this);
        status.setText("EXP-001 Android native loading harness started\n"
                + "See logcat tag PVR-Bridge for PVR_BRIDGE_LOADED.");
        setContentView(status);
    }
}
