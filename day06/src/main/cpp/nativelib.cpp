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

    linkedList.insert(2, 2);

    for (int i = 0; i < linkedList.size(); ++i) {
        LOGI("index:%d,value:%d", i, linkedList.get(i));
    }

    LOGI("---------");

    linkedList.remove(0);//删除头节点
    linkedList.remove(1);//删除中间节点
    linkedList.remove(linkedList.size() - 1);//删除尾节点
    for (int i = 0; i < linkedList.size(); ++i) {
        LOGI("index:%d,value:%d", i, linkedList.get(i));
    }
}

/**
 * 冒泡排序
 * 外层循环控制排序的轮次，每轮排序会把最大值“冒泡”到数组的末尾。
 * 内层循环用于相邻元素的比较和交换。如果前一个元素大于后一个元素，就交换它们的位置。
 * 每次内层循环结束后，当前最大的元素就会被“冒泡”到数组的最后。
 */
void bubbleSort(int arr[], int len) {
    // 外层循环控制排序的轮次
    for (int i = 0; i < len - 1; ++i) {
        // 内层循环进行相邻元素的比较和交换
        for (int j = 0; j < len - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ygq_ndk_day06_NativeLib_00024Companion_bubbleSort(JNIEnv *env, jobject thiz) {
    int arr[] = {-1, -2, -8, -18,-4};
    bubbleSort(arr,  sizeof(arr) / sizeof(arr[0]));

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) {
        LOGI("index:%d,v:%d", i, arr[i]);
    }
}