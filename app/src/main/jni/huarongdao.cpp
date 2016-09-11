#include "huarongdao.h"
#include "AndroidLog.h"
#include <time.h>
#include <stdio.h>
//#include <conio.h>
#include <stdlib.h>

static void jni_print(const char *fmt, ...)
{
    va_list ap;
    char temp_str[1024];
    va_start(ap, fmt);

    //字符串输出
    vsprintf(temp_str,fmt,ap);
    LOGD("%s", temp_str);
    va_end(ap);
}

// 作者原文：http://www.fjptsz.com/xxjs/xjw/rj/110.htm
//===============================================
//下文是利用哈希技术优化后的代码
//===============================================
//---------------------------------------------------------------------------
//----本程序在C++Builder6.0及VC++6.0中调试通过----
//----程序名称:"华容道之哈希算法"搜索----
//----程序设计:许剑伟----
//----最后修改时间:2006.10.22----
//----速度:横刀立马12毫秒(P2.4G机器)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//--以下定义几函数--
//---------------------------------------------------------------------------
void qkmem(void *ps,int n){ //内存块置0,同memset(mem,0,memsize);
    register int *p=(int*)ps,*p2=p+n/4;
    while(p<p2) *p++=0;
    char *p3=(char *)p,*p4=(char *)ps+n;
    while(p3<p4) *p3++=0;
}
//---------------------------------------------------------------------------
//--以下是搜索算法之一(解决哈希表问题)--
//---------------------------------------------------------------------------
#define hsize 128*1024//使用128k(17位)哈希表,如果改用更大的表,相应的哈希计算位数也要改
//以下这两个哈希计算是对棋盘折叠计算,注意异或与加法相似,不会提高散列度,适当的移位则会提高散列度
#define hs17(h1,h) h=(h1&0x0001FFFF)^(h1>>17) //17位哈希值计算(折叠式计算)
#define hs15(h1,h) h=(h1&0x00007FFF)^(h1>>19) //15位哈希值计算(折叠式计算)
#define phs(h1,h,b){if(!b[h]){b[h]=h1;return 1;} if(b[h]==h1)return 0;h++;} //哈希值测试,返回1是新节点
class PmHx{ //盘面哈希计算
    public:
    unsigned int *hsb,*hsb2; //哈希表
    int cht; //哈希冲突次数
    PmHx(){//初始化编码表
        int i;
        hsb=new unsigned int[hsize+hsize/4+64];hsb2=hsb+hsize+32; //第二哈希表大小为第一哈希表的1/4
        reset();
    }
    ~PmHx(){ delete[] hsb; }
    void reset(){ cht=0; qkmem(hsb,(hsize+hsize/4+64)*sizeof(unsigned int));}
    int check(char *q){ //盘面编码
        //生成散列参数n1,n2,m0
        //以下参数生成算法不保证参数与棋盘的唯一对应关系,因此理论上存在哈希表冲突判断错误的可能
        //只不过产生错误的可能性几乎可能完全忽略
        unsigned int i,n1,n2,m0,h,h1,*p=(unsigned int*)q;
        n1=(p[1]<<3)+(p[2]<<5)+p[0]; //每次折叠时都应充分发挥各子作用,增强散列
        n2=(p[3]<<1)+(p[4]<<4);
        m0=(n2<<6)^(n1<<3); //增强散列参数
        int a=1;
        //第一哈希处理
        h1=n1+n2+m0; hs17(h1,h);//h1为散列和,h为第一哈希索引
        for(i=0;i<2;i++) phs(h1,h,hsb); //多次查表,最多32次
        //第二哈希处理
        h1=n1-n2+m0; hs15(h1,h);//h1为散列差,h为第二哈希值
        for(i=0;i<10;i++) phs(h1,h,hsb2); //首次查表
        cht++; //哈希冲突计数(通过5次哈希,一般情况下冲突次数为0)
        return 1;
    }
    void check2(char *q){ //按左右对称规则考查棋盘,并记录到哈希表
        char i,q2[20];
        for(i=0;i<20;i+=4) q2[i]=q[i+3],q2[i+1]=q[i+2],q2[i+2]=q[i+1],q2[i+3]=q[i];
        check(q2);
        //check2()执行次数较少,是实节点的次数,约为check()执行次数的1/3,所以不必过份求其速度
    }
}; //建立哈希表索引器
//---------------------------------------------------------------------------
//以下设计走法生成器
//---------------------------------------------------------------------------
#define zin0(des) z->s[z->n]=i,z->d[z->n++]=des //保存步法宏
#define zin1(des) z->s[z->n]=i-1,z->d[z->n++]=des-1 //保存步法宏(左移1列)
#define zin4(des) z->s[z->n]=i-4,z->d[z->n++]=des-4 //保存步法宏(上移1行)
#define zinb(des,fx) i=des+(fx); if(q[i]==2) {if(h){zin0(k1);zin0(k2);}else zin0(des);}
void zbFX(char *q,PMZB *z){ //分析当前可能的步法,并将所有可能的步法保存在z中
    int i,k1=-1,k2,h=0;z->n=0;  //k1空格1位置,k2空格2位置,h为两空格的联合类型,计步复位
    for(i=0;i<20;i++){ if(!q[i]){if(k1==-1) k1=i; else { k2=i; break; }} } //查空格的位置
    int col1=COL[k1],col2=COL[k2];
    if(k1+4==k2) h=1;         //空格竖联合
    if(k1+1==k2&&col1<3) h=2; //空格横联合
    if(col1>0){zinb(k1,-1);
        if(q[i]==3) {if(h==1) zin0(k1);}
        if(q[i]==5) {if(h==1) zin1(k1);}
        if(q[i]==4) {if(h==2) zin1(k2); zin1(k1);}
    }
    if(col1<3){zinb(k1,1);
        if(q[i]==3) {if(h==1) zin0(k1);}
        if(q[i]==5) {if(h==1) zin0(k1);}
        if(q[i]==4) {zin0(k1);} //如果横联合,k1不是第一空,所以不用判断h
    }
    if(k1>3){zinb(k1,-4);
        if(q[i]==4&&q[i+1]==4&&(col1!=1||q[i-1]!=4)){ if(h==2) zin0(k1); }
        if(q[i]==1){
            if(q[i-4]==3) {if(h==1) zin4(k2); zin4(k1);}
            if(q[i-4]==5&&q[i-3]==5){if(h==2) zin4(k1);}
        }
    }
    if(k1<16){zinb(k1,4);
        if(q[i]==3) zin0(k1);
        if(q[i]==4&&q[i+1]==4&&(col1!=1||q[i-1]!=4)){ if(h==2) zin0(k1); }
        if(q[i]==5&&q[i+1]==5){ if(h==2) zin0(k1); }
    }
    if(col2>0){zinb(k2,-1); if(q[i]==4) zin1(k2); }
    if(k2>3)  {zinb(k2,-4); if(q[i]==1&&q[i-4]==3)zin4(k2);}
    if(col2<3){zinb(k2,1);  if(q[i]==4) {if(h==2) zin0(k1); zin0(k2);}}
    if(k2<16) {zinb(k2,4);  if(q[i]==3) {if(h==1) zin0(k1); zin0(k2);}}
}
//---------------------------------------------------------------------------
//--以下是搜索过程（广度优先）--
//---------------------------------------------------------------------------
class ZBD{ //走步队(搜索器)
    public:
    QP *z;       //棋盘队列
    int dn,dm,cur;//队(队尾),队头及队头内走步集游标
    PMZB zbj;    //队头走步集
    int *hs;     //回溯用的指针,指向父亲(对应的父亲)
    char*hss;    //对应的源步
    int max;     //最大队长
    int *res,ren;//结果
    int js,js2;  //搜索情况计数
    PmHx Hx;     //希希处理类
    ZBD(int k){ z=new QP[k]; hs=new int[k*2+500]; res=hs+k; hss=new char[k]; max=k; reset(); }
    ~ZBD(){ delete[] z; delete[] hs; delete[] hss; }
    void reset() { dn=0; dm=0,cur=-1; ren=0; js=js2=0; hss[dn]=-1;}
    void zb(char *q,char s,char d){ //走一步函数
        char c=q[s];q[s]=0;
        switch(c){
            case 3: q[s+4]=0; q[d+4]=1; break; //竖,余位填充1
            case 4: q[s+1]=0; q[d+1]=c; break; //横
            case 5: q[s+1]=q[s+4]=q[s+5]=0; q[d+1]=c,q[d+4]=q[d+5]=1; break;
        }q[d]=c;
    }
    int zbcd(char *q){ //走步出队
        if(cur==-1) zbFX(z[dm],&zbj);
        cur++; if(cur>=zbj.n) {dm++,cur=-1; return 1;} //步集游标控制
        if(hss[dm]==zbj.s[cur]) return 1;//和上次移动同一个棋子时不搜索,可提速20%左右
        qpcpy(q,z[dm]); zb(q,zbj.s[cur],zbj.d[cur]); //走步后产生新节点,结果放在q中(出队)
        return 0;
    }
    void zbrd(char *q){ //走步入队
        if(dn>=max) { jni_print("队溢出.\r\n"); return; }
        qpcpy(z[dn],q); //出队
        if(cur>=0) hss[dn]=zbj.d[cur]; //记录下移动的目标位置(用于剪枝)
        hs[dn++]=dm; //记录回溯点
    }
    char* getre(int n){ return z[res[n]];} //取第n步盘面
    int bfs(char *qp,int dep,int all=0){ //广度优先搜索,参数为棋盘及搜索最大深度,all为穷举节点总数
        if(dep>500||dep<=0) dep=200;
        reset(); Hx.reset(); //哈希表及队复位
        char q[20]; qpcpy(q,qp);
        int i,k;
        for(zbrd(q),i=1;i<=dep;i++){ //一层一层的搜索
            k=dn; if(dm==k) return -1;
            if(all)jni_print("第%d层共%d节点\r\n",i-1,k-dm);
            while(dm<k){ //广度优先
                if(zbcd(q)) continue;     //返回1说明是步集出队,不是步出队
                js++; //遍历总次数计数
                if(q[13]==5&&q[14]==5&&!all) { //大王出来了
                    for(ren=i,k=ren-2,res[ren-1]=dm;k>=0;k--) res[k]=hs[res[k+1]]; //回溯
                    return 1;
                }
                if(i<dep&&Hx.check(q)){ //到了最后一层可以不再入队了
                    js2++;       //js2遍历的实结点个数,js2不包括起始点,出队时阻止了回头步,始节点不可能再遍历
                    Hx.check2(q);//对称节点做哈希处理
                    zbrd(q);
                }
            }
        }
        return 0;
    }
}S(40*1024); //建立搜索引擎
//---------------------------------------------------------------------------
//----输入输出部分----
//---------------------------------------------------------------------------
void prt(char *q){ //打印棋盘
    int i,j,k;
    char y[20],x[20],xy[20],p[20],c1,c2;
    for(i=0;i<20;i++){
        y[i]='|',x[i]='-',xy[i]='+';
        if(q[i]) p[i]=q[i]+48; else p[i]=' ';
        if(q[i]==1) p[i]=p[i-4];
    }
    for(i=0;i<20;i++){
        if(q[i]==0) {if(COL[i]<3&&q[i+1]==0) y[i]=' '; if(i<16&&q[i+4]==0) x[i]=' ';}
        if(q[i]==3) { x[i]='.'; }
        if(q[i]==4) { y[i]='.'; i++; }
        if(q[i]==5) { y[i]=' '; y[i+4]=' '; x[i]=' '; x[i+1]=' '; xy[i]='.'; i++; }
    }
    jni_print("+-+-+-+-+\r\n");
    for(i=0;i<5;i++){
        k=i*4;
        jni_print("|");
        for(j=0;j<4;j++){ jni_print("%c%c",p[k+j],y[k+j]); }
        jni_print("\r\n|");
        for(j=0;j<4;j++){ jni_print("%c%c",x[k+j],xy[k+j]); }
        jni_print("\r\n");
    }
}

