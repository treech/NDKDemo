package com.ygq.ndk.day08.ui

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.ygq.ndk.day08.NativeLib
import com.ygq.ndk.day08.R
import com.ygq.ndk.day08.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private lateinit var src: Bitmap

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        src = BitmapFactory.decodeResource(resources, R.drawable.test)
        binding.iv.setImageBitmap(src)

        val dest = src.copy(src.config, true)
        val againstBitmap = NativeLib.againstWorld(dest)
        binding.ivDest.setImageBitmap(againstBitmap)
    }
}