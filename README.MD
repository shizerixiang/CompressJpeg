## 图片压缩工具 v1.0.0
#### 注意：现在仅支持 armeabi、armeabi-v7a 的 cpu 架构，计划未来将实现其他 cpu 架构。

### 添加依赖方式：
#### Step 1. 添加以下语句到你 Project 的 build.gradle 下
```
allprojects {
    repositories {
        ...
        maven { url 'https://jitpack.io' }
    }
}
```
#### Step 2. 添加依赖库到你 Module 的 build.gradle 下
```
dependencies {
        compile 'com.github.shizerixiang:CompressJpeg:v1.0.0'
}
```
### Using Kotlin
```
    // TODO 异步调用
    val bitmap = BitmapFactory.decodeResource(resources, R.mipmap.bg_001)
    val path = CompressNative.compressBitmap(bitmap, Environment.getExternalStorageDirectory().absolutePath + File.separator +
            "DTTimer" + File.separator+"Photo"+File.separator+"BevisW.jpg",quality = 60)
    Log.e("activity", "是否成功：$path")
```