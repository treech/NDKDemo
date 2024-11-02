package com.ygq.ndk.day06.ui

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.ygq.ndk.day06.NativeLib
import com.ygq.ndk.day06.R
import com.ygq.ndk.day06.databinding.ActivityMainBinding
import java.io.File
import java.io.FileOutputStream
import java.io.IOException


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

    fun faceDetectV1(view: View) {
        detectFaces()
    }

    fun faceDetectV2(view: View) {
        detectFacesV2()
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

    private fun detectFaces() {
        try {
            Thread {
                val modelFile = File(cacheDir, "face_detection_yunet_2023mar.onnx")
                val inputStream = resources.openRawResource(R.raw.face_detection_yunet_2023mar)
                val fos = FileOutputStream(modelFile)
                val buffer = ByteArray(4096)
                var length: Int
                while ((inputStream.read(buffer).also { length = it }) > 0) {
                    fos.write(buffer, 0, length)
                }
                fos.close()
                inputStream.close()
                val dest = Bitmap.createBitmap(bitmap.width, bitmap.height, Bitmap.Config.ARGB_8888)
                val success = NativeLib.detectFaces(bitmap, dest, modelFile.absolutePath)
                Log.i(TAG, "detectFaces result:$success")
                if (success) {
                    runOnUiThread {
                        binding.iv.setImageBitmap(dest)
                    }
                }
            }.start()
        } catch (e: IOException) {
            e.printStackTrace()
            Log.e(TAG, "Failed to ONNX model from resources! Exception thrown: $e")
            Toast.makeText(this, "Failed to ONNX model from resources!", Toast.LENGTH_LONG).show()
            return
        }
    }

    private fun detectFacesV2() {
        try {
            Thread {
                val modelFile = File(cacheDir, "lbpcascade_frontalface.xml")
                val inputStream = resources.openRawResource(R.raw.lbpcascade_frontalface)
                val fos = FileOutputStream(modelFile)
                val buffer = ByteArray(4096)
                var length: Int
                while ((inputStream.read(buffer).also { length = it }) > 0) {
                    fos.write(buffer, 0, length)
                }
                fos.close()
                inputStream.close()
                val dest = Bitmap.createBitmap(bitmap.width, bitmap.height, Bitmap.Config.ARGB_8888)
                val success = NativeLib.detectFacesV2(bitmap, dest, modelFile.absolutePath)
                Log.i(TAG, "detectFaces result:$success")
                if (success) {
                    runOnUiThread {
                        binding.iv.setImageBitmap(dest)
                    }
                }
            }.start()
        } catch (e: IOException) {
            e.printStackTrace()
            Log.e(TAG, "Failed to ONNX model from resources! Exception thrown: $e")
            Toast.makeText(this, "Failed to ONNX model from resources!", Toast.LENGTH_LONG).show()
            return
        }
    }

    companion object {
        private const val TAG = "MainActivity"
    }
}