//
// Created by Lenovo on 2016/9/10.
//
#include <jni.h>

#ifndef HUARONGDAO_HUARONGDAO_H
#define HUARONGDAO_HUARONGDAO_H
#ifdef __cplusplus
extern "C" {
#endif

#define JAVA_CLASS com_gwcd_sy_hrd_HrdLib
// 简化jni函数接口的宏
#define NAME3(CLASS3, FUNC3) Java_##CLASS3##_##FUNC3
#define NAME2(CLASS2, FUNC2) NAME3(CLASS2, FUNC2)
#define NAME(FUNC) NAME2(JAVA_CLASS, FUNC)

//棋盘表示使用char一维数组,例:char q[20];
//大王是5(大王只能1个),横将是4,竖将是3,兵是2,空位用0表示
//大王与横将前两个须相同,余占位填充1,竖将第二个占位同样填充1
//棋盘上只能为2个空格,不能多也不能少
//---------------------------------------------------------------------------
const int COL[20]={0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};//列号表
struct PMZB{       //盘面走步集结构
    char s[10],d[10];//原位置,目标位置,最多只会有10步
    int n;           //总步数
};
typedef char QP[20];

//以下define用于棋盘复制,不要用环循,实地址直接引用要快8倍
#define qpcpy(q,p) {int *a=(int*)q,*b=(int*)p;a[0]=b[0],a[1]=b[1],a[2]=b[2],a[3]=b[3],a[4]=b[4];}/*复制棋盘*/

typedef struct {
    int len;
    char ** qp_list;
} qp_list_t;

//棋盘表示使用char一维数组,例:char q[20];
//大王是5(大王只能1个),横将是4,竖将是3,兵是2,空位用0表示
//大王与横将前两个须相同,余占位填充1,竖将第二个占位同样填充1
//棋盘上只能为2个空格,不能多也不能少
extern qp_list_t * hrd_play_hash(QP qp);

/*
 * Class:     com_gwcd_sy_hrd_HrdLib
 * Method:    hrdPlay
 * Signature: ([C)[Lcom/gwcd/sy/hrd/Qipan;
 */
//JNIEXPORT void JNICALL HRD_NAME(hrdPlay)(JNIEnv *, jclass, jcharArray);
JNIEXPORT jobjectArray JNICALL Java_com_gwcd_sy_hrd_HrdLib_hrdPlay(JNIEnv *, jclass, jcharArray);

#ifdef __cplusplus
}
#endif
#endif //HUARONGDAO_HUARONGDAO_H