void prt_android(char * r) {
    int j;
    for(j=0;j<20;j++){
//        if(r[j]) r[j]=r[j]+48; else r[j]=' ';
//        if(r[j]==1+48) r[j]=r[j-4];
        if(r[j]==1) r[j]=r[j-4];
    }
    jni_print("%c %c %c %c", r[0]+48, r[1]+48, r[2]+48, r[3]+48);
    jni_print("%c %c %c %c", r[4]+48, r[5]+48, r[6]+48, r[7]+48);
    jni_print("%c %c %c %c", r[8]+48, r[9]+48, r[10]+48, r[11]+48);
    jni_print("%c %c %c %c", r[12]+48, r[13]+48, r[14]+48, r[15]+48);
    jni_print("%c %c %c %c", r[16]+48, r[17]+48, r[18]+48, r[19]+48);
}
//---------------------------------------------------------------------------
//棋盘表示使用char一维数组,例:char q[20];
//大王是5(大王只能1个),横将是4,竖将是3,兵是2,空位用0表示
//大王与横将前两个须相同,余占位填充1,竖将第二个占位同样填充1
//棋盘上只能为2个空格,不能多也不能少
/*如：
QP qp={
   3,5,5,3,
   1,1,1,1,
   3,4,4,3,
   1,2,2,1,
   2,0,0,2
 };
 */
