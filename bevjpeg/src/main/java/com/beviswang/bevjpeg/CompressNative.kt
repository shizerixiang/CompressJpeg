package com.beviswang.bevjpeg

import android.graphics.Bitmap
import android.graphics.BitmapFactory

object CompressNative {
    // Used to load the 'jpeg-lib' library on application startup.
    init {
        System.loadLibrary("jpeg-lib")
    }

    /**
     * Compress bitmap.This is a I/O operation.
     * @param bitmap Bitmap that needs to be compressed.
     * @param outputPath Absolute path of output.
     * @param quality Quality after compression.The maximum value is 100,minimum value is 1.
     * @param width The width of the generated bitmap.
     * @param height The height of the generated bitmap.
     * @param optimize Is compress.If it is true,the volume of compression will be smaller.
     * If false,compression time will be shorter.The default value is true.
     * @return Whether compression is successful or not.
     */
    fun compressBitmap(bitmap: Bitmap, outputPath: String, quality: Int = 80,
                       width: Int = bitmap.width, height: Int = bitmap.height,
                       optimize: Boolean = true): Boolean =
            compressBitmap(bitmap, width, height, quality, outputPath.toByteArray(),
                    optimize) == "1"


    /**
     * Compress bitmap.This is a I/O operation.
     * @param filePath The absolute path of bitmap.
     * @param outputPath Absolute path of output.
     * @param quality Quality after compression.The maximum value is 100,minimum value is 1.
     * @param width The width of the generated bitmap.
     * @param height The height of the generated bitmap.
     * @param optimize Is compress.If it is true,the volume of compression will be smaller.
     * If false,compression time will be shorter.The default value is true.
     * @return Whether compression is successful or not.
     */
    fun compressFile(filePath: String, outputPath: String, quality: Int = 80,
                     width: Int = -1, height: Int = -1,
                     optimize: Boolean = true): Boolean {
        val bitmap = BitmapFactory.decodeFile(filePath)
        val bHeight = if (height == -1) bitmap.height else bitmap.height
        val bWidth = if (width == -1) bitmap.width else bitmap.width
        return compressBitmap(bitmap, outputPath, quality = quality, height = bHeight,
                width = bWidth, optimize = optimize)
    }

    /** This native method is a I/O operation. */
    private external fun compressBitmap(bitmap: Bitmap, width: Int, height: Int, quality: Int,
                                        fileName: ByteArray, optimize: Boolean): String?
}