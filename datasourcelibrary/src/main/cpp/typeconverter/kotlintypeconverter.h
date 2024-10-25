//
// Created by bn3mo on 2024-10-24.
//

#ifndef ANDROIDNATIVELIBRARYCHANNEL_KOTLINTYPECONVERTER_H
#define ANDROIDNATIVELIBRARYCHANNEL_KOTLINTYPECONVERTER_H

#include <jni.h>
#include <mutex>
#include <cstdint>

class GlobalJVM
{
public:
    static bool initialize(JNIEnv * env) {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            env->GetJavaVM(&_vm);
        }
        return true;
    }
    static void release() {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _vm = nullptr;
        }
    }

    static JavaVM* get() {
        JavaVM* ret;
        {
            std::lock_guard<std::mutex> lock(_mtx);
            ret = _vm;
        }
        return ret;
    }
private:
    static inline JavaVM* _vm;
    static inline std::mutex _mtx;
};

class JNIScope
{
public:
    explicit JNIScope(JavaVM* vm) : _vm(nullptr), _env(nullptr), _attached(false) {
        if (vm == nullptr)
            return;

        _vm = vm;
        int getEnvStat = _vm->GetEnv((void **)&_env, JNI_VERSION_1_6);
        switch (getEnvStat)
        {
            case JNI_EDETACHED :
                auto result = _vm->AttachCurrentThread(&_env, nullptr);
                if (result != 0)
                {
                    _attached = true;
                }
                break;
        }
    }
    virtual ~JNIScope() {
        if (_attached) {
            if (_vm)
            {
                _vm->DetachCurrentThread();
            }
        }
    }
    JNIEnv* get() {
        return _env;
    }
private:
    JavaVM* _vm;
    JNIEnv* _env;
    bool _attached {false};
};

template<typename NativeType>
struct KotlinType
{
    static_assert(std::is_same<NativeType, void>::value, "Cannot be converted into kotlin type");
};

template<>
struct KotlinType<int32_t>
{
    // Kotlin Int
    using value = jobject;
};
template<>
struct KotlinType<int64_t>
{
    // Kotlin Long
    using value = jobject;
};
template<>
struct KotlinType<double>
{
    // Kotlin Double
    using value = jobject;
};
template<>
struct KotlinType<bool>
{
    // Kotlin Boolean
    using value = jobject;
};

template<>
struct KotlinType<int8_t>
{
    // Kotlin Byte
    using value = jobject;
};

template<>
struct KotlinType<char*>
{
    // C-Style String
    using value = jstring;
};

template<>
struct KotlinType<int32_t*>
{
    using value = jintArray;
};

template<>
struct KotlinType<int64_t*>
{
    using value = jlongArray;
};

template<>
struct KotlinType<double*>
{
    using value = jdoubleArray;
};

template<>
struct KotlinType<bool*>
{
    using value = jbooleanArray;
};

template<>
struct KotlinType<int8_t*>
{
    using value = jbyteArray;
};

// We don't have to downgrade under C++17.
// Android application always require to follow android os
template<typename NativeType>
typename KotlinType<NativeType>::value toKotlinType(NativeType value, int32_t length = 8196)
{
    {
        JNIScope scope{GlobalJVM::get()};
        auto *env = scope.get();
        if (!env)
            return nullptr;

        // Convert int32_t to Kotlin Int Type
        if constexpr (std::is_same_v<NativeType, int32_t>) {
            jclass integerClass = env->FindClass("java/lang/Integer");
            jmethodID intInit = env->GetMethodID(integerClass, "<init>", "(I)V");
            return env->NewObject(integerClass, intInit, (jint)value);
        }
        // Convert int64_t to Kotlin Long Type
        else if constexpr (std::is_same_v<NativeType, int64_t>) {
            jclass longClass = env->FindClass("java/lang/Long");
            jmethodID longInit = env->GetMethodID(longClass, "<init>", "(J)V");
            return env->NewObject(longClass, longInit, (jlong)value);
        }
        // Convert int64_t to Kotlin Boolean Type
        else if constexpr (std::is_same_v<NativeType, bool>) {
            jclass booleanClass = env->FindClass("java/lang/Boolean");
            jmethodID booleanInit = env->GetMethodID(booleanClass, "<init>", "(Z)V");
            return env->NewObject(booleanClass, booleanInit, (jboolean)value);
        }
        // Convert int64_t to Kotlin Double Type
        else if constexpr (std::is_same_v<NativeType, double>) {
            jclass doubleClass = env->FindClass("java/lang/Double");
            jmethodID doubleInit = env->GetMethodID(doubleClass, "<init>", "(D)V");
            return env->NewObject(doubleClass, doubleInit, (jdouble)value);
        }
        // Convert int8_t to Kotlin Byte Type
        else if constexpr (std::is_same_v<NativeType, int8_t>) {
            jclass byteClass = env->FindClass("java/lang/Byte");
            jmethodID byteInit = env->GetMethodID(byteClass, "<init>", "(B)V");
            return env->NewObject(byteClass, byteInit, (jbyte)value);
        }
        // Convert char* to Kotlin String Type
        else if constexpr (std::is_same_v<NativeType, char *>) {
            return env->NewStringUTF(value);
        }
        // Convert int32_t* to Kotlin IntArray Type
        else if constexpr (std::is_same_v<NativeType, int32_t*>) {
            jintArray intArray = env->NewIntArray(length);
            env->SetIntArrayRegion(intArray, 0, sizeof(value) / sizeof(int32_t), value);
            return intArray;
        }
        // Convert int64_t* to Kotlin LongArray Type
        else if constexpr (std::is_same_v<NativeType, int64_t*>) {
            jlongArray longArray = env->NewLongArray(length);
            env->SetLongArrayRegion(longArray, 0, length, value);
            return longArray;
        }
        // Convert double* to Kotlin DoubleArray Type
        else if constexpr (std::is_same_v<NativeType, double*>) {
            jdoubleArray doubleArray = env->NewDoubleArray(length);
            env->SetDoubleArrayRegion(doubleArray, 0, length, value);
            return doubleArray;
        }
        // Convert bool* to Kotlin Boolean Array Type
        else if constexpr (std::is_same_v<NativeType, bool*>) {
            jbooleanArray booleanArray = env->NewBooleanArray(length);
            env->SetBooleanArrayRegion(booleanArray, 0, length, value);
            return booleanArray;
        }
        // Convert int8_t* to Kotlin Byte Array Type
        else if constexpr (std::is_same_v<NativeType, int8_t*>) {
            jbyteArray byteArray = env->NewByteArray(length);
            env->SetByteArrayRegion(byteArray, 0, length, value);
            return byteArray;
        }
        else {
            return nullptr;
        }
    }
}


