package com.gwcd.sy.hrd;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onCallJniPlay(View view) {
        char[] qipan = {
                4, 4, 5, 5,
                4, 4, 1, 1,
                4, 4, 4, 4,
                2, 0, 2, 3,
                2, 0, 2, 1};
        Qipan[] qpResult = HrdLib.hrdPlay(qipan);
        if (qpResult != null) {
            Log.d("sy", "get qpResult");
            for (Qipan qp : qpResult) {
                Log.d("sy", "下一步：");
                int[] r = new int[20];
                for (int i = 0; i < 20; i++) {
                    r[i] = qp.qp[i];
                }
                Log.d("sy", r[0] + " " + r[1] + " " + r[2] + " " + r[3]);
                Log.d("sy", r[4] + " " + r[5] + " " + r[6] + " " + r[7]);
                Log.d("sy", r[8] + " " + r[9] + " " + r[10] + " " + r[11]);
                Log.d("sy", r[12] + " " + r[13] + " " + r[14] + " " + r[15]);
                Log.d("sy", r[16] + " " + r[17] + " " + r[18] + " " + r[19]);
            }
        } else {
            Log.e("sy", "qpResult null");
        }
    }
}
