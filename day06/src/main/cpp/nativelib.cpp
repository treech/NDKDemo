#include <jni.h>
#include <string>
#include "log.h"
#include "LinkedList.hpp"

extern "C"
JNIEXPORT void JNICALL
Java_com_ygq_ndk_day06_NativeLib_00024Companion_testNode(JNIEnv *env, jobject jobj) {
    LinkedList<int> linkedList;

    time_t start = clock();
    for (int i = 0; i < 100000; ++i) {
        linkedList.push(i);
    }
    time_t end = clock();
    //list size:100000,consume time : 15 原来的push方式耗时
    //list size:100000,consume time : 0 增加尾指针的方式耗时
    LOGI("list size:%d,consume time : %d", linkedList.size(), (end - start) / CLOCKS_PER_SEC);
}