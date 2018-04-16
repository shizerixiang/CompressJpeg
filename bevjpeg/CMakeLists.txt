cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)
# set() 用来设置一个路径全局变量
set(dis_DIR ${CMAKE_SOURCE_DIR}/libBuild)

# 添加一个动态库，直接引用 .so 文件
add_library(lib_jpeg SHARED IMPORTED)
set_target_properties(lib_jpeg PROPERTIES IMPORTED_LOCATION
 ${dis_DIR}/jpeg/lib/${ANDROID_ABI}/libjpeg.so)

# 创建一个总库 jpeg-lib 并链接一个.c文件，可以直接引用第三方库的头文件
add_library(jpeg-lib SHARED src/main/cpp/jpeg-lib.c)

# 链接第三方库的头文件
target_include_directories(jpeg-lib PRIVATE
                        ${dis_DIR}/jpeg/include)

find_library(log-lib log)

# 把所有库文件都引入工程
target_link_libraries(jpeg-lib
                   android
                   jnigraphics
                   lib_jpeg
                   ${log-lib})