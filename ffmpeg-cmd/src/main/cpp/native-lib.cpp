#include <jni.h>
#include <string>
#include <android/log.h>

using namespace std;

//定义TAG之后，我们可以在LogCat通过TAG过滤出NDK打印的日志
#define TAG "JNI_YGQ_DAY02"

// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

// 声明方法(实现的方式就是基于命令，windows )
// argc 命令的个数
// char **argv 二维数组
int ffmpegmain(int argc, char **argv,void(call_back)(int,int));

// 回调函数
static jobject call_back_jobj;
static JNIEnv *mEnv;
void call_back(int current,int total){
    // LOGE("压缩进度：%d/%d",current,total);
    // 把进度回调出去 对象是 jobject callback
    if(call_back_jobj != NULL && mEnv != NULL){
        // C进阶再去讲 指针，什么是内存，内存模型 , 获取 j_mid 是会被多次执行
        jclass j_clazz = mEnv->GetObjectClass(call_back_jobj);
        // javap 命令也能打印
        jmethodID j_mid = mEnv->GetMethodID(j_clazz,"onCompress","(II)V");
        mEnv->CallVoidMethod(call_back_jobj,j_mid,current,total);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ygq_ndk_ffmpeg_cmd_NativeLib_compressVideo(JNIEnv *env, jobject thiz, jobjectArray compress_command, jobject callback) {
    call_back_jobj = env->NewGlobalRef(callback);
    mEnv = env;
    // ffmpeg 处理视频的压缩，自己处理怎么处理，for循环每一帧 ，对每一帧进行处理（算法）
    // armeabi 这个里面的so都是用来处理音视频的, include 都是头文件
    // 还有几个没有被打包编译成 so ,因为这些不算是音视频的处理代码，只是我们现在支持命令（封装）
    // 1. 获取命令的个数
    int argc = env->GetArrayLength(compress_command);
    // 2. 给 char **argv 填充数据
    char **argv = (char **) malloc(sizeof(char*) * argc);
    for (int i = 0; i < argc; ++i) {
        jstring j_param = (jstring) env->GetObjectArrayElement(compress_command, i);
        argv[i] = (char *) env->GetStringUTFChars(j_param, NULL);
        LOGE("参数：%s",argv[i]);
    }
    // 3. 调用命令函数去压缩，回调处理
//    ffmpegmain(argc,argv,call_back);

    // 4. 释放内存
    for (int i = 0; i < argc; ++i) {
        free(argv[i]);
    }
    free(argv);
    env->DeleteGlobalRef(call_back_jobj);
}