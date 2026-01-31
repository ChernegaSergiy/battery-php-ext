package com.example.batterytest;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import com.example.Battery;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("battery_test");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        Battery.init(this);

        TextView tv = new TextView(this);
        tv.setTextSize(24f);
        tv.setPadding(50, 50, 50, 50);
        setContentView(tv);

        String info = getBatteryInfoFromJNI();
        tv.setText(info);
    }

    public native String getBatteryInfoFromJNI();
}
