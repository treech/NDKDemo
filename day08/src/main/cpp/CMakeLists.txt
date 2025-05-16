# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
set(libname opencv)
project("${libname}")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

#${CMAKE_SOURCE_DIR}为CMakeLists文件的当前路径
#设置so库路径
set(my_lib_path ${CMAKE_SOURCE_DIR}/../../../src/main/jniLibs)

# 添加头文件路径
include_directories(${CMAKE_SOURCE_DIR}/../../../src/main/jniLibs/include)

add_library(
        opencv_java4
        SHARED
        IMPORTED)
set_target_properties(
        opencv_java4
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libopencv_java4.so)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(
        ${libname}
        SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        nativelib.cpp
)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(
        ${libname} opencv_java4
        # List libraries link to the target library
        android
        # 引用bitmap头文件需要加
        jnigraphics
        log
)