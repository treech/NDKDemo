#include <jni.h>
#include <string>
#include <android/log.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <android/bitmap.h>

using namespace cv;
using namespace std;
using namespace dnn;

//定义TAG之后，我们可以在LogCat通过TAG过滤出NDK打印的日志
#define TAG "JNI_YGQ_DAY08"

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
JNIEXPORT jobject JNICALL
Java_com_ygq_ndk_day08_NativeLib_00024Companion_againstWorld(JNIEnv *env, jobject jcls, jobject bitmap) {
    Mat src;
    bitmap2Mat(env, src, bitmap);

    Mat dest(src.size(), src.type());

    int w = src.cols;
    int h = src.rows;
    int middle_h = h >> 1;
    int one_fourth_h = middle_h >> 1;
    for (int row = 0; row < middle_h; ++row) {
        for (int col = 0; col < w; ++col) {
            dest.at<Vec4b>(row, col) = src.at<Vec4b>(row + one_fourth_h, col);
        }
    }

    for (int row = 0; row < middle_h; ++row) {
        for (int col = 0; col < w; ++col) {
            dest.at<Vec4b>(row + middle_h, col) = src.at<Vec4b>(h - row - one_fourth_h, col);
        }
    }
    mat2bitmap(env, dest, bitmap);
    return bitmap;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_ygq_ndk_day08_NativeLib_00024Companion_embossingEffects(JNIEnv *env, jobject jcls, jobject bitmap) {
    Mat src;
    bitmap2Mat(env, src, bitmap);

    Mat dest(src.size(), src.type());

    int w = src.cols;
    int h = src.rows;
    for (int row = 0; row < h - 1; ++row) {
        for (int col = 0; col < w - 1; ++col) {
            Vec4b pixel_p = src.at<Vec4b>(row, col);
            Vec4b pixel_n = src.at<Vec4b>(row + 1, col + 1);
            // BGRA
            // 处理BGR通道
            dest.at<Vec4b>(row, col)[0] = saturate_cast<uchar>(pixel_p[0] - pixel_n[0] + 128);
            dest.at<Vec4b>(row, col)[1] = saturate_cast<uchar>(pixel_p[1] - pixel_n[1] + 128);
            dest.at<Vec4b>(row, col)[2] = saturate_cast<uchar>(pixel_p[2] - pixel_n[2] + 128);
            // 保持Alpha通道不变
            dest.at<Vec4b>(row, col)[3] = src.at<Vec4b>(row, col)[3];
        }
    }
    mat2bitmap(env, dest, bitmap);
    return bitmap;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_ygq_ndk_day08_NativeLib_00024Companion_mosaicEffects(JNIEnv *env, jobject jcls, jobject bitmap) {
    Mat src;
    bitmap2Mat(env, src, bitmap);

    int w = src.cols;
    int h = src.rows;

    // 马赛克块大小
    int size = 50;

    for (int row = 0; row < h; row += size) {
        for (int col = 0; col < w; col += size) {
            // 确保不越界
            int end_row = min(row + size, h);
            int end_col = min(col + size, w);

            // 10*10 获取当前块的左上角像素值
            Vec4b pixel = src.at<Vec4b>(row, col);
            for (int r = row; r < end_row; ++r) {
                for (int c = col; c < end_col; ++c) {
                    src.at<Vec4b>(r, c) = pixel;
                }
            }
        }
    }
    mat2bitmap(env, src, bitmap);
    return bitmap;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_ygq_ndk_day08_NativeLib_00024Companion_groundGlassEffects(JNIEnv *env, jobject thiz, jobject bitmap) {
    Mat src;
    bitmap2Mat(env, src, bitmap);

    int w = src.cols;
    int h = src.rows;

    // 毛玻璃块大小 取区域内的随机像素
    int size = 50;

    RNG rng(time(NULL));
    for (int row = 0; row < h - size; ++row) {
        for (int col = 0; col < w - size; ++col) {
            int random = rng.uniform(0, size);
            // size*size 分配随机像素值
            src.at<Vec4b>(row, col) = src.at<Vec4b>(row + random, col + random);
        }
    }
    mat2bitmap(env, src, bitmap);
    return bitmap;
}