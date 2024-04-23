package com.ygq.ndk.ffmpeg.cmd

import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.ygq.ndk.ffmpeg.cmd.databinding.ActivityMainBinding
import java.io.File
import java.io.FileOutputStream

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val mainBinding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(mainBinding.root)
        mainBinding.tv.movementMethod = ScrollingMovementMethod.getInstance()

        val nativeLib = NativeLib()
        mainBinding.tv.text = nativeLib.getFFmpegVersion()

        Thread {
            val inputStream = assets.open("in.mp4")
            val inputFile = File(cacheDir, "temp_in.mp4")

            try {
                val outputStream = FileOutputStream(inputFile)
                val bytes = ByteArray(1024)
                var length: Int
                while ((inputStream.read(bytes).also { length = it }) != -1) {
                    outputStream.write(bytes, 0, length)
                }
                outputStream.close()
                inputStream.close()
            } catch (e: Exception) {
                e.printStackTrace()
            }

            val outputFile = File(cacheDir, "temp_out.mp4")
            val command = arrayOf("ffmpeg", "-i", inputFile.absolutePath, "-b:v", "1024k", outputFile.absolutePath)

            nativeLib.compressVideo(command, object : NativeLib.CompressCallback {
                override fun onCompress(current: Int, total: Int) {
                    Log.e("ygq", "current:$current,total:$total")
                }
            })
        }.start()

    }
}