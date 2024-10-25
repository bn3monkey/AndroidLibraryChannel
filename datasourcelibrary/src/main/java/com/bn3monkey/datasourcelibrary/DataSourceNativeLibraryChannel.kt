package com.bn3monkey.datasourcelibrary

import com.bn3monkey.datasourcelibrary.nativechannel.DataSourceNativeLibraryChannel_V2_0_0

interface DataSourceNativeLibraryChannel {
    fun run(command : ByteArray) : ByteArray
    val commandSize : Int
}

object DataSourceNativelibraryChannelFactory
{
    fun create(nativeLibrary: DataSourceNativeLibrary) : DataSourceNativeLibraryChannel
    {
        return when(nativeLibrary.getVersion())
        {
            else -> DataSourceNativeLibraryChannel_V2_0_0(nativeLibrary)
        }
    }
}
