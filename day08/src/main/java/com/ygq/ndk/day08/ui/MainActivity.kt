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

        val dest2 = src.copy(src.config, true)
        val againstBitmap2 = NativeLib.embossingEffects(dest2)
        binding.ivDest2.setImageBitmap(againstBitmap2)

        val dest3 = src.copy(src.config, true)
        val againstBitmap3 = NativeLib.mosaicEffects(dest3)
        binding.ivDest3.setImageBitmap(againstBitmap3)

        val dest4 = src.copy(src.config, true)
        val againstBitmap4 = NativeLib.groundGlassEffects(dest4)
        binding.ivDest4.setImageBitmap(againstBitmap4)
    }
}