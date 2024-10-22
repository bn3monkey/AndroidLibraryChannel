package com.bn3monkey.clientlibrary

class ClientNativeLibrary {

    /**
     * A native method that is implemented by the 'clientlibrary' native library,
     * which is packaged with this application.
     */
    external fun createSocket(port : Int) : Int
    external fun closeSocket(socket : Int)

    external fun openNGS(socket : Int) : Boolean
    external fun closeNGS(socket : Int) : Boolean

    companion object {
        // Used to load the 'clientlibrary' library on application startup.
        init {
            System.loadLibrary("clientlibrary")
        }
    }
}