#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_bn3monkey_datadestlibrary_DataDestinationNativeLibrary_open(JNIEnv *env, jobject thiz,
                                                             jint sock) {
    // TODO: implement openNGS()
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_bn3monkey_datadestlibrary_DataDestinationNativeLibrary_close(JNIEnv *env, jobject thiz,
                                                              jint sock) {
    // TODO: implement closeNGS()
}