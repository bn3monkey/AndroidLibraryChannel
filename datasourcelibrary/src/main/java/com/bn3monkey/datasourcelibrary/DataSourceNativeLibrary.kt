package com.bn3monkey.datasourcelibrary

import java.nio.ByteBuffer

class DataSourceNativeLibrary {
    private var nativeContext : Long
    init {
        nativeContext = initializeNative()
    }    
    fun dispose()
    {
        releaseNative(nativeContext)
    }

    fun getVersion() : String = getVersionNative()

    fun connectDevice(fd:Int) : Boolean = connectDeviceNative(nativeContext,fd)
    fun disconnectDevice() = disconnectDeviceNative(nativeContext)
    fun isDeviceConnected() = isDeviceConnectedNative(nativeContext)
    
    fun writeToDeviceEEPROM(address: Int, buffer : ByteArray, offset : Int, length : Int)
        = writeToDeviceEEPROMNative(nativeContext, address, buffer, offset, length)
    fun readFromDeviceEEPROM(address : Int, buffer : ByteArray, offset : Int, length : Int)
        = readFromDeviceEEPROMNative(nativeContext, address, buffer, offset, length)
    fun burstWrite(address : Int, buffer : ByteArray, offset : Int, length : Int)
        = burstWriteNative(nativeContext, address, buffer, offset, length)
    fun singleRead(address : Int, outValue : IntArray) : Int
        = singleReadNative(nativeContext, address, outValue)
    fun singleWrite(address : Int, value : Int) : Int
        = singleWriteNative(nativeContext, address, value)
    
    val deviceID
        get() = getDeviceIDNative(nativeContext)

    val deviceComponentAID  
        get() = getDeviceComponentAIDNative(nativeContext)
    
    val deviceComponentBID
        get() = getDeviceComponentBIDNative(nativeContext)

    fun registerDeviceStateCallback(onDeviceStateChanged: (Int) -> Unit) =
        registerDeviceStateCallbackNative(nativeContext, onDeviceStateChanged)
    fun unregisterDeviceStateCallback() = unregisterDeviceStateCallbackNative(nativeContext)

    fun registerDeviceDisconnectionCallback(onDeviceDisconnected : () -> Unit) =
        registerDeviceDisconnectionCallbackNative(nativeContext, onDeviceDisconnected)
    fun unregisterDeviceDisconnectionCallback() = unregisterDeviceDisconnectionCallbackNative(nativeContext)

    fun registerDataCallback(onDataReceived : (ByteArray, Int) -> Unit)
        = registerDataCallbackNative(nativeContext, onDataReceived)
    fun unregisterDataCallback()
        = unregisterDataCallbackNative(nativeContext)

    fun writeToDeviceEEPROM(
        address : Int, bufferAddress : Long, offset : Int, length : Int) : Int
        = writeToDeviceEEPROM2Native(nativeContext, address, bufferAddress, offset, length)
    fun readFromDeviceEEPROM(
        address : Int, bufferAddress : Long, offset : Int, length : Int) : Int
        = readFromDeviceEEPROM2Native(nativeContext, address, bufferAddress, offset, length)
    fun burstWrite(address : Int, bufferAddress : Long, offset : Int, length : Int) : Int
            = burstWrite2Native(nativeContext, address, bufferAddress, offset, length)
    fun singleRead(address : Int, valueAddress : Long) : Int
            = singleRead2Native(nativeContext, address, valueAddress)



    private external fun getVersionNative() : String
    private external fun initializeNative() : Long
    private external fun releaseNative(context : Long)

    private external fun connectDeviceNative(context : Long, fd : Int) : Boolean
    private external fun disconnectDeviceNative(context : Long)

    private external fun isDeviceConnectedNative(context : Long) : Boolean
    private external fun writeToDeviceEEPROMNative(
        context : Long, address : Int, buffer : ByteArray, offset : Int, length : Int) : Int
    private external fun readFromDeviceEEPROMNative(
        context : Long, address : Int, buffer : ByteArray, offset : Int, length : Int) : Int
    private external fun burstWriteNative(context : Long, address : Int, buffer : ByteArray, offset : Int, length : Int) : Int
    private external fun singleReadNative(context : Long, address : Int, outValue : IntArray) : Int
    private external fun singleWriteNative(context : Long, address : Int, value : Int) : Int

    private external fun getDeviceIDNative(context : Long) : Int
    private external fun getDeviceComponentAIDNative(context : Long) : Int
    private external fun getDeviceComponentBIDNative(context : Long) : Int

    private external fun registerDeviceStateCallbackNative(context : Long,
                                                     onDeviceStateChanged : (Int) -> Unit)
    private external fun unregisterDeviceStateCallbackNative(context : Long)

    private external fun registerDeviceDisconnectionCallbackNative(context : Long,
                                                                   onDeviceDisconnected : () -> Unit)
    private external fun unregisterDeviceDisconnectionCallbackNative(context : Long)

    private external fun registerDataCallbackNative(context : Long, onDataReceived : (ByteArray, Int) -> Unit)
    private external fun unregisterDataCallbackNative(context : Long)

    private external fun writeToDeviceEEPROM2Native(
        context : Long, address : Int, bufferAddress : Long, offset : Int, length : Int) : Int
    private external fun readFromDeviceEEPROM2Native(
        context : Long, address : Int, bufferAddress : Long, offset : Int, length : Int) : Int
    private external fun burstWrite2Native(context : Long, address : Int, bufferAddress : Long, offset : Int, length : Int) : Int
    private external fun singleRead2Native(context : Long, address : Int, valueAddress : Long) : Int


    companion object {
        // Used to load the 'datasourcelibrary' library on application startup.
        init {
            System.loadLibrary("datasourcelibrary")
        }
    }
}