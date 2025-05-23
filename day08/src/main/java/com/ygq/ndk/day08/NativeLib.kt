package com.ygq.ndk.day08

import android.graphics.Bitmap

class NativeLib {

    companion object {
        // Used to load the 'nativelib' library on application startup.
        init {
            System.loadLibrary("opencv")
        }

        external fun againstWorld(src: Bitmap): Bitmap

        external fun embossingEffects(src: Bitmap): Bitmap

        external fun mosaicEffects(src: Bitmap): Bitmap

        external fun groundGlassEffects(src: Bitmap): Bitmap
    }
}