package com.ygq.ndk.day05.ui

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.ygq.ndk.day05.NativeLib
import com.ygq.ndk.day05.R
import com.ygq.ndk.day05.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private lateinit var bitmap: Bitmap

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        bitmap = BitmapFactory.decodeResource(resources, R.drawable.test)
        binding.iv.setImageBitmap(bitmap)
    }

    fun faceDetect(view: View) {
//        NativeLib.faceDetect(bitmap)
    }

    fun grayTransform(view: View) {
        Thread {
            val dest = Bitmap.createBitmap(bitmap.width, bitmap.height, Bitmap.Config.ARGB_8888)
            val result = NativeLib.grayTransform(bitmap, dest)
            if (result) {
                runOnUiThread {
                    binding.iv.setImageBitmap(dest)
                }
            }
        }.start()
    }

    companion object {
        private const val TAG = "MainActivity"
    }
}