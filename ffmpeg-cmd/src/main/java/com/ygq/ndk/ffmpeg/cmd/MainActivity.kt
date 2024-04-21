package com.ygq.ndk.ffmpeg.cmd

import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import androidx.appcompat.app.AppCompatActivity
import com.ygq.ndk.ffmpeg.cmd.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val mainBinding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(mainBinding.root)
        mainBinding.tv.movementMethod = ScrollingMovementMethod.getInstance()

        val nativeLib = NativeLib()
        mainBinding.tv.text = nativeLib.getFFmpegVersion()
    }
}