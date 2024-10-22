package com.bn3monkey.serverlibrary.nativechannel

import com.bn3monkey.serverlibrary.ServerNativeLibrary
import com.bn3monkey.serverlibrary.ServerNativeLibraryChannel
import java.nio.ByteBuffer

class ServerNativeLibraryChannel_V2_0_0(private val nativeLibrary : ServerNativeLibrary) :
    ServerNativeLibraryChannel
{
    data class HansonoUltrasoundCommand_V2_0_0(private val command : ByteArray)
    {
        companion object {
            val size = 128
            val nameSize = 96
        }

        init {
            assert(command.size >= size)
        }
        val name : String
            get() = command.decodeToString(0, nameSize)

        private fun getParam(index : Int) : ByteBuffer {
            val startIndex = nameSize + 8*index
            val endIndex = nameSize + 8*(index + 1)

            val extractedBytes = command.sliceArray(startIndex..<endIndex)
            return ByteBuffer.wrap(extractedBytes)
        }

        fun getInt(index : Int) : Int {
            return getParam(index).getInt()
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
            return getParam(index).array().decodeToString()
        }

    }

    data class HansonoUltrasoundResult_V2_0_0(private val name : String, private val result : String)
    {
        fun serialize() : ByteArray {
            val buffer = ByteArray(128) {0}

            val nameBuffer = name.encodeToByteArray()
            System.arraycopy(nameBuffer, 0, buffer, 0, nameBuffer.size)

            val resultBuffer = result.encodeToByteArray()
            System.arraycopy(resultBuffer, 0, buffer, 32, resultBuffer.size)

            return buffer
        }
    }

    override fun run(
        command: ByteArray
    ): ByteArray {

        val parsedCommand = HansonoUltrasoundCommand_V2_0_0(command)

        val res = when(parsedCommand.name)
        {
            "version" -> "2.0.0"
            "1" -> "1"
            "2" -> "2"
            else -> "0"
        }

        val parsedResult = HansonoUltrasoundResult_V2_0_0(parsedCommand.name, res)
        return parsedResult.serialize()
    }

    override val commandSize : Int
        get() {
            return HansonoUltrasoundCommand_V2_0_0.size
        }
}