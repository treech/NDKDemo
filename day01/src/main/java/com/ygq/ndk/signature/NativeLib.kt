package com.ygq.ndk.signature

class NativeLib {

    external fun signatureParams(param: String): String

    companion object {
        // Used to load the 'nativelib' library on application startup.
        init {
            System.loadLibrary("nativelib")
        }
    }
}