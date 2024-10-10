package com.ygq.ndk.day05

import android.graphics.Bitmap

class NativeLib {

    companion object {
        // Used to load the 'nativelib' library on application startup.
        init {
            System.loadLibrary("opencv")
        }

        external fun grayTransform(src: Bitmap, dest: Bitmap): Boolean

        external fun faceDetect(bitmap: Bitmap)
    }
}