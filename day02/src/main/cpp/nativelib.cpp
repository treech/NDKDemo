#include <jni.h>
#include <string>
#include "md5.h"
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

int is_verify = 0;
char *APP_SIGNATURE = "308202e4308201cc020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b30090603550406130255533020170d3231303532353036353032375a180f32303531303531383036353032375a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330820122300d06092a864886f70d01010105000382010f003082010a02820101008d71051c6ccaaa6e5ea21f919d8623772c03c377d84f3f99888642e18332cc5d3b3eb98a76ada68de5b0095def154ec7b5ad08c27fe69a3527a27cf6de3600b35680bb4a18ec9a27c72bbc1efa3676d16d65d9e0b71ccd0aa36efdee21fbb74cfe02546dd16e999d64b68018cf2b770c8bb3a7e68e56c0f73a46fca7d93a81110e149aa8358410d481c98de471d260532d979b53af4676c91a2ddf852af529499b2bf93b502e3bab5ba9bda760517fa9d426b5b70951a92c681871bc4782716705ca3450b8d17dd04d43eb8aaa5174490229503a57ac26b313f3849dc8efa9ca6935d002b9f7a4960387d1e8eb2fd4a305ebef55f77e3dcdad0c030c8b530d510203010001300d06092a864886f70d01010505000382010100636a55a1e25a25ff1e911d38d4c8b403a6b8b0076c7db14e1054f96d2c8f5be6cd77c9667f10a447ce562557dfed415522b8f423da7424270a8b726825cb1815962affc2f6b210654620711db7d16512164d6d1753d2c0eed41ae91d424876df5c5917832d9f237423ffd7145d2d770917d486ee46a1177e62a886d8490d475db1e37f055663275a70d3c7e646a437556fe8dd200782a8019aa77638c78955f13e2c4deaabf78730830d1924a5207ddcd82f2be3365d089e1f8574ab4d5330d24768e7f22c9015734e7e5b94f0fc022f1bae318fbb68b9868e626bb9ecec20224542dffdf18ce6e4e90dff2b5ce20cbfc6d11f39a027c49244269c4dc47cdfe2";
char *MD5_KEY = "YGQ";
const char *PACKAGE_NAME = "com.ygq.ndk.day02";

extern "C"
JNIEXPORT jstring JNICALL
Java_com_ygq_ndk_signature_NativeLib_signatureParams(JNIEnv *env, jobject type, jstring params_) {
    if (is_verify == 0) {
        return env->NewStringUTF("error signature");
    }

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

/**
 Native层校验签名并不安全，使用Xposed去调试,可以改变对应方法的地址值
 1、如果发现Xposed调试，退出应用
 2、如果使用Xposed去调试tracep_id会发生改变，如发现改变退出应用
PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
Signature[] signatures = packageInfo.signatures;
return signatures[0].toCharsString();
*/
extern "C"
JNIEXPORT void JNICALL
Java_com_ygq_ndk_signature_NativeLib_signatureVerify(JNIEnv *env, jobject type, jobject context) {
    //查看android sdk类方法签名
    //javap -classpath D:\programs\Android\Sdk\platforms\android-34\android.jar -s android.content.Context
    //getPackageName
    jclass j_class = env->GetObjectClass(context);
    jmethodID j_mid = env->GetMethodID(j_class, "getPackageName", "()Ljava/lang/String;");
    jstring j_package_name = (jstring) env->CallObjectMethod(context, j_mid);
    const char *package_name_str = env->GetStringUTFChars(j_package_name, NULL);
    if (strcmp(package_name_str, PACKAGE_NAME) != 0) {
        LOGE("package name verify fail");
        return;
    }

    //getPackageManager
    j_mid = env->GetMethodID(j_class, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject pm = env->CallObjectMethod(context, j_mid);
    j_class = env->GetObjectClass(pm);

    //getPackageInfo PackageManager.GET_SIGNATURES 0x00000040 十进制是64
    j_mid = env->GetMethodID(j_class, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject package_info = env->CallObjectMethod(pm, j_mid, j_package_name, 0x00000040);

    env->ReleaseStringUTFChars(j_package_name,package_name_str);
    env->DeleteLocalRef(j_package_name);
    env->DeleteLocalRef(pm);

    //获取变量signatures
    j_class = env->GetObjectClass(package_info);
    jfieldID j_fid = env->GetFieldID(j_class, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatures_array = (jobjectArray) env->GetObjectField(package_info, j_fid);
    env->DeleteLocalRef(package_info);

    //获取signatures[0]
    jobject signature_obj = env->GetObjectArrayElement(signatures_array, 0);
    env->DeleteLocalRef(signatures_array);

    //调用Signature的toCharsString
    j_class = env->GetObjectClass(signature_obj);
    j_mid = env->GetMethodID(j_class, "toCharsString", "()Ljava/lang/String;");
    env->DeleteLocalRef(j_class);

    jstring signature_jstr = (jstring) env->CallObjectMethod(signature_obj, j_mid);

    env->DeleteLocalRef(signature_obj);
    const char *signature_str = (char *) env->GetStringUTFChars(signature_jstr, NULL);
    if (strcmp(signature_str, APP_SIGNATURE) != 0) {
        LOGE("app signature fail");
        return;
    }
    env->ReleaseStringUTFChars(signature_jstr,signature_str);
    env->DeleteLocalRef(signature_jstr);
    is_verify = 1;
    LOGI("app signature success");
}