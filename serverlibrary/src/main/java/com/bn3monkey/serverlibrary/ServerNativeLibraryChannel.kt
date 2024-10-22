package com.bn3monkey.serverlibrary

import com.bn3monkey.serverlibrary.nativechannel.ServerNativeLibraryChannel_V1_0_0
import com.bn3monkey.serverlibrary.nativechannel.ServerNativeLibraryChannel_V2_0_0

interface ServerNativeLibraryChannel {
    fun run(command : ByteArray) : ByteArray
    val commandSize : Int
}

object ServerNativeChannelFactory
{
    fun create(nativeLibrary: ServerNativeLibrary) : ServerNativeLibraryChannel
    {
        return when(nativeLibrary.getVersion())
        {
            "1.0.0" -> ServerNativeLibraryChannel_V1_0_0(nativeLibrary)
            "2.0.0" -> ServerNativeLibraryChannel_V2_0_0(nativeLibrary)
            else -> ServerNativeLibraryChannel_V1_0_0(nativeLibrary)
        }
    }
}