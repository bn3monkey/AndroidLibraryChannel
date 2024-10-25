//
// Created by bn3mo on 2024-10-25.
//
#include <jni.h>
#include <string>
#include <mutex>

extern "C"
JNIEXPORT jlong JNICALL
Java_com_bn3monkey_datasourcelibrary_NativeAddress_getByteArrayAddress(JNIEnv *env, jobject thiz,
                                                                       jbyteArray value) {
    auto* address = env->GetByteArrayElements(value, nullptr);
    return (jlong)address;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_bn3monkey_datasourcelibrary_NativeAddress_releaseByteArrayAddress(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jbyteArray value,
                                                                           jlong address) {
    env->ReleaseByteArrayElements(value, (jbyte*)address, 0);
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_bn3monkey_datasourcelibrary_NativeAddress_getIntArrayAddress(JNIEnv *env, jobject thiz,
                                                                      jintArray value) {
    auto* address = env->GetIntArrayElements(value, nullptr);
    return (jlong)address;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_bn3monkey_datasourcelibrary_NativeAddress_releaseIntArrayAddress(JNIEnv *env, jobject thiz,
                                                                          jintArray value,
                                                                          jlong address) {
    env->ReleaseIntArrayElements(value, (jint*)address, 0);
}