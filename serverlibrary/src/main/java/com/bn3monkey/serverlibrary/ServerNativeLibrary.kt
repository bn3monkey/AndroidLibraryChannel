package com.bn3monkey.serverlibrary

class ServerNativeLibrary {

    fun getVersion() : String

    companion object {
        // Used to load the 'serverlibrary' library on application startup.
        init {
            System.loadLibrary("serverlibrary")
        }
    }
}