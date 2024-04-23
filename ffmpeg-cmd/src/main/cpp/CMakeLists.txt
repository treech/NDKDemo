cmake_minimum_required(VERSION 3.22.1)

#配置project名称
set(libname ffmpeg-lib)
project("${libname}")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

#设置so库路径
set(my_lib_path ${CMAKE_SOURCE_DIR}/../../../src/main/jniLibs)

#${CMAKE_SOURCE_DIR}为CMakeLists文件的当前路径
# 添加ffmpeg头文件路径
include_directories(${CMAKE_SOURCE_DIR}/../../../src/main/jniLibs/include)
include_directories(${CMAKE_SOURCE_DIR}/../../../src/main/jniLibs/other)

#编解码
add_library(
        avcodec
        SHARED
        IMPORTED)
set_target_properties(
        avcodec
        PROPERTIES IMPORTED_LOCATION
       ${my_lib_path}/${ANDROID_ABI}/libavcodec-56.so)

#设备信息
add_library(
        avdevice
        SHARED
        IMPORTED)
set_target_properties(
        avdevice
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libavdevice-56.so
)

#滤镜特效处理
add_library(
        avfilter
        SHARED
        IMPORTED)
set_target_properties(
        avfilter
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libavfilter-5.so
)

#封装格式
add_library(
        avformat
        SHARED
        IMPORTED)
set_target_properties(
        avformat
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libavformat-56.so
)

#工具库
add_library(
        avutil
        SHARED
        IMPORTED)
set_target_properties(
        avutil
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libavutil-54.so
)

#后期处理
add_library(
        postproc
        SHARED
        IMPORTED)
set_target_properties(
        postproc
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libpostproc-53.so
)

#音视频采样数据格式转换
add_library(
        swresample
        SHARED
        IMPORTED)
set_target_properties(
        swresample
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libswresample-1.so
)

#视频像素数据格式转换
add_library(
        swscale
        SHARED
        IMPORTED)
set_target_properties(
        swscale
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libswscale-3.so
)

add_library( # Sets the name of the library.
        ${libname}

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp
        ffmpeg_filter.c
        ffmpeg_mod.c
        ffmpeg_opt.c
        cmdutils.c)

# 查找日志库
find_library(
        log-lib
        log)

# 将其他库链接到目标库ffmpeg-cmd
target_link_libraries(
        #链接额外的ffmpeg的编译
        ${libname} avcodec avdevice avfilter avformat avutil postproc swresample swscale
        ${log-lib})