package com.ygq.ndk.day07.ui

import android.graphics.BitmapFactory
import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.ygq.ndk.day07.BitmapUtil
import com.ygq.ndk.day07.NativeLib
import com.ygq.ndk.day07.R
import com.ygq.ndk.day07.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        val srcBitmap = BitmapFactory.decodeResource(resources, R.drawable.test)
        binding.srcImg.setImageBitmap(srcBitmap)
        val dstBitmap = BitmapUtil.gray(srcBitmap)
        binding.dstImg.setImageBitmap(dstBitmap)
    }

    companion object {
        private const val TAG = "MainActivity"
    }
}