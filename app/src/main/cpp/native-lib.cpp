#include <jni.h>
#include <string>
#include "log.h"
#include "detect-crc32.h"
#include <jni.h>



extern "C"
JNIEXPORT jstring JNICALL
Java_ng1ok_detect_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello;
    if(sameTextCRC32){
        hello = "CRC檢驗通過";
    }else{
        hello = "CRC檢驗不通過";
    }

    return env->NewStringUTF(hello.c_str());

}

extern "C"
JNIEXPORT jstring JNICALL
Java_ng1ok_detect_MainActivity_checkCRC32(JNIEnv *env, jobject thiz) {
    detectCRC32();

    std::string res;
    if(sameTextCRC32){
        res = "[✔] libc.text段CRC校驗";
    }else{
        res = "[✖] libc.text段CRC校驗";
    }


    return env->NewStringUTF(res.c_str());
}


extern "C"
JNIEXPORT jstring JNICALL
Java_ng1ok_detect_MainActivity_checkCRC32WithSyscall(JNIEnv *env, jobject thiz) {
    detectCRC32WithSyscall();

    std::string res;
    if(sameTextCRC32_sys){
        res = "[✔] libc.text段CRC校驗 (using syscall)";
    }else{
        res = "[✖] libc.text段CRC校驗 (using syscall)";
    }


    return env->NewStringUTF(res.c_str());

}

extern "C"
JNIEXPORT jstring JNICALL
Java_ng1ok_detect_MainActivity_checkCRC32WithMySyscall(JNIEnv *env, jobject thiz) {
    detectCRC32WithMySyscall();

    std::string res;
    if(sameTextCRC32_mySys){
        res = "[✔] libc.text段CRC校驗 (using mySyscall)";
    }else{
        res = "[✖] libc.text段CRC校驗 (using mySyscall)";
    }


    return env->NewStringUTF(res.c_str());
}


