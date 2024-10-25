package com.bn3monkey.datasourcelibrary

import java.io.InputStream
import java.io.OutputStream
import java.lang.invoke.WrongMethodTypeException
import java.net.Socket
import java.nio.ByteBuffer

class DataSourceCommandClient {
    fun open(port : Int) {
        socket = Socket("127.0.0.1", port)
        inputStream = socket.getInputStream()
        outputStream = socket.getOutputStream()
    }

    fun close() {
        socket.close()
    }

    fun connectDevice(fd : Int) : Boolean {
        val functionName = "connectDevice"
        val command = createCommand(functionName, fd)
        outputStream.write(command)

        val result = ByteArray(128)
        inputStream.read(result)
        return getBooleanResult(result, functionName)
    }

    fun disconnectDevice() {
        val functionName = "disconnectDevice"
        val command = createCommand(functionName)
        outputStream.write(command)

        val result = ByteArray(128)
        inputStream.read(result)
    }

    fun isDeviceConnected(): Boolean {
        val functionName = "isDeviceConnected"
        val command = createCommand(functionName)
        outputStream.write(command)

        val result = ByteArray(128)
        inputStream.read(result)
        return getBooleanResult(result, functionName)
    }

    fun writeToDeviceEEPROM(address: Int, bufferAddress: Long, offset: Int, length: Int): Int {
        val functionName = "writeToDeviceEEPROM"
        val command = createCommand(functionName, address, bufferAddress, offset, length)
        outputStream.write(command)

        val result = ByteArray(128)
        inputStream.read(result)
        return getIntResult(result, functionName)
    }

    fun readFromDeviceEEPROM(address: Int, bufferAddress: Long, offset: Int, length: Int): Int {
        val functionName = "readFromDeviceEEPROM"
        val command = createCommand(functionName, address, bufferAddress, offset, length)
        outputStream.write(command)

        val result = ByteArray(128)
        inputStream.read(result)
        return getIntResult(result, functionName)// 읽은 데이터를 반환
    }

    fun burstWrite(address: Int, bufferAddress: Long, offset: Int, length: Int): Int {
        val functionName = "burstWrite"
        val command = createCommand(functionName, address, bufferAddress, offset, length)
        outputStream.write(command)

        val result = ByteArray(128)
        inputStream.read(result)
        return getIntResult(result, functionName)
    }

    fun singleRead(address: Int, valueAddress : Long): Int {
        val functionName = "singleRead"
        val command = createCommand(functionName, address, valueAddress)
        outputStream.write(command)

        val result = ByteArray(128)
        inputStream.read(result)
        return getIntResult(result, functionName)
    }

    fun singleWrite(address: Int, value: Int) : Int {
        val functionName = "singleWrite"
        val command = createCommand(functionName, address, value)
        outputStream.write(command)

        val result = ByteArray(128)
        inputStream.read(result)
        return getIntResult(result, functionName)
    }

    fun getDeviceID(): Int {
        val functionName = "deviceID"
        val command = createCommand(functionName)
        outputStream.write(command)

        val result = ByteArray(128)
        inputStream.read(result)
        return getIntResult(result, functionName)
    }

    fun getDeviceComponentAID(): Int {
        val functionName = "deviceComponentAID"
        val command = createCommand(functionName)
        outputStream.write(command)

        val result = ByteArray(128)
        inputStream.read(result)
        return getIntResult(result, functionName)
    }

    fun getDeviceComponentBID(): Int {
        val functionName = "deviceComponentBID"
        val command = createCommand(functionName)
        outputStream.write(command)

        val result = ByteArray(128)
        inputStream.read(result)
        return getIntResult(result, functionName)
    }

    private lateinit var socket : Socket
    private lateinit var inputStream : InputStream
    private lateinit var outputStream : OutputStream

    private fun createCommand(name : String, param1 : Any? = null, param2 : Any? = null, param3 : Any? = null, param4 : Any? = null) : ByteArray
    {
        fun setParameter(buffer : ByteBuffer, index : Int, param : Any)
        {
            buffer.position(96 + 8*index)
            when (param)
            {
                is Unit -> { /* Do Nothing */}
                is Boolean -> buffer.put(if (param) 1 else 0)
                is Int -> buffer.putInt(param)
                is Long -> buffer.putLong(param)
                is Double -> buffer.putDouble(param)
                is String -> buffer.put(param.encodeToByteArray())
                else -> throw IllegalArgumentException("Unsupported type: ${param::class}")
            }
        }

        val buffer = ByteBuffer.allocate(128)
        buffer.put(name.encodeToByteArray())
        param1?.let { param ->
            setParameter(buffer, 0, param)
        }
        param2?.let { param ->
            setParameter(buffer, 1, param)
        }
        param3?.let { param ->
            setParameter(buffer, 2, param)
        }
        param4?.let { param ->
            setParameter(buffer, 3, param)
        }
        return buffer.array()
    }

    private fun getResultBuffer(result : ByteArray, name : String) : ByteBuffer {
        val buffer = ByteBuffer.wrap(result)
        val resultName = buffer.array().decodeToString(0, 96).substringBefore('\u0000')
        if (name != resultName)
        {
            throw WrongMethodTypeException("Result name does not match with command name ($name, $resultName)")
        }
        buffer.position(96)
        return buffer
    }

    private fun getBooleanResult(result : ByteArray, name : String) : Boolean
    {
        val buffer = getResultBuffer(result, name)
        return buffer.getChar() == 1.toChar()
    }
    private fun getIntResult(result : ByteArray, name : String) : Int
    {
        val buffer = getResultBuffer(result, name)
        return buffer.getInt()
    }
    private fun getLongResult(result : ByteArray, name : String) : Long
    {
        val buffer = getResultBuffer(result, name)
        return buffer.getLong()
    }
    private fun getDoubleResult(result : ByteArray, name : String) : Double
    {
        val buffer = getResultBuffer(result, name)
        return buffer.getDouble()
    }
    private fun getStringResult(result : ByteArray, name : String) : String
    {
        val buffer = getResultBuffer(result, name)
        return buffer.array().decodeToString().substringBefore('\u0000')
    }
}