qp_list_t* hrd_play_hash(QP qp_in){
    int i,ok,t1=clock();
    qp_list_t * qp_list = NULL;
    char ** qp_record = NULL;
    if (qp_in == NULL) {
        LOGE("qp_in is null");
        goto end;
    } else  {
        char buf[512] = {0};
        int i;
        for (i = 0; i < 20; i++) {
            char num[2];
            sprintf(num, "%c ", qp_in[i] + 48);
            memcpy(buf + 2 * i, num, 2);
        }
        jni_print("in:%s", buf);
    }
    ok=S.bfs(qp_in,500);
    if(!ok) {
        jni_print("此局500层内无解.");
        goto end;
    }
    if(ok==-1) {
        jni_print("此局无解.");
        goto end;
    }
    qp_list = (qp_list_t *)malloc(sizeof(qp_list_t));
    qp_record = (char **)malloc(S.ren * 20);
    qp_list->len = S.ren;
    qp_list->qp_list = qp_record;
    jni_print("%d层有解,遍历%d节点,哈希%d节点,队长%d,哈希冲突%d次,用时%.3fms",S.ren,S.js,S.js2,S.dn,S.Hx.cht,(clock()-t1) / 1000.0);
    for(i=0;i<S.ren;i++) {
        char * r = S.getre(i);
        *(qp_record + i) = r;
    }
    end:
       return qp_list;
}

