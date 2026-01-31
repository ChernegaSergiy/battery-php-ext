package com.example;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;

public class Battery {
    private static Context sContext;

    public static void init(Context ctx) {
        if (ctx != null) sContext = ctx.getApplicationContext();
    }

    public static int getLevel() {
        if (sContext == null) return -1;
        BatteryManager bm = (BatteryManager) sContext.getSystemService(Context.BATTERY_SERVICE);
        if (bm != null) {
            int v = bm.getIntProperty(BatteryManager.BATTERY_PROPERTY_CAPACITY);
            if (v >= 0) return v;
        }
        IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        Intent batteryStatus = sContext.registerReceiver(null, ifilter);
        if (batteryStatus == null) return -1;
        int level = batteryStatus.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
        int scale = batteryStatus.getIntExtra(BatteryManager.EXTRA_SCALE, -1);
        if (level >= 0 && scale > 0) return (int) ((level / (float) scale) * 100);
        return -1;
    }

    public static boolean isCharging() {
        if (sContext == null) return false;
        IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        Intent batteryStatus = sContext.registerReceiver(null, ifilter);
        if (batteryStatus == null) return false;
        int status = batteryStatus.getIntExtra(BatteryManager.EXTRA_STATUS, -1);
        return status == BatteryManager.BATTERY_STATUS_CHARGING || status == BatteryManager.BATTERY_STATUS_FULL;
    }
}
