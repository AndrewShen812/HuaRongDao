package com.gwcd.sy.hrd;

import android.content.Intent;
import android.content.res.Resources;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.RadioGroup;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final int QP_SIZE = 20;

    private RadioGroup mSelGroup;
    private Button[] mQpGrids = new Button[QP_SIZE];
    private GridType mType = GridType.INVALID;

    public static Qipan[] mQpResult;

    private enum GridType {
        CAO, HENG, SHU, ZU, SPACE, INVALID;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mSelGroup = (RadioGroup) findViewById(R.id.rg_sel_group);
        mSelGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                switch (checkedId) {
                    case R.id.rb_sel_cao: mType = GridType.CAO; break;
                    case R.id.rb_sel_heng: mType = GridType.HENG; break;
                    case R.id.rb_sel_shu: mType = GridType.SHU; break;
                    case R.id.rb_sel_zu: mType = GridType.ZU; break;
                    case R.id.rb_sel_space: mType = GridType.SPACE; break;
                }
            }
        });
        Resources res = getResources();
        String packageName = getApplicationContext().getPackageName();
        for (int i = 0; i < mQpGrids.length; i++) {
            mQpGrids[i] = (Button) findViewById(res.getIdentifier("btn_qp_grid" + (i + 1), "id", packageName));
            mQpGrids[i].setOnClickListener(this);
        }
    }

    public void onCallClear(View view) {
        for (int i = 0; i < mQpGrids.length; i++) {
            mQpGrids[i].setText("");
        }
    }

    public void onCallJniPlay(View view) {
        //曹操是5(只能1个),横将是4,竖将是3,兵是2,空位用0表示
        //曹操与竖将前两个须相同,余占位填充1,
//        char[] qipan = {
//                2, 2, 5, 5,
//                4, 4, 1, 1,
//                3, 2, 4, 4,
//                1, 2, 4, 4,
//                0, 0, 4, 4};
        char[] qipan = new char[mQpGrids.length];
        for (int i = 0; i < mQpGrids.length; i++) {
            if("曹".equals(mQpGrids[i].getText().toString())) {
                qipan[i] = 5;
                qipan[i] = (i >= 4 && qipan[i] == qipan[i - 4]) ? (char)1 : (char)5;
            }
            if("横".equals(mQpGrids[i].getText().toString())) {
                qipan[i] = 4;
            }
            if("竖".equals(mQpGrids[i].getText().toString())) {
                qipan[i] = 3;
                qipan[i] = (i >= 4 && qipan[i] == qipan[i - 4]) ? (char)1 : (char)3;
            }
            if("卒".equals(mQpGrids[i].getText().toString())) {
                qipan[i] = 2;
            }
            if(TextUtils.isEmpty(mQpGrids[i].getText().toString())) {
                qipan[i] = 0;
            }
        }
        mQpResult = HrdLib.hrdPlay(qipan);
        if (mQpResult != null) {
            Log.d("sy", "get qpResult");
//            for (Qipan qp : qpResult) {
//                Log.d("sy", "下一步：");
//                int[] r = new int[20];
//                for (int i = 0; i < 20; i++) {
//                    r[i] = qp.qp[i];
//                }
//                Log.d("sy", r[0] + " " + r[1] + " " + r[2] + " " + r[3]);
//                Log.d("sy", r[4] + " " + r[5] + " " + r[6] + " " + r[7]);
//                Log.d("sy", r[8] + " " + r[9] + " " + r[10] + " " + r[11]);
//                Log.d("sy", r[12] + " " + r[13] + " " + r[14] + " " + r[15]);
//                Log.d("sy", r[16] + " " + r[17] + " " + r[18] + " " + r[19]);
//            }
            Intent intent = new Intent(MainActivity.this, ResultActivity.class);
//            Bundle d = new Bundle();
//            d.putParcelableArray("QpList", qpResult);
//            intent.putExtras(d);
            startActivity(intent);
        } else {
            Toast.makeText(MainActivity.this, "无解", Toast.LENGTH_SHORT).show();
            Log.e("sy", "qpResult null");
        }
    }

    @Override
    public void onClick(View v) {
        Button grid = (Button)v;
        if (grid == null) {
            return;
        }
        switch (mType) {
            case CAO: grid.setText("曹"); break;
            case HENG: grid.setText("横"); break;
            case SHU: grid.setText("竖"); break;
            case ZU: grid.setText("卒"); break;
            case SPACE: grid.setText(""); break;
        }
    }
}
