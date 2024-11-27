#include <jni.h>
#include <string>
#include <android/log.h>

using namespace std;

//定义TAG之后，我们可以在LogCat通过TAG过滤出NDK打印的日志
#define TAG "JNI_YGQ_DAY03"

// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

#define MAX(a, b) a>b?a:b

#define PRINT_MSG(msg, repeat_count) \
            for(int i=0;i<repeat_count;i++){ \
                    LOGE("i:%d,%s",i,msg);\
            }

#define PRINT(msg) PRINT_MSG(msg,1)

#define LOG(level,...) \
    LOGE("[" level "] " __VA_ARGS__);

#define LOG_ERROR(...) LOG("ERROR",__VA_ARGS__)

extern "C"
JNIEXPORT void JNICALL
Java_com_ygq_ndk_day03_NativeLib_00024Companion_arraycopy(JNIEnv *env, jobject thiz, jobject src, jint src_pos, jobject dest, jint dest_pos, jint length) {

    auto srcList = reinterpret_cast<jobjectArray>(src);
    auto destList = reinterpret_cast<jobjectArray>(dest);

    if (srcList == NULL || destList == NULL) {
        LOGE("srcList or destList cast error");
        return;
    }
    jsize container_size = env->GetArrayLength(destList);
    if (length + dest_pos > container_size) {
        LOGE("dest_pos:%d length:%d sum is large than container_size:%d", dest_pos, length, container_size);
        return;
    }

    for (int i = src_pos; i < src_pos + length; ++i) {
        jobject obj = env->GetObjectArrayElement(srcList, i);
        env->SetObjectArrayElement(destList, i + dest_pos, obj);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ygq_ndk_day03_NativeLib_00024Companion_defineFunTest(JNIEnv *env, jobject thiz) {
    int x = 10, y = 20;
    LOGE("max value:%d", MAX(x, y));

    PRINT("Hello C++")
    PRINT_MSG("Hello Android", 3)

    LOG_ERROR("System is up and running.\n");
    LOG_ERROR("This is a warning: %s\n", "Disk space low");
    LOG_ERROR("Error occurred: %d\n", -1);
}