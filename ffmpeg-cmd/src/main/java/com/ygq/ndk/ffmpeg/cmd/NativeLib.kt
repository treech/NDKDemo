package com.ygq.ndk.ffmpeg.cmd

class NativeLib {
    // 加载 so 库文件
    companion object {
        init {
            System.loadLibrary("ffmpeg-lib")
        }
        // 不需要全部 load 相当于 android 调用其他方法类型，不需要全部 load
    }

    // native ffmpeg 压缩视频
    external fun compressVideo(compressCommand: Array<String>, callback: CompressCallback)

    external fun getFFmpegVersion():String

    interface CompressCallback {
        fun onCompress(current: Int, total: Int)
    }
}