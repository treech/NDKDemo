package com.ygq.ndk.signature

import android.content.Context

class NativeLib {

    external fun signatureParams(param: String): String

    external fun signatureVerify(context: Context)

    companion object {
        // Used to load the 'nativelib' library on application startup.
        init {
            System.loadLibrary("nativelib")
        }
    }
}