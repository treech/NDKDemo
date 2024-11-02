#include <jni.h>
#include <string>
#include "log.h"
#include "LinkedList.hpp"

extern "C"
JNIEXPORT void JNICALL
Java_com_ygq_ndk_day06_NativeLib_00024Companion_testNode(JNIEnv *env, jobject jobj) {
    LinkedList<int> linkedList;
    linkedList.push(0);
    linkedList.push(1);
    linkedList.push(2);

    for (int i = 0; i < linkedList.size(); ++i) {
        LOGI("index:%d,value:%d", i, linkedList.get(i));
    }
}