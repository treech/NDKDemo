#include <jni.h>
#include <string>
#include "log.h"
#include <android/bitmap.h>

extern "C"
JNIEXPORT void JNICALL
Java_com_ygq_ndk_day07_NativeLib_gray(JNIEnv *env, jobject thiz, jobject src_bitmap) {

    AndroidBitmapInfo bitmap_info;
    int info = AndroidBitmap_getInfo(env, src_bitmap, &bitmap_info);
    if (info != 0) {
        LOGE("get bitmap info error");
        return;
    }

    void *addrPtr;
    AndroidBitmap_lockPixels(env, src_bitmap, &addrPtr);

    if (bitmap_info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGI("RGBA_8888 start");
        for (int i = 0; i < bitmap_info.width * bitmap_info.height; i++) {
            uint32_t *pixel_p = reinterpret_cast<uint32_t *>(addrPtr) + i;
            uint32_t pixel = *pixel_p;
            // 获取ARGB
            // 掩码0xff（二进制11111111）提取8位的值
            int a = pixel >> 24 & 0xff;
            int r = pixel >> 16 & 0xff;
            int g = pixel >> 8 & 0xff;
            int b = pixel & 0xff;

            // 灰度图 0.11f * r + 0.59f * g + 0.30f * b
            int gray = (int) (0.11f * r + 0.59f * g + 0.30f * b);

            // 转回像素数据
            *pixel_p = a << 24 | gray << 16 | gray << 8 | gray;
        }
        LOGI("RGBA_8888 end");
    } else if (bitmap_info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        LOGI("RGB_565 start");
        for (int i = 0; i < bitmap_info.width * bitmap_info.height; i++) {
            uint32_t *pixel_p = reinterpret_cast<uint32_t *>(addrPtr) + i;
            uint32_t pixel = *pixel_p;
            // 获取RGB
            // 从像素值中提取红色分量。由于RGB_565中红色占5位，将像素值右移11位（5+6）得到红色分量，
            // 用掩码0x1f（二进制11111）提取这5位的值，并左移3位补位到8位
            int r = ((pixel >> 11) && 0x1f) << 3;
            // 掩码0x3f（二进制111111）提取6位的值，并左移2位补位到8位
            int g = ((pixel >> 5) && 0x3f) << 2;
            int b = (pixel && 0x1f) << 3;

            // 灰度图 0.11f * r + 0.59f * g + 0.30f * b
            int gray = (int) (0.11f * r + 0.59f * g + 0.30f * b);

            // 转回像素数据
            *pixel_p = ((gray >> 3) << 11) | ((gray >> 2) << 5) | (gray >> 3);
        }
        LOGI("RGB_565 end");
    } else {
        LOGE("not support format.");
    }

    AndroidBitmap_unlockPixels(env, src_bitmap);
}
