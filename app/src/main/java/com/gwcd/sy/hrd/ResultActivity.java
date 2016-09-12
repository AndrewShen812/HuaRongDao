package com.gwcd.sy.hrd;

import android.content.Context;
import android.content.res.Resources;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

public class ResultActivity extends AppCompatActivity {

    private RecyclerView mQpList;
    private ResultAdapter mAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        Bundle b = getIntent().getExtras();
//        if (b == null) {
//            finish();
//            return;
//        }
//        Parcelable[] array = b.getParcelableArray("QpList");
//        Qipan[] dataList = (Qipan[]) b.getParcelableArray("QpList");
//        if (dataList == null || dataList.length == 0) {
//            finish();
//            return;
//        }
        setContentView(R.layout.activity_result);
        mQpList = (RecyclerView) findViewById(R.id.rv_result_qp_list);
        mAdapter = new ResultAdapter(this, MainActivity.mQpResult);
        mQpList.setLayoutManager(new LinearLayoutManager(this));
        mQpList.setAdapter(mAdapter);
    }

    private class ResultAdapter extends RecyclerView.Adapter<QpHolder> {
        private Qipan[] mData;
        private Resources mRes;
        private String mPackage;

        public ResultAdapter(Context context, Qipan[]mData) {
            mRes = context.getResources();
            mPackage = context.getApplicationContext().getPackageName();
            this.mData = mData;
        }

        @Override
        public QpHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            QpHolder holder = new QpHolder(mRes, mPackage,
                    getLayoutInflater().inflate(R.layout.layout_qipan, parent, false));
            return holder;
        }

        @Override
        public void onBindViewHolder(QpHolder holder, int position) {
            holder.mStep.setVisibility(View.VISIBLE);
            holder.mStep.setText("第" + position + "步");
            Qipan qp = mData[position];
            for (int i = 0; i < holder.mGrids.length; i++) {
                if (qp.qp[i] == 5) {
                    holder.mGrids[i].setText("曹");
                }
                if (qp.qp[i] == 4) {
                    holder.mGrids[i].setText("横");
                }
                if (qp.qp[i] == 3) {
                    holder.mGrids[i].setText("竖");
                }
                if (qp.qp[i] == 2) {
                    holder.mGrids[i].setText("卒");
                }
                if (qp.qp[i] == 0) {
                    holder.mGrids[i].setText("");
                }
            }
        }

        @Override
        public int getItemCount() {
            return mData == null ? 0 : mData.length;
        }
    }

    private class QpHolder extends RecyclerView.ViewHolder {
        private Resources mRes;
        private String mPackage;
        private TextView mStep;
        private Button[] mGrids;
        public QpHolder(Resources res, String mPackage, View itemView) {
            super(itemView);
            this.mRes = res;
            this.mPackage = mPackage;
            mGrids = new Button[20];
            mStep = (TextView) itemView.findViewById(R.id.tv_qipan_step_num);
            for (int i = 0; i < mGrids.length; i++) {
                mGrids[i] = (Button) itemView.findViewById(mRes.getIdentifier("btn_qp_grid" + (i + 1), "id", mPackage));
                mGrids[i].setClickable(false);
            }
        }
    }
}
