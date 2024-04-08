package com.ygq.ndk.signature.ui

import android.R.id.input
import android.content.pm.PackageInfo
import android.content.pm.PackageManager
import android.content.pm.Signature
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.ygq.ndk.signature.NativeLib
import com.ygq.ndk.signature.databinding.ActivityMainBinding
import java.nio.charset.Charset
import java.security.MessageDigest
import java.security.NoSuchAlgorithmException


class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        val nativeLib = NativeLib()
        binding.tv.text = nativeLib.signatureParams("userName=240336124&userPwd=123456")

        val packageInfo: PackageInfo = packageManager.getPackageInfo(packageName, PackageManager.GET_SIGNATURES)
        val signatures: Array<Signature> = packageInfo.signatures
        val signature = signatures[0].toCharsString()
        Log.i(TAG, "signature:$signature")

        binding.tv2.text = getMD5Str("userName=240336124&userPwd=123456")
    }

    private fun getMD5Str(str: String): String {
        val md = MessageDigest.getInstance("MD5")
        val messageDigest = md.digest(str.toByteArray())

        val hexString = StringBuilder()
        for (byte in messageDigest) {
            val hex = Integer.toHexString(0xff and byte.toInt())
            if (hex.length == 1) hexString.append('0')
            hexString.append(hex)
        }
        return hexString.toString()
    }

    companion object {
        private const val TAG = "MainActivity"
    }
}