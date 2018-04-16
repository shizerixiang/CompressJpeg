#include <android/bitmap.h>
#include <android/log.h>
#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <time.h>
#include "jpeglib.h"
#include "cdjpeg.h"
#include <dirent.h>

typedef uint8_t BYTE;
#define TAG "compressNative "
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

#define true 1
#define false 0
char *error;
struct my_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr *my_error_ptr;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo) {
    my_error_ptr myerr = (my_error_ptr) cinfo->err;
    (*cinfo->err->output_message)(cinfo);
    error = (char *) myerr->pub.jpeg_message_table[myerr->pub.msg_code];
    LOGE("jpeg_message_table[%d]:%s", myerr->pub.msg_code,
         myerr->pub.jpeg_message_table[myerr->pub.msg_code]);
    longjmp(myerr->setjmp_buffer, 1);
}

/*
 * Generate JPEG.
 */
int generateJPEG(BYTE *data, int w, int h, int quality,
                 const char *outfilename, jboolean optimize) {
    int nComponent = 3;

    struct jpeg_compress_struct jcs;

    struct my_error_mgr jem;

    jcs.err = jpeg_std_error(&jem.pub);
    jem.pub.error_exit = my_error_exit;
    if (setjmp(jem.setjmp_buffer)) {
        LOGE("Setjmp is failed.");
        return 0;
    }
    // Assigning space for the JPEG object and initializing.
    jpeg_create_compress(&jcs);
    // Obtaining file information.
    LOGI("The output path: %s", outfilename);
    FILE *f = fopen(outfilename, "wb+");
    if (f == NULL) {
        LOGE("File is not found and failed to create.");
        return 0;
    }
    // Specify the compressed data source.
    jpeg_stdio_dest(&jcs, f);
    jcs.image_width = (JDIMENSION) w;
    jcs.image_height = (JDIMENSION) h;

    jcs.arith_code = false;
    jcs.input_components = nComponent;
    jcs.in_color_space = JCS_RGB;

    jpeg_set_defaults(&jcs);
    jcs.optimize_coding = optimize;
    // Set compress parameters.
    jpeg_set_quality(&jcs, quality, true);
    // Compress start.
    jpeg_start_compress(&jcs, TRUE);

    JSAMPROW row_pointer[1];
    int row_stride;
    row_stride = jcs.image_width * nComponent;
    while (jcs.next_scanline < jcs.image_height) {
        row_pointer[0] = &data[jcs.next_scanline * row_stride];
        // Write data.
        jpeg_write_scanlines(&jcs, row_pointer, 1);
    }

    if (jcs.optimize_coding) {
        LOGI("The optimize is true.");
    } else {
        LOGI("The optimize is false.");
    }
    // Compression complete.
    jpeg_finish_compress(&jcs);
    // Free resource.
    jpeg_destroy_compress(&jcs);
    fclose(f);

    return 1;
}

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb;

/**
 * Conversion java/lang/String is char*.
 * @param env
 * @param barr
 * @return
 */
char *jstringTostring(JNIEnv *env, jbyteArray barr) {
    char *rtn = NULL;
    jsize alen = (*env)->GetArrayLength(env, barr);
    jbyte *ba = (*env)->GetByteArrayElements(env, barr, 0);
    if (alen > 0) {
        rtn = (char *) malloc((size_t) (alen + 1));
        memcpy(rtn, ba, (size_t) alen);
        rtn[alen] = 0;
    }
    (*env)->ReleaseByteArrayElements(env, barr, ba, 0);
    return rtn;
}

/*
 * Class:     com_beviswang_bevjpeg_CompressNative
 * Method:    compressBitmap
 */
JNIEXPORT jstring JNICALL Java_com_beviswang_bevjpeg_CompressNative_compressBitmap
        (JNIEnv *env, jclass jclazz,jobject bitmap,
         jint width, jint height,
         jint quality,
         jbyteArray fileName,
         jboolean optimize) {
    AndroidBitmapInfo infoColor;
    BYTE *pixelColor;
    BYTE *data;
    BYTE *tempData;
    char *filename = jstringTostring(env, fileName);

    if ((AndroidBitmap_getInfo(env, bitmap, &infoColor)) < 0) {
        LOGE("Parsing failure.");
        return (*env)->NewStringUTF(env, "0");
    }

    if ((AndroidBitmap_lockPixels(env, bitmap, (void **) &pixelColor)) < 0) {
        LOGE("Load failure.");
    }

    BYTE r, g, b;
    int color;
    data = malloc((size_t) (width * height * 3));
    tempData = data;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            color = *((int *) pixelColor);
            r = (BYTE) ((color & 0x00FF0000) >> 16);
            g = (BYTE) ((color & 0x0000FF00) >> 8);
            b = (BYTE) (color & 0X000000FF);

            *data = b;
            *(data + 1) = g;
            *(data + 2) = r;
            data += 3;
            pixelColor += 4;
        }
    }

    AndroidBitmap_unlockPixels(env, bitmap);
    int resultCode = generateJPEG(tempData, width, height, quality, filename, optimize);

    free(tempData);
    if (resultCode == 0) {
        jstring result = (*env)->NewStringUTF(env, "0");
        LOGE("The method generateJPEG() has not been successfully executed.");
        return result;
    }

    return (*env)->NewStringUTF(env, "1");
}