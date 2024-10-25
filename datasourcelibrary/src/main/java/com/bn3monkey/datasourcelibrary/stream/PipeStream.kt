package com.bn3monkey.datasourcelibrary.stream

interface PipeStream {
    fun clear()
    fun readSome(dest : ByteArray, offset : Int, size : Int) : Int
    fun readSome(dest : ByteArray, size : Int) : Int = readSome(dest, 0, size)

    fun write(source : ByteArray, size : Int) : Int
    fun write(source : ByteArray) : Int = write(source, source.size)

    val available : Int
}

object PipeStreamFactory
{
    fun create() : PipeStream
    {
        return PipeStreamImpl(128*16)
    }
}