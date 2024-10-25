package com.bn3monkey.datasourcelibrary

class NativeAddress {
    external fun getByteArrayAddress(value : ByteArray) : Long
    external fun releaseByteArrayAddress(value : ByteArray, address : Long)

    external fun getIntArrayAddress(value : IntArray) : Long
    external fun releaseIntArrayAddress(value : IntArray, address : Long)

    companion object {
        // Used to load the 'datasourcelibrary' library on application startup.
        init {
            System.loadLibrary("nativeaddress")
        }
    }
}