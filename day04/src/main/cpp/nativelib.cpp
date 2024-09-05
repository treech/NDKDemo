#include <jni.h>
#include <string>
#include <android/log.h>

using namespace std;

//定义TAG之后，我们可以在LogCat通过TAG过滤出NDK打印的日志
#define TAG "JNI_YGQ_DAY04"

// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

class Exception {
public:
    Exception(int code, const char *str) {
        this->code = code;
        msg = str ? new char[strlen(str) + 1] : nullptr;
        if (msg) {
            strcpy(this->msg, str);
        }
    }

    ~Exception() {
        delete[] msg;
    }

    int getCode() const {
        return code;
    }

    char *getMsg() const {
        return msg;
    }

private:
    int code;
    char *msg;
};

void c_method() throw(Exception) {
    throw Exception(-1, "oops error occur");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ygq_ndk_day04_NativeLib_00024Companion_arraycopy(JNIEnv *env, jobject thiz, jobject src, jint src_pos, jobject dest, jint dest_pos, jint length) {

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
Java_com_ygq_ndk_day04_NativeLib_00024Companion_makeCrash(JNIEnv *env, jobject thiz) {
    try {
        c_method();
    } catch (const Exception &e) {
        LOGE("find exception");
        // 动态计算消息的长度：假设错误码是整数，需要的字符数可以用 snprintf 计算,+1 是为了放终止符 '\0'
        int bufferSize = snprintf(nullptr, 0, "Error Code: %d, Message: %s", e.getCode(), e.getMsg() ? e.getMsg() : "No message") + 1;

        // 动态分配缓冲区
        char *fullMessage = (char *) malloc(bufferSize);
        if (fullMessage == NULL) {
            LOGE("malloc fullMessage error");
            return;
        }
        if (fullMessage != nullptr) {
            snprintf(fullMessage, bufferSize, "Error Code: %d, Message: %s", e.getCode(), e.getMsg() ? e.getMsg() : "No message");

            // 获取要抛出的异常类
            jclass exceptionClass = env->FindClass("java/lang/Exception");
            if (exceptionClass != nullptr) {
                // 抛出带有自定义消息的 Java 异常
                env->ThrowNew(exceptionClass, fullMessage);
            }

            // 释放动态分配的内存
            free(fullMessage);
        }
    }
}