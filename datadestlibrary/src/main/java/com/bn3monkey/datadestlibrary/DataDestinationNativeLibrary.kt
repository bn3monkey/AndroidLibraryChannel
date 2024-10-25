package com.bn3monkey.datadestlibrary

class DataDestinationNativeLibrary {

    /**
     * A native method that is implemented by the 'datadestlibrary' native library,
     * which is packaged with this application.
     */

    external fun open(sock : Int) : Boolean
    external fun close(sock : Int) : Boolean

    companion object {
        // Used to load the 'datadestlibrary' library on application startup.
        init {
            System.loadLibrary("datadestlibrary")
        }
    }
}