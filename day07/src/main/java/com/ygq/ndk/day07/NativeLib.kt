package com.ygq.ndk.day07

import android.graphics.Bitmap

class NativeLib {

    companion object {
        // Used to load the 'nativelib' library on application startup.
        init {
            System.loadLibrary("nativelib")
        }
    }

    external fun gray(src: Bitmap)
}