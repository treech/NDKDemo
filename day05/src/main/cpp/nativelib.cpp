#include <jni.h>
#include <string>
#include <android/log.h>
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>

using namespace cv;

//定义TAG之后，我们可以在LogCat通过TAG过滤出NDK打印的日志
#define TAG "JNI_YGQ_DAY05"

// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

//https://github.com/opencv/opencv/blob/4.x/modules/java/generator/src/cpp/utils.cpp

void bitmap2Mat(JNIEnv *env, Mat &mat, jobject bitmap) {
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);
    void *pixels = 0;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    // CV_8UC4 8位无符号整数的4通道矩阵,对应BGRA格式
    mat.create(info.height, info.width, CV_8UC4);

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGD("nBitmapToMat: RGBA_8888 -> CV_8UC4");
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        tmp.copyTo(mat);
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        LOGD("nBitmapToMat: RGB_565 -> CV_8UC4");
        //8 位无符号整数的 2 通道矩阵
        Mat tmp(info.height, info.width, CV_8UC2, pixels);
        cvtColor(tmp, mat, COLOR_BGR5652RGBA);
    } else {
        LOGE("nBitmapToMat: unSupport format");
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}

void mat2bitmap(JNIEnv *env, Mat &src, jobject bitmap) {
    // 1. 获取图片的宽高，以及格式信息
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);
    void *pixels;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        if (src.type() == CV_8UC1) {
            LOGD("nMatToBitmap: CV_8UC1 -> RGBA_8888");
            cvtColor(src, tmp, COLOR_GRAY2RGBA);
        } else if (src.type() == CV_8UC3) {
            LOGD("nMatToBitmap: CV_8UC3 -> RGBA_8888");
            cvtColor(src, tmp, COLOR_RGB2RGBA);
        } else if (src.type() == CV_8UC4) {
            LOGD("nMatToBitmap: CV_8UC4 -> RGBA_8888");
            src.copyTo(tmp);
        }
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        Mat tmp(info.height, info.width, CV_8UC2, pixels);
        if (src.type() == CV_8UC1) {
            LOGD("nMatToBitmap: CV_8UC1 -> RGB_565");
            cvtColor(src, tmp, COLOR_GRAY2BGR565);
        } else if (src.type() == CV_8UC3) {
            LOGD("nMatToBitmap: CV_8UC3 -> RGB_565");
            cvtColor(src, tmp, COLOR_RGB2BGR565);
        } else if (src.type() == CV_8UC4) {
            LOGD("nMatToBitmap: CV_8UC4 -> RGB_565");
            cvtColor(src, tmp, COLOR_RGBA2BGR565);
        }
    } else {
        LOGE("nMatToBitmap: unSupport format");
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_ygq_ndk_day05_NativeLib_00024Companion_grayTransform(JNIEnv *env, jobject thiz, jobject src, jobject dest) {
    Mat mat;
    bitmap2Mat(env, mat, src);
    Mat mat_gay;
    cvtColor(mat, mat_gay, COLOR_BGRA2GRAY);
    mat2bitmap(env, mat_gay, dest);
    return JNI_TRUE;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ygq_ndk_day05_NativeLib_00024Companion_faceDetect(JNIEnv *env, jobject thiz, jobject bitmap) {

}