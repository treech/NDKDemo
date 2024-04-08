#include <jni.h>
#include <string>
#include "md5.h"
#include <android/log.h>

using namespace std;

//定义TAG之后，我们可以在LogCat通过TAG过滤出NDK打印的日志
#define TAG "JNI_YGQ_DAY01"

// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

char *MD5_KEY = "YGQ";

extern "C"
JNIEXPORT jstring JNICALL
Java_com_ygq_ndk_signature_NativeLib_signatureParams(JNIEnv *env, jobject type, jstring params_) {
    const char *params = env->GetStringUTFChars(params_, NULL);
    //添加自定义的字段
    string signature_str(params);
    signature_str.insert(0, MD5_KEY);
    signature_str = signature_str.substr(0, signature_str.length() - 2);
    LOGI("signature_str:%s", signature_str.c_str());

    //md5加密
    MD5_CTX md5_ctx;
    MD5Init(&md5_ctx);
    MD5Update(&md5_ctx, (unsigned char *) signature_str.c_str(), signature_str.length());
    unsigned char dest[16] = {0};
    MD5Final(dest, &md5_ctx);
    env->ReleaseStringUTFChars(params_, params);

    char md5_str[33] = {0};
    for (int i = 0; i < 16; i++) {
        // 最终生成 32 位 ，不足前面补一位 0
        sprintf(md5_str, "%s%02x", md5_str, dest[i]);
    }
    return env->NewStringUTF(md5_str);
}