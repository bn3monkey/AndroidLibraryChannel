package com.bn3monkey.datasourcelibrary

import android.util.Log
import androidx.test.ext.junit.runners.AndroidJUnit4
import org.junit.Test
import org.junit.runner.RunWith
import kotlinx.coroutines.*
import org.junit.Assert.assertEquals

@RunWith(AndroidJUnit4::class)
class NativeLibraryTest {
    private val logTag = "NativeLibraryTest"
    fun debugMsg(msg : String) {
        Log.d(logTag, msg)
        print(msg)
    }
    fun errorMsg(msg : String) {
        Log.e(logTag, msg)
        print(msg)
    }
    // val appContext = InstrumentationRegistry.getInstrumentation().targetContext

    @Test
    fun runLibrary()
    {
        val dataSourceLib = DataSourceNativeLibrary()

        val waitObject = Object()

        dataSourceLib.registerDeviceDisconnectionCallback {
            debugMsg("onDataReceived")
        }
        dataSourceLib.registerDeviceStateCallback {
            state ->
            when (state) {
                0 -> {
                    debugMsg("onDeviceStateChanged : DISCONNECTED")
                    synchronized(waitObject) {
                        waitObject.notifyAll()
                    }
                }
                1 -> {
                    debugMsg("onDeviceStateChanged : CONNECTED")
                }
                2 -> {
                    debugMsg("onDeviceStateChanged : ACTIVATED")

                }
                3 -> {
                    debugMsg("onDeviceStateChanged : STREAMING")
                    synchronized(waitObject) {
                        waitObject.notifyAll()
                    }
                }
                else -> {}
            }
        }

        for (fd in -1..7) {
            dataSourceLib.connectDevice(fd);
            synchronized(waitObject) {
                waitObject.wait()
            }

            val deviceID = dataSourceLib.deviceID
            val deviceComponentAID = dataSourceLib.deviceComponentAID
            val deviceComponentBID = dataSourceLib.deviceComponentBID
            debugMsg("Current FD : $fd device ID : $deviceID")
            debugMsg("deviceComponentAID : $deviceComponentAID deviceComponentB : $deviceComponentBID")

            when (fd) {
                0 -> {
                    assertEquals(1, deviceID)
                    assertEquals(1, deviceComponentAID)
                    assertEquals(1, deviceComponentBID)
                }
                1 -> {
                    assertEquals(2, deviceID)
                    assertEquals(1, deviceComponentAID)
                    assertEquals(1, deviceComponentBID)

                }
                2 -> {
                    assertEquals(1, deviceID)
                    assertEquals(2, deviceComponentAID)
                    assertEquals(1, deviceComponentBID)
                }
                3 -> {
                    assertEquals(2, deviceID)
                    assertEquals(2, deviceComponentAID)
                    assertEquals(1, deviceComponentBID)
                }
                4 -> {
                    assertEquals(1, deviceID)
                    assertEquals(1, deviceComponentAID)
                    assertEquals(2, deviceComponentBID)

                }
                5 -> {
                    assertEquals(2, deviceID)
                    assertEquals(1, deviceComponentAID)
                    assertEquals(2, deviceComponentBID)
                }
                6 -> {
                    assertEquals(1, deviceID)
                    assertEquals(2, deviceComponentAID)
                    assertEquals(2, deviceComponentBID)

                }
                7 -> {
                    assertEquals(2, deviceID)
                    assertEquals(2, deviceComponentAID)
                    assertEquals(2, deviceComponentBID)
                }
                else -> {
                    assertEquals(-1, deviceID)
                    assertEquals(-1, deviceComponentAID)
                    assertEquals(-1, deviceComponentBID)
                }
            }
            dataSourceLib.disconnectDevice()
        }

        dataSourceLib.connectDevice(1)
        synchronized(waitObject) {
            waitObject.wait()
        }

        dataSourceLib.registerDataCallback {
                data, length ->
            val string = data.decodeToString(0, length)
            debugMsg("onDataReceived : $string")
        }

        ByteArray(1024).let {
            byteArray ->
            byteArray[0] = 1
            byteArray[1] = 2
            byteArray[2] = 3
            byteArray[3] = 4

            dataSourceLib.writeToDeviceEEPROM(
                1,
                byteArray,
                0,
                4
            ).let { result ->
                assertEquals(0, result)
            }

            byteArray[0] = 0
            byteArray[1] = 0
            byteArray[2] = 0
            byteArray[3] = 0
            dataSourceLib.readFromDeviceEEPROM(
                1,
                byteArray,
                0,
                4
            ).let { result ->
                assertEquals(0, result)
                assertEquals(1.toByte(), byteArray[0])
                assertEquals(2.toByte(), byteArray[1])
                assertEquals(3.toByte(), byteArray[2])
                assertEquals(4.toByte(), byteArray[3])
            }
        }

        ByteArray(1024).let {
            byteArray ->
            byteArray[0] = 2
            byteArray[1] = 3
            byteArray[2] = 4
            byteArray[3] = 5

            dataSourceLib.burstWrite(
                2,
                byteArray,
                0,
                4
            ).let { result ->
                assertEquals(0, result)
            }
        }

        IntArray(1).let{
            value ->
            val inputValue = 3
            dataSourceLib.singleWrite(3, inputValue).let {
                result ->
                assertEquals(0, result)
            }
            dataSourceLib.singleRead(3, value).let {
                result ->
                assertEquals(0, result)
                assertEquals(inputValue, value[0])
            }
        }

        Thread.sleep(5000)

        dataSourceLib.unregisterDataCallback()

        Thread.sleep(3000)
        dataSourceLib.disconnectDevice()

        dataSourceLib.unregisterDeviceDisconnectionCallback()
        dataSourceLib.unregisterDeviceStateCallback()
        dataSourceLib.dispose()
    }

