package com.ygq.ndk.day03

class NativeLib {

    companion object {
        // Used to load the 'nativelib' library on application startup.
        init {
            System.loadLibrary("nativelib")
        }

        external fun arraycopy(
            src: Any, srcPos: Int,
            dest: Any, destPos: Int,
            length: Int,
        )
    }
}