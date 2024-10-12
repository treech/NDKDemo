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

// 使用模型进行人脸检测
vector<Rect> detectFaces(Net &net, const Mat &image) {
    LOGI("detectFaces start...");
    Mat blob = blobFromImage(image, 1.0, Size(320, 320), Scalar(104.0, 177.0, 123.0), true, false);
    net.setInput(blob);
    Mat detections = net.forward();
    vector<Rect> faces;
    for (int i = 0; i < detections.size[2]; i++) {
        float confidence = detections.at<float>(i, 2);
        LOGD("confidence:%f", confidence);
        if (confidence > 0.9) {  // 置信度大于阈值
            int x1 = static_cast<int>(detections.at<float>(i, 3) * image.cols);
            int y1 = static_cast<int>(detections.at<float>(i, 4) * image.rows);
            int x2 = static_cast<int>(detections.at<float>(i, 5) * image.cols);
            int y2 = static_cast<int>(detections.at<float>(i, 6) * image.rows);
            faces.emplace_back(Rect(Point(x1, y1), Point(x2, y2)));
        }
    }
    return faces;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_ygq_ndk_day05_NativeLib_00024Companion_detectFaces(JNIEnv *env, jobject thiz, jobject src, jobject dest, jstring modelPath) {
    const char *modelPathCStr = env->GetStringUTFChars(modelPath, JNI_FALSE);
    LOGI("modelPath:%s", modelPathCStr);
    string modelPathStr(modelPathCStr);
    env->ReleaseStringUTFChars(modelPath, modelPathCStr);
    try {
        Net net = readNet(modelPathStr);
        if (net.empty()) {
            LOGE("Failed to load model from: %s", modelPathStr.c_str());
            return JNI_FALSE;
        }
        Mat image;
        bitmap2Mat(env, image, src);
        if (image.empty()) {
            LOGE("Failed to convert bitmap to Mat.");
            return JNI_FALSE;
        }
        // 将 RGBA 图像转换为 RGB 图像
        vector<Rect> faces;
        if (image.channels() == 4) {
            LOGI("RGBA to RGB");
            Mat rgb_image;
            cvtColor(image, rgb_image, COLOR_RGBA2RGB);
            faces = detectFaces(net, rgb_image);
            for (const auto &face: faces) {
                rectangle(rgb_image, face, Scalar(255, 0, 0), 8);
            }
            mat2bitmap(env, rgb_image, dest);
        } else {
            faces = detectFaces(net, image);
            for (const auto &face: faces) {
                rectangle(image, face, Scalar(255, 0, 0), 8);
            }
            mat2bitmap(env, image, dest);
        }
        LOGI("detect face size:%d", faces.size());
        return JNI_TRUE;
    } catch (const exception &e) {
        LOGE("Unexpected error: %s", e.what());
        return JNI_FALSE;
    } catch (...) {
        LOGE("Unknown error occurred.");
        return JNI_FALSE;
    }
}

CascadeClassifier cascadeClassifier;

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_ygq_ndk_day05_NativeLib_00024Companion_detectFacesV2(JNIEnv *env, jobject thiz, jobject src, jobject dest, jstring modelPath) {
    const char *modelPathCStr = env->GetStringUTFChars(modelPath, JNI_FALSE);
    LOGI("modelPath:%s", modelPathCStr);
    try {
        cascadeClassifier.load(modelPathCStr);
        LOGE("load cascadeClassifier success");
        env->ReleaseStringUTFChars(modelPath, modelPathCStr);

        Mat mat;
        bitmap2Mat(env, mat, src);

        // 处理灰度 opencv 处理灰度图, 提高效率，一般所有的操作都会对其进行灰度处理
        Mat gray_mat;
        cvtColor(mat, gray_mat, COLOR_BGRA2GRAY);

        // 再次处理 直方均衡补偿
        Mat equalize_mat;
        equalizeHist(gray_mat, equalize_mat);

        // 识别人脸，也可以直接用 彩色图去做,识别人脸要加载人脸分类器文件
        std::vector<Rect> faces;
        cascadeClassifier.detectMultiScale(equalize_mat, faces, 1.1, 5);
        LOGI("detect face size:%d", faces.size());
        for (Rect &faceRect: faces) {
            // 在人脸部分画个图
            rectangle(mat, faceRect, Scalar(255, 0, 0), 8);
        }
        // 把 mat 我们又放到 bitmap 里面
        mat2bitmap(env, mat, dest);
        return JNI_TRUE;
    } catch (const exception &e) {
        LOGE("Unexpected error: %s", e.what());
        return JNI_FALSE;
    } catch (...) {
        LOGE("Unknown error occurred.");
        return JNI_FALSE;
    }
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