package com.ygq.ndk.day06

class NativeLib {

    companion object {
        // Used to load the 'nativelib' library on application startup.
        init {
            System.loadLibrary("nativelib")
        }

        external fun testNode()

        external fun bubbleSort()
    }
}