package com.bn3monkey.datasourcelibrary.stream


class PipeStreamImpl(val capacity : Int) : PipeStream {

    private val _buffer = ByteArray(capacity)
    private var _readPos = 0
    private var _writePos = 0
    private var _size = 0

    override val available: Int
        get() = _size

    override fun clear() {
        _readPos = 0
        _writePos = 0
        _size = 0
    }


    override fun readSome(dest: ByteArray, offset: Int, size: Int): Int {
        if (_size == 0) return 0

        val toRead = minOf(size, _size)
        var bytesRead = 0

        while (bytesRead < toRead) {
            val spaceUntilEnd = capacity - _readPos
            val bytesToCopy = minOf(spaceUntilEnd, toRead - bytesRead)
            System.arraycopy(_buffer, _readPos, dest, offset + bytesRead, bytesToCopy)
            _readPos = (_readPos + bytesToCopy) % capacity
            bytesRead += bytesToCopy
        }

        _size -= bytesRead
        return bytesRead
    }

    override fun write(source: ByteArray, size : Int) : Int {
        if (_size == capacity) return 0

        val toWrite = minOf(size, capacity - _size)
        var bytesWritten = 0

        while (bytesWritten < toWrite) {
            val spaceUntilEnd = capacity - _writePos
            val bytesToCopy = minOf(spaceUntilEnd, toWrite - bytesWritten)
            System.arraycopy(source, bytesWritten, _buffer, _writePos, bytesToCopy)
            _writePos = (_writePos + bytesToCopy) % capacity
            bytesWritten += bytesToCopy
        }

        _size += bytesWritten
        return bytesWritten
    }

}