package com.bn3monkey.datasourcelibrary.nativechannel

import com.bn3monkey.datasourcelibrary.DataSourceNativeLibrary
import com.bn3monkey.datasourcelibrary.DataSourceNativeLibraryChannel
import java.nio.ByteBuffer

class DataSourceNativeLibraryChannel_V2_0_0(private val nativeLibrary : DataSourceNativeLibrary) :
    DataSourceNativeLibraryChannel
{
    data class DataSourceNativeChannelCommand(private val command : ByteArray)
    {
        companion object {
            val size = 128
            val nameSize = 96
        }

        init {
            assert(command.size >= size)
        }

        // need to be fixed
        val name : String
            get() = command.decodeToString(0, nameSize).substringBefore('\u0000')

        private fun getParam(index : Int) : ByteBuffer {
            val startIndex = nameSize + 8*index
            val endIndex = nameSize + 8*(index + 1)

            val extractedBytes = command.sliceArray(startIndex..<endIndex)
            return ByteBuffer.wrap(extractedBytes)
        }

        fun getInt(index : Int) : Int {
            return getParam(index).getInt()
        }
        fun getShort(index : Int) : Short {
            return getParam(index).getShort()
        }
        fun getLong(index : Int) : Long {
            return getParam(index).getLong()
        }
        fun getFloat(index : Int) : Float {
            return getParam(index).getFloat()
        }
        fun getDouble(index : Int) : Double {
            return getParam(index).getDouble()
        }
        fun getString(index : Int) : String {
            return getParam(index).array().decodeToString().substringBefore('\u0000')
        }

    }

    class DataSourceNativeChannelResult(private val name : String, private val value : Any)
    {
        fun serialize() : ByteArray
        {
            val buffer = ByteBuffer.allocate(128)
            buffer.put(name.encodeToByteArray())
            buffer.position(96)
            when(value)
            {
                is Unit -> { /* Do Nothing */}
                is Boolean -> buffer.put(if (value) 1 else 0)
                is Int -> buffer.putInt(value)
                is Long -> buffer.putLong(value)
                is Double -> buffer.putDouble(value)
                is String -> buffer.put(value.encodeToByteArray())
                else -> throw IllegalArgumentException("Unsupported type: ${value::class}")
            }
            return buffer.array()
        }
    }


    override fun run(
        command: ByteArray
    ): ByteArray {

        val parsedCommand = DataSourceNativeChannelCommand(command)
        val commandName = parsedCommand.name
        val res : Any = when(commandName)
        {
            "version" -> nativeLibrary.getVersion();
            "connectDevice" -> {
                val fd = parsedCommand.getInt(0)
                nativeLibrary.connectDevice(fd)
            }
            "disconnectDevice" -> {
                nativeLibrary.disconnectDevice()
            }
            "isDeviceConnected" -> nativeLibrary.isDeviceConnected()

            "writeToDeviceEEPROM" -> {
                val address = parsedCommand.getInt(0)
                val buffer = parsedCommand.getLong(1)
                val offset = parsedCommand.getInt(2)
                val length = parsedCommand.getInt(3)
                nativeLibrary.writeToDeviceEEPROM(address, buffer, offset, length)
            }

            "readFromDeviceEEPROM" -> {
                val address = parsedCommand.getInt(0)
                val buffer = parsedCommand.getLong(1)
                val offset = parsedCommand.getInt(2)
                val length = parsedCommand.getInt(3)
                nativeLibrary.readFromDeviceEEPROM(address, buffer, offset, length)
            }

            "burstWrite" -> {
                val address = parsedCommand.getInt(0)
                val buffer = parsedCommand.getLong(1)
                val offset = parsedCommand.getInt(2)
                val length = parsedCommand.getInt(3)
                nativeLibrary.burstWrite(address, buffer, offset, length)
            }

            "singleRead" -> {
                val address = parsedCommand.getInt(0)
                val outValue = parsedCommand.getLong(1)
                nativeLibrary.singleRead(address, outValue)
            }

            "singleWrite" -> {
                val address = parsedCommand.getInt(0)
                val value = parsedCommand.getInt(1)
                nativeLibrary.singleWrite(address, value)
            }

            "deviceID" -> nativeLibrary.deviceID

            "deviceComponentAID" -> nativeLibrary.deviceComponentAID

            "deviceComponentBID" -> nativeLibrary.deviceComponentBID

            else -> {}
        }

        val parsedResult = DataSourceNativeChannelResult(commandName, res)
        return parsedResult.serialize()
    }

    override val commandSize : Int
        get() {
            return DataSourceNativeChannelCommand.size
        }
}