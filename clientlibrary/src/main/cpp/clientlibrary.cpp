#include <jni.h>
#include <string>


extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_clientlibrary_ClientNativeLibrary_createSocket(JNIEnv *env, jobject thiz,
                                                                  jint port) {
    // TODO: implement createSocket()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_bn3monkey_clientlibrary_ClientNativeLibrary_closeSocket(JNIEnv *env, jobject thiz,
                                                                 jint socket) {
    // TODO: implement closeSocket()
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_bn3monkey_clientlibrary_ClientNativeLibrary_openNGS(JNIEnv *env, jobject thiz,
                                                             jint socket) {
    // TODO: implement openNGS()
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_bn3monkey_clientlibrary_ClientNativeLibrary_closeNGS(JNIEnv *env, jobject thiz,
                                                              jint socket) {
    // TODO: implement closeNGS()
}