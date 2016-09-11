package com.gwcd.sy.hrd;

/**
 * Created by Lenovo on 2016/9/10.
 */
public class HrdLib {

    static {
        System.loadLibrary("hrd_jni");
    }

    public static native Qipan[] hrdPlay(char[] qp);
}
