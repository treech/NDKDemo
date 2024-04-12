package com.ygq.ndk.signature.ui

import android.content.pm.PackageInfo
import android.content.pm.PackageManager
import android.content.pm.Signature
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.ygq.ndk.signature.NativeLib
import com.ygq.ndk.signature.databinding.ActivityMainBinding


class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val packageInfo: PackageInfo = packageManager.getPackageInfo(packageName, PackageManager.GET_SIGNATURES)
        val signatures: Array<Signature> = packageInfo.signatures
        val signature = signatures[0].toCharsString()
        /**
        308202e4308201cc020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b30090603550406130255533020170d3231303532353036353032375a180f32303531303531383036353032375a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330820122300d06092a864886f70d01010105000382010f003082010a02820101008d71051c6ccaaa6e5ea21f919d8623772c03c377d84f3f99888642e18332cc5d3b3eb98a76ada68de5b0095def154ec7b5ad08c27fe69a3527a27cf6de3600b35680bb4a18ec9a27c72bbc1efa3676d16d65d9e0b71ccd0aa36efdee21fbb74cfe02546dd16e999d64b68018cf2b770c8bb3a7e68e56c0f73a46fca7d93a81110e149aa8358410d481c98de471d260532d979b53af4676c91a2ddf852af529499b2bf93b502e3bab5ba9bda760517fa9d426b5b70951a92c681871bc4782716705ca3450b8d17dd04d43eb8aaa5174490229503a57ac26b313f3849dc8efa9ca6935d002b9f7a4960387d1e8eb2fd4a305ebef55f77e3dcdad0c030c8b530d510203010001300d06092a864886f70d01010505000382010100636a55a1e25a25ff1e911d38d4c8b403a6b8b0076c7db14e1054f96d2c8f5be6cd77c9667f10a447ce562557dfed415522b8f423da7424270a8b726825cb1815962affc2f6b210654620711db7d16512164d6d1753d2c0eed41ae91d424876df5c5917832d9f237423ffd7145d2d770917d486ee46a1177e62a886d8490d475db1e37f055663275a70d3c7e646a437556fe8dd200782a8019aa77638c78955f13e2c4deaabf78730830d1924a5207ddcd82f2be3365d089e1f8574ab4d5330d24768e7f22c9015734e7e5b94f0fc022f1bae318fbb68b9868e626bb9ecec20224542dffdf18ce6e4e90dff2b5ce20cbfc6d11f39a027c49244269c4dc47cdfe2
         */
        Log.i(TAG, "signature:$signature")
        val nativeLib = NativeLib()
        nativeLib.signatureVerify(this)

        nativeLib.signatureParams("userName=240336124&userPwd=123456")
    }

    companion object {
        private const val TAG = "MainActivity"
    }
}