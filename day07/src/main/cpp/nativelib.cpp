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

    for (int i = 0; i < bitmap_info.width * bitmap_info.height; i++) {
        uint32_t *pixel_p = reinterpret_cast<uint32_t *>(addrPtr) + i;
        uint32_t pixel = *pixel_p;
        // 获取ARGB
        int a = pixel >> 24 & 0xff;
        int r = pixel >> 16 & 0xff;
        int g = pixel >> 8 & 0xff;
        int b = pixel & 0xff;

        // 灰度图 0.11f * r + 0.59f * g + 0.30f * b
        int gray = (int) (0.11f * r + 0.59f * g + 0.30f * b);

        // 转回像素数据
        *pixel_p = a << 24 | gray << 16 | gray << 8 | gray;
    }

    AndroidBitmap_unlockPixels(env, src_bitmap);
}