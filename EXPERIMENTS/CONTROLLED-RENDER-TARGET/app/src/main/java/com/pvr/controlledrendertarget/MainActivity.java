package com.pvr.controlledrendertarget;

import android.app.Activity;
import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;
import android.widget.FrameLayout;

public final class MainActivity extends Activity implements SurfaceHolder.Callback {
    static { System.loadLibrary("pvr_minecraft_bridge"); System.loadLibrary("controlled_renderer"); }

    private SurfaceView surface;
    private TextView status;

    private static native void nativeStart(Surface surface);
    private static native void nativeStop();

    @Override public void onCreate(Bundle state) {
        super.onCreate(state);
        FrameLayout root = new FrameLayout(this);
        surface = new SurfaceView(this);
        status = new TextView(this);
        status.setText("PVR Controlled Render Target\nWaiting for surface...");
        root.addView(surface);
        root.addView(status);
        setContentView(root);
        surface.getHolder().addCallback(this);
    }

    @Override public void surfaceCreated(SurfaceHolder holder) {
        nativeStart(holder.getSurface());
        status.setText("PVR Controlled Render Target\nRenderer starting...");
    }

    @Override public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) { }

    @Override public void surfaceDestroyed(SurfaceHolder holder) {
        nativeStop();
        status.setText("Renderer stopped");
    }
}
