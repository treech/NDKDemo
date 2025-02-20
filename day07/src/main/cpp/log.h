//
// Created by ygq on 2024/11/2.
//
#include <android/log.h>

#ifndef MYNDKDEMO_LOG_H
#define MYNDKDEMO_LOG_H

//定义TAG之后，我们可以在LogCat通过TAG过滤出NDK打印的日志
#define TAG "JNI_YGQ_day07"

// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

#endif //MYNDKDEMO_LOG_H
