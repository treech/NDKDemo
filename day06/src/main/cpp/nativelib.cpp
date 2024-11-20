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
    linkedList.push(5);
    linkedList.push(6);
    linkedList.push(7);

    linkedList.insert(2,2);

    for (int i = 0; i < linkedList.size(); ++i) {
        LOGI("index:%d,value:%d", i, linkedList.get(i));
    }

    LOGI("---------");

    linkedList.remove(0);//删除头节点
    linkedList.remove(1);//删除中间节点
    linkedList.remove(linkedList.size()-1);//删除尾节点
    for (int i = 0; i < linkedList.size(); ++i) {
        LOGI("index:%d,value:%d", i, linkedList.get(i));
    }
}