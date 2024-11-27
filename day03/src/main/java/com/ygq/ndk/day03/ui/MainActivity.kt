package com.ygq.ndk.day03.ui

import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.ygq.ndk.day03.NativeLib
import com.ygq.ndk.day03.Student
import com.ygq.ndk.day03.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val srcList = Array(10) { i -> Student("name=$i", i) }

        val destList = Array(30) { Student() }
        //System.arraycopy(srcList, 0, destList, 20, srcList.size)
        NativeLib.arraycopy(srcList, 0, destList, 21, srcList.size)

        for (i in destList.indices) {
            Log.i(TAG, "index:$i,data:${destList[i]}")
        }
    }

    fun defineFunTest(view: View) {
        NativeLib.defineFunTest()
    }

    companion object {
        private const val TAG = "MainActivity"
    }
}