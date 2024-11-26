package com.ygq.ndk.day06.ui

import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.ygq.ndk.day06.NativeLib
import com.ygq.ndk.day06.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
    }

    fun init(view: View) {
        NativeLib.testNode()
    }


    fun bubbleSort(view: View) {
        NativeLib.bubbleSort()
    }

    companion object {
        private const val TAG = "MainActivity"
    }

}