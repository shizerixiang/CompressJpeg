package com.beviswang.compressjpeg

import android.Manifest
import android.graphics.BitmapFactory
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.util.Log
import com.beviswang.bevjpeg.CompressNative
import com.beviswang.capturelib.util.PermissionHelper
import org.jetbrains.anko.doAsync
import java.io.File

class MainActivity : AppCompatActivity(), PermissionHelper.OnRequestPermissionsResultCallbacks {
    override fun onPermissionsDenied(requestCode: Int, perms: MutableList<String>?, isAllDenied: Boolean) {
        finish()
    }

    override fun onPermissionsGranted(requestCode: Int, perms: MutableList<String>?, isAllGranted: Boolean) {
        decode()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method

        if (PermissionHelper.requestPermissions(this, 0x20,
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
            decode()
        } else finish()
    }

    private fun decode() {
        // TODO 异步
        doAsync {
            val bitmap = BitmapFactory.decodeResource(resources, R.mipmap.bg_001)
            val path = CompressNative.compressBitmap(bitmap, Environment.getExternalStorageDirectory().absolutePath + File.separator +
                    "DTTimer" + File.separator+"Photo"+File.separator+"BevisW.jpg",quality = 60)
            Log.e("activity", "是否成功：$path")
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        PermissionHelper.onRequestPermissionsResult(requestCode, permissions, grantResults, this)
    }
}