template<typename... ArgumentType>
class KotlinCallback
{
public:
    explicit KotlinCallback(jobject callback) {
        char signature[512] {0};
        createSignature(signature);
        {
            JNIScope scope{GlobalJVM::get()};
            auto *env = scope.get();
            if (!env)
                return;

            _callback = env->NewGlobalRef(callback);
            jclass jclazz = env->GetObjectClass(_callback);
            _methodID = env->GetMethodID(jclazz, "invoke", signature);
        }
    }
    KotlinCallback(const KotlinCallback& other) = delete;
    virtual ~KotlinCallback() {
        {
            JNIScope scope{GlobalJVM::get()};
            auto *env = scope.get();
            if (!env)
                return;

            if (_callback)
            {
                env->DeleteGlobalRef(_callback);
            }
        }
    }
    void operator()(ArgumentType... args)
    {
        {
            JNIScope scope{GlobalJVM::get()};
            auto *env = scope.get();
            if (!env)
                return;

            env->CallObjectMethod(_callback, _methodID, toKotlinType(args)...);
        }
    }


private:
    jobject _callback {nullptr};
    jmethodID _methodID {nullptr};

    void createSignature(char* buffer)
    {
        size_t arg_size = sizeof...(ArgumentType);
        size_t index {1};
        const char* sig = "Ljava/lang/Object;";
        size_t sig_size = strlen(sig);
        buffer[0] = '(';
        for (size_t i =0;i<arg_size;i++)
        {
            std::strcpy(buffer + index, sig);
            index += sig_size;
        }
        buffer[index++] = ')';
        std::strcpy(buffer + index, sig);
    }

};

template<>
class KotlinCallback<void>
{
public:
    explicit KotlinCallback(jobject callback) {
        char signature[512] {0};
        createSignature(signature);
        {
            JNIScope scope{GlobalJVM::get()};
            auto *env = scope.get();
            if (!env)
                return;

            _callback = env->NewGlobalRef(callback);
            jclass jclazz = env->GetObjectClass(_callback);
            _methodID = env->GetMethodID(jclazz, "invoke", signature);
        }
    }
    KotlinCallback(const KotlinCallback& other) = delete;
    virtual ~KotlinCallback() {
        {
            JNIScope scope{GlobalJVM::get()};
            auto *env = scope.get();
            if (!env)
                return;

            if (_callback)
            {
                env->DeleteGlobalRef(_callback);
            }
        }
    }
    void operator()()
    {
        {
            JNIScope scope{GlobalJVM::get()};
            auto *env = scope.get();
            if (!env)
                return;

            env->CallObjectMethod(_callback, _methodID);
        }
    }


private:
    jobject _callback {nullptr};
    jmethodID _methodID {nullptr};

    void createSignature(char* buffer)
    {
        const char* sig = "Ljava/lang/Object;";
        size_t sig_size = strlen(sig);
        buffer[0] = '(';
        buffer[1] = ')';
        std::strcpy(buffer + 2, sig);
    }

};

// Code Snippet

#endif //ANDROIDNATIVELIBRARYCHANNEL_KOTLINTYPECONVERTER_H
