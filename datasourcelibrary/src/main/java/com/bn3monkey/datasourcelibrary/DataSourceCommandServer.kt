package com.bn3monkey.datasourcelibrary

import android.util.Log
import com.bn3monkey.datasourcelibrary.stream.PipeStream
import com.bn3monkey.datasourcelibrary.stream.PipeStreamFactory
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.cancel
import kotlinx.coroutines.launch
import kotlinx.coroutines.newSingleThreadContext
import kotlinx.coroutines.plus
import kotlinx.coroutines.sync.Mutex
import kotlinx.coroutines.sync.withLock
import kotlinx.coroutines.withContext
import java.net.InetSocketAddress
import java.nio.ByteBuffer
import java.nio.channels.SelectionKey
import java.nio.channels.Selector
import java.nio.channels.ServerSocketChannel
import java.nio.channels.SocketChannel

class DataSourceCommandServer(
    parentScope : CoroutineScope
) {
    private val tag = "DataSourceCommandChannel"

    private val _job = Job()
    private val scope = parentScope + _job

    val mutex = Mutex()
    var isRunning = false

    fun open(nativeChannel : DataSourceNativeLibraryChannel) {
        val selector = Selector.open()
        val serverChannel = ServerSocketChannel.open().apply {
            bind(InetSocketAddress(port))
            configureBlocking(false)
            register(selector, SelectionKey.OP_ACCEPT)

            Log.d(tag, "Command Channel is listening on port $port")
        }


        isRunning = true

        scope.launch(newSingleThreadContext("HsnCC")) {
            mutex.withLock {
                val buffer = ByteBuffer.allocate(512)
                val pipeStream = PipeStreamFactory.create()

                while (isRunning) {
                    selector.select()

                    for (selectedKey in selector.selectedKeys())
                    {
                        if (selectedKey.isValid)
                        {
                            when {
                                selectedKey.isAcceptable -> {
                                    handleAccept(selectedKey, selector)
                                }
                                selectedKey.isReadable -> {
                                    handleRead(selectedKey, buffer, nativeChannel, pipeStream)
                                }
                            }
                        }
                    }
                }

                serverChannel.close()
            }
        }
    }

    suspend fun handleAccept(key : SelectionKey, selector : Selector)
    {
        val serverChannel = key.channel() as ServerSocketChannel
        val clientChannel = withContext(Dispatchers.IO) {
            serverChannel.accept()
        }?.apply {
            configureBlocking(false)
            register(selector, SelectionKey.OP_READ)
            Log.d(tag, "Command Channel connects with client (${this.remoteAddress})")
        }
    }
    suspend fun handleRead(key : SelectionKey, buffer : ByteBuffer,
                           nativeChannel : DataSourceNativeLibraryChannel,
                           pipeStream : PipeStream
    )
    {
        val clientChannel = key.channel() as SocketChannel
        buffer.clear()

        val bytesRead = clientChannel.read(buffer)
        Log.d(tag, "Command Channel read ${bytesRead} bytes")

        pipeStream.write(buffer.array(), bytesRead)

        while (pipeStream.available >= nativeChannel.commandSize)
        {
            Log.d(tag, "Pipe Stream previous length : ${pipeStream.available}")
            val commandBuffer = ByteArray(nativeChannel.commandSize)
            Log.d(tag, "Pipe Stream next length : ${pipeStream.available}")

            pipeStream.readSome(commandBuffer, 0, nativeChannel.commandSize)

            val result = nativeChannel.run(commandBuffer)
            val writtenByteArray = ByteBuffer.wrap(result)
            clientChannel.write(writtenByteArray)

        }
    }

    fun close() {
        scope.launch {
            mutex.withLock {
                isRunning = false
            }
        }
        scope.cancel("Command Channel is closed")
    }

    val port : Int = 20158
}