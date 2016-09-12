package com.gwcd.sy.hrd;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * Created by Lenovo on 2016/9/11.
 */
public class Qipan implements Parcelable {
    byte[] qp;

    public Qipan() {}

    protected Qipan(Parcel in) {
        in.readByteArray(qp);
//        qp = in.createByteArray();
//        Parcelable[] parcelableArray =
//                in.readParcelableArray(Byte.class.getClassLoader());
//        byte[] resultArray = null;
//        if (parcelableArray != null) {
//            resultArray = Arrays.copyOf(parcelableArray, parcelableArray.length, Byte[].class);
//        }
    }

    public static final Creator<Qipan> CREATOR = new Creator<Qipan>() {
        @Override
        public Qipan createFromParcel(Parcel in) {
            return new Qipan(in);
        }

        @Override
        public Qipan[] newArray(int size) {
            return new Qipan[size];
        }
    };

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeByteArray(qp);
    }
}