/*
 * Class:     com_gwcd_sy_hrd_HrdLib
 * Method:    hrdPlay
 * Signature: ([C)[Lcom/gwcd/sy/hrd/Qipan;
 */
JNIEXPORT jobjectArray JNICALL Java_com_gwcd_sy_hrd_HrdLib_hrdPlay
        (JNIEnv * env, jclass cls, jcharArray charArray)
{
    char qp[20] = {0};
    jobjectArray qpArray;
    jchar * in_array;
    in_array = env->GetCharArrayElements(charArray, NULL);
    for (int i = 0; i < 20; i++) {
        qp[i] = (jchar)(*(in_array + i));
    }
    qp_list_t * qp_list = hrd_play_hash(qp);
    if (qp_list != NULL) {
        jclass qpClass = env->FindClass("com/gwcd/sy/hrd/Qipan");
        qpArray = env->NewObjectArray(qp_list->len, qpClass, NULL);
        jmethodID m_init_id = env->GetMethodID(qpClass,"<init>","()V");
        jfieldID qp_id = env->GetFieldID(qpClass, "qp", "[B");
        int i;
        char ** record = qp_list->qp_list;
        for (i = 0; i < qp_list->len; i++) {
            char * r = (char *)(*(record + i));
            jni_print("第%d步:",i);
            prt_android(r);
            jobject qp_obj = env->NewObject(qpClass, m_init_id);
            jbyteArray array = env->NewByteArray(20);
            env->SetByteArrayRegion(array, 0, 20, (jbyte *)r);
            env->SetObjectField(qp_obj, qp_id, array);
            env->SetObjectArrayElement(qpArray, i, qp_obj);
        }
        env->DeleteLocalRef(qpClass);
//        env->DeleteLocalRef(m_init_id);
//        env->DeleteLocalRef(qp_id);
        free(qp_list->qp_list);
        free(qp_list);
    }
    env->ReleaseCharArrayElements(charArray, in_array, 0);

    return qpArray;
}
