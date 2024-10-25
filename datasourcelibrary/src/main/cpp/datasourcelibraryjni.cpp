#include <jni.h>
#include <string>
#include <mutex>

#include "typeconverter/kotlintypeconverter.h"
#include "datasourcenativelibrary_v2_0_0.hpp"


extern "C"
JNIEXPORT jstring JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_getVersionNative(JNIEnv *env,
                                                                             jobject thiz) {
    const char* version = DataSourceLibrary_version();
    jstring ret = env->NewStringUTF(version);
    return ret;
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_initializeNative(JNIEnv *env,
                                                                             jobject thiz) {
    GlobalJVM::initialize(env);
    auto context = DataSourceLibrary_initialize();
    jlong ret = (jlong)context;
    return ret;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_releaseNative(JNIEnv *env, jobject thiz,
                                                                          jlong context) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    DataSourceLibrary_release(nativeContext);
    GlobalJVM::release();
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_connectDeviceNative(JNIEnv *env,
                                                                                jobject thiz,
                                                                                jlong context,
                                                                                jint fd) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    return DataSourceLibrary_connectDevice(nativeContext, fd);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_disconnectDeviceNative(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jlong context) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    return DataSourceLibrary_disconnectDevice(nativeContext);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_isDeviceConnectedNative(JNIEnv *env,
                                                                                    jobject thiz,
                                                                                    jlong context) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    return DataSourceLibrary_isDeviceConnected(nativeContext);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_writeToDeviceEEPROMNative(JNIEnv *env,
                                                                                      jobject thiz,
                                                                                      jlong context,
                                                                                      jint address,
                                                                                      jbyteArray buffer,
                                                                                      jint offset,
                                                                                      jint length) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    auto* nativeBuffer = env->GetByteArrayElements(buffer, nullptr);

    bool ret = DataSourceLibrary_writeToDeviceEEPROM(nativeContext, address, (const uint8_t*)nativeBuffer, offset, length);

    env->ReleaseByteArrayElements(buffer, nativeBuffer, 0);
    return ret;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_readFromDeviceEEPROMNative(JNIEnv *env,
                                                                                       jobject thiz,
                                                                                       jlong context,
                                                                                       jint address,
                                                                                       jbyteArray buffer,
                                                                                       jint offset,
                                                                                       jint length) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    auto* nativeBuffer = env->GetByteArrayElements(buffer, nullptr);

    bool ret =  DataSourceLibrary_readFromDeviceEEPROM(nativeContext, address, (uint8_t*)nativeBuffer, offset, length);
    env->ReleaseByteArrayElements(buffer, nativeBuffer, 0);
    return ret;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_burstWriteNative(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jlong context,
                                                                             jint address,
                                                                             jbyteArray buffer,
                                                                             jint offset,
                                                                             jint length) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    auto* nativeBuffer = env->GetByteArrayElements(buffer, nullptr);

    bool ret = DataSourceLibrary_burstWrite(nativeContext, address, (uint8_t*)nativeBuffer, offset, length);
    env->ReleaseByteArrayElements(buffer, nativeBuffer, 0);
    return ret;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_singleReadNative(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jlong context,
                                                                             jint address,
                                                                             jintArray out_value) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    auto* nativeOutValue = env->GetIntArrayElements(out_value, nullptr);

    bool ret = DataSourceLibrary_singleRead(nativeContext, address, (uint16_t*)nativeOutValue);

    env->ReleaseIntArrayElements(out_value, nativeOutValue, 0);
    return ret;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_singleWriteNative(JNIEnv *env,
                                                                              jobject thiz,
                                                                              jlong context,
                                                                              jint address,
                                                                              jint value) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    return DataSourceLibrary_singleWrite(nativeContext, address, (uint16_t)value);

}
extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_getDeviceIDNative(JNIEnv *env,
                                                                              jobject thiz,
                                                                              jlong context) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    return DataSourceLibrary_getDeviceID(nativeContext);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_getDeviceComponentAIDNative(JNIEnv *env,
                                                                                        jobject thiz,
                                                                                        jlong context) {

    auto* nativeContext= (DataSourceLibraryContext*)context;
    return DataSourceLibrary_getDeviceComponentAID(nativeContext);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_getDeviceComponentBIDNative(JNIEnv *env,
                                                                                        jobject thiz,
                                                                                        jlong context) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    return DataSourceLibrary_getDeviceComponentBID(nativeContext);
}

class DeviceStateCallback
{
public:
    explicit DeviceStateCallback(jobject callback) : _callback(callback) {
    }
    virtual ~DeviceStateCallback() = default;
    void operator()(int32_t value) {
        _callback(value);
    }

    static void* create(jobject callback) {
        auto* ret = new DeviceStateCallback(callback);
        return (void*)ret;
    }
    static void run(void* instance, int32_t value)
    {
        auto* callback = static_cast<DeviceStateCallback*>(instance);
        (*callback)(value);
    }
    static void release(void* instance) {
        auto* callback = static_cast<DeviceStateCallback*>(instance);
        delete callback;
    }

private:
    KotlinCallback<int32_t> _callback;
};

extern "C"
JNIEXPORT void JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_registerDeviceStateCallbackNative(
        JNIEnv *env, jobject thiz, jlong context, jobject on_device_state_changed) {


    auto* nativeContext= (DataSourceLibraryContext*)context;
    auto* user_context = DeviceStateCallback::create(on_device_state_changed);

    DataSourceLibrary_registerDeviceStateCallback(nativeContext, user_context,
                                                  DeviceStateCallback::run,
                                                  DeviceStateCallback::release);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_unregisterDeviceStateCallbackNative(
        JNIEnv *env, jobject thiz, jlong context) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    DataSourceLibrary_unregisterDeviceStateCallback(nativeContext);
}


class DeviceDisconnectionCallback
{
public:
    explicit DeviceDisconnectionCallback(jobject callback) : _callback(callback) {
    }
    virtual ~DeviceDisconnectionCallback() = default;
    void operator()() {
        _callback();
    }

    static void* create(jobject callback) {
        auto* ret = new DeviceDisconnectionCallback(callback);
        return (void*)ret;
    }
    static void run(void* instance)
    {
        auto* callback = static_cast<DeviceDisconnectionCallback*>(instance);
        (*callback)();
    }
    static void release(void* instance) {
        auto* callback = static_cast<DeviceDisconnectionCallback*>(instance);
        delete callback;
    }

private:
    KotlinCallback<void> _callback;
};

extern "C"
JNIEXPORT void JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_registerDeviceDisconnectionCallbackNative(
        JNIEnv *env, jobject thiz, jlong context, jobject on_device_disconnected) {
    // TODO: implement registerDeviceDisconnectionCallbackNative()
    auto* nativeContext= (DataSourceLibraryContext*)context;
    auto* user_context = DeviceDisconnectionCallback::create(on_device_disconnected);

    DataSourceLibrary_registerDeviceDisconnectionCallback(nativeContext, user_context,
                                           DeviceDisconnectionCallback::run,
                                           DeviceDisconnectionCallback::release);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_unregisterDeviceDisconnectionCallbackNative(
        JNIEnv *env, jobject thiz, jlong context) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    DataSourceLibrary_unregisterDeviceDisconnectionCallback(nativeContext);
}

class DataCallback
{
public:
    explicit DataCallback(jobject callback) : _callback(callback) {
    }
    virtual ~DataCallback() = default;
    void operator()(int8_t* value, int32_t value2) {
        _callback(value, value2);
    }

    static void* create(jobject callback) {
        auto* ret = new DataCallback(callback);
        return (void*)ret;
    }
    static void run(void* instance, int8_t* value, int32_t value2)
    {
        auto* callback = static_cast<DataCallback*>(instance);
        (*callback)(value, value2);
    }
    static void release(void* instance) {
        auto* callback = static_cast<DataCallback*>(instance);
        delete callback;
    }

private:
    KotlinCallback<int8_t*, int32_t> _callback;
};

extern "C"
JNIEXPORT void JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_registerDataCallbackNative(JNIEnv *env,
                                                                                        jobject thiz,
                                                                                        jlong context,
                                                                                        jobject on_data_received) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    auto* user_context = DataCallback::create(on_data_received);

    DataSourceLibrary_registerDataCallback(nativeContext, user_context,
                                           DataCallback::run,
                                           DataCallback::release);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_unregisterDataCallbackNative(
        JNIEnv *env, jobject thiz, jlong context) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    DataSourceLibrary_unregisterDataCallback(nativeContext);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_writeToDeviceEEPROM2Native(JNIEnv *env,
                                                                                        jobject thiz,
                                                                                        jlong context,
                                                                                        jint address,
                                                                                        jlong buffer_address,
                                                                                        jint offset,
                                                                                        jint length) {
    // TODO: implement writeToDeviceEEPROM2Native()
    auto* nativeContext= (DataSourceLibraryContext*)context;
    return DataSourceLibrary_writeToDeviceEEPROM(nativeContext, address, (const uint8_t*)buffer_address, offset, length);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_readFromDeviceEEPROM2Native(
        JNIEnv *env, jobject thiz, jlong context, jint address, jlong buffer_address, jint offset,
        jint length) {
    auto* nativeContext= (DataSourceLibraryContext*)context;
    return DataSourceLibrary_readFromDeviceEEPROM(nativeContext, address, (uint8_t*)buffer_address, offset, length);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_burstWrite2Native(JNIEnv *env,
                                                                               jobject thiz,
                                                                               jlong context,
                                                                               jint address,
                                                                               jlong buffer_address,
                                                                               jint offset,
                                                                               jint length) {
    // TODO: implement burstWrite2Native()
    auto* nativeContext= (DataSourceLibraryContext*)context;
    return DataSourceLibrary_burstWrite(nativeContext, address, (uint8_t*)buffer_address, offset, length);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_bn3monkey_datasourcelibrary_DataSourceNativeLibrary_singleRead2Native(JNIEnv *env,
                                                                               jobject thiz,
                                                                               jlong context,
                                                                               jint address,
                                                                               jlong value_address) {

    auto* nativeContext= (DataSourceLibraryContext*)context;
    return DataSourceLibrary_singleRead(nativeContext, address, (uint16_t*)value_address);
}