    @Test
    fun runLibraryWithSocket()
    {
        val defaultCoroutineScope = CoroutineScope(Dispatchers.Main)

        val dataSourceLib = DataSourceNativeLibrary()
        val server = DataSourceCommandServer(defaultCoroutineScope)

        val waitObject = Object()
        dataSourceLib.registerDeviceStateCallback {
                state ->
            when (state) {
                0 -> {
                    debugMsg("onDeviceStateChanged : DISCONNECTED")
                    synchronized(waitObject) {
                        waitObject.notifyAll()
                    }
                }
                1 -> {
                    debugMsg("onDeviceStateChanged : CONNECTED")
                }
                2 -> {
                    debugMsg("onDeviceStateChanged : ACTIVATED")

                }
                3 -> {
                    debugMsg("onDeviceStateChanged : STREAMING")
                    synchronized(waitObject) {
                        waitObject.notifyAll()
                    }
                }
                else -> {}
            }
        }

        server.open(DataSourceNativelibraryChannelFactory.create(dataSourceLib))

        val client = DataSourceCommandClient()
        client.open(server.port)

        for (fd in -1..7) {
            client.connectDevice(fd);
            synchronized(waitObject) {
                waitObject.wait()
            }

            val deviceID = client.getDeviceID()
            val deviceComponentAID = client.getDeviceComponentAID()
            val deviceComponentBID = client.getDeviceComponentBID()
            debugMsg("Current FD : $fd device ID : $deviceID")
            debugMsg("deviceComponentAID : $deviceComponentAID deviceComponentB : $deviceComponentBID")

            when (fd) {
                0 -> {
                    assertEquals(1, deviceID)
                    assertEquals(1, deviceComponentAID)
                    assertEquals(1, deviceComponentBID)
                }
                1 -> {
                    assertEquals(2, deviceID)
                    assertEquals(1, deviceComponentAID)
                    assertEquals(1, deviceComponentBID)

                }
                2 -> {
                    assertEquals(1, deviceID)
                    assertEquals(2, deviceComponentAID)
                    assertEquals(1, deviceComponentBID)
                }
                3 -> {
                    assertEquals(2, deviceID)
                    assertEquals(2, deviceComponentAID)
                    assertEquals(1, deviceComponentBID)
                }
                4 -> {
                    assertEquals(1, deviceID)
                    assertEquals(1, deviceComponentAID)
                    assertEquals(2, deviceComponentBID)

                }
                5 -> {
                    assertEquals(2, deviceID)
                    assertEquals(1, deviceComponentAID)
                    assertEquals(2, deviceComponentBID)
                }
                6 -> {
                    assertEquals(1, deviceID)
                    assertEquals(2, deviceComponentAID)
                    assertEquals(2, deviceComponentBID)

                }
                7 -> {
                    assertEquals(2, deviceID)
                    assertEquals(2, deviceComponentAID)
                    assertEquals(2, deviceComponentBID)
                }
                else -> {
                    assertEquals(-1, deviceID)
                    assertEquals(-1, deviceComponentAID)
                    assertEquals(-1, deviceComponentBID)
                }
            }
            client.disconnectDevice()
        }

        client.connectDevice(1)
        synchronized(waitObject) {
            waitObject.wait()
        }

        dataSourceLib.registerDataCallback {
                data, length ->
            val string = data.decodeToString(0, length)
            debugMsg("onDataReceived : $string")
        }

        ByteArray(1024).let {
                byteArray ->
            byteArray[0] = 1
            byteArray[1] = 2
            byteArray[2] = 3
            byteArray[3] = 4

            NativeAddress().getByteArrayAddress(byteArray).let {
                address ->
                client.writeToDeviceEEPROM(
                    1,
                    address,
                    0,
                    4
                ).let { result ->
                    assertEquals(0, result)
                    NativeAddress().releaseByteArrayAddress(byteArray, address)
                }
            }

            byteArray[0] = 0
            byteArray[1] = 0
            byteArray[2] = 0
            byteArray[3] = 0

            NativeAddress().getByteArrayAddress(byteArray).let { address ->
                client.readFromDeviceEEPROM(
                    1,
                    address,
                    0,
                    4
                ).let { result ->
                    assertEquals(0, result)
                    NativeAddress().releaseByteArrayAddress(byteArray, address)
                    assertEquals(1.toByte(), byteArray[0])
                    assertEquals(2.toByte(), byteArray[1])
                    assertEquals(3.toByte(), byteArray[2])
                    assertEquals(4.toByte(), byteArray[3])
                }
            }
        }

        ByteArray(1024).let {
                byteArray ->
            byteArray[0] = 2
            byteArray[1] = 3
            byteArray[2] = 4
            byteArray[3] = 5

            NativeAddress().getByteArrayAddress(byteArray).let { address ->
                client.burstWrite(
                    2,
                    address,
                    0,
                    4
                ).let { result ->
                    assertEquals(0, result)
                    NativeAddress().releaseByteArrayAddress(byteArray, address)
                }
            }
        }

        IntArray(1).let{
                value ->
            val inputValue = 3
            value[0] = 0

            client.singleWrite(3, inputValue).let {
                    result ->
                assertEquals(0, result)
            }

            NativeAddress().getIntArrayAddress(value).let {
                address ->
                client.singleRead(3, address).let { result ->
                    assertEquals(0, result)
                    NativeAddress().releaseIntArrayAddress(value, address)
                    assertEquals(inputValue, value[0])
                }
            }
        }

        Thread.sleep(5000)

        dataSourceLib.unregisterDataCallback()

        Thread.sleep(3000)
        client.disconnectDevice()

        client.close()
        server.close()
    }
}