//
// Created by hansono_bn3monkey on 2024-10-22.
//
#include "servernativelibrary_v2_0_0.hpp"

#if SERVERLIBRARY_MAJOR_VERSION == 2 && SERVERLIBRARY_MINOR_VERSION == 0 && SERVERLIBRARY_REVISION_VERSION == 0

#include <new>
#include <chrono>
#include <thread>
#include "Log.hpp"

#include "serverlibrarylistener.hpp"
#include "serverlibrarydevicecommunicator.hpp"

#define LOG_TAG "SERVERLIB"
#define LOG_D(...) Bn3Monkey::Log::D(LOG_TAG, __VA_ARGS__);
#define LOG_E(...) Bn3Monkey::Log::E(LOG_TAG, __VA_ARGS__);

const char* ServerLibrary_version()
{
    return SERVERLIBRARY_VERSION_STRING;
}

struct ServerLibraryContext
{
    int32_t data_socket;

    ServerLibraryListener<uint8_t*, int32_t> onDataReceived;
    ServerLibraryListener<void> onDeviceDisconnected;

    ServerLibrary_DeviceCommunicator deviceCommunicator;

    ServerLibraryContext() : data_socket(-1), deviceCommunicator(onDataReceived, onDeviceDisconnected)
    {

    }

    virtual ~ServerLibraryContext() {

    }
};

char context_container[sizeof(struct ServerLibraryContext)];

struct ServerLibraryContext* ServerLibrary_initialize()
{
    LOG_D("%s is called", __FUNCTION__);
    struct ServerLibraryContext* ret = new (context_container) ServerLibraryContext();
    return ret;
}

void ServerLibrary_release(struct ServerLibraryContext* context)
{
    LOG_D("%s is called", __FUNCTION__);
    if (context)
    {
        context->~ServerLibraryContext();
    }
}

bool ServerLibrary_connectDevice(struct ServerLibraryContext* context, int32_t fd)
{
    LOG_D("%s is called | fd : %d", __FUNCTION__, fd);
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    context->device_fd = fd;
    return true;
}

void ServerLibrary_disconnectDevice(struct ServerLibraryContext* context)
{
    LOG_D("%s is called", __FUNCTION__);
    context->device_fd = -1;
}


bool ServerLibrary_isDeviceConnected(struct ServerLibraryContext* context)
{
    LOG_D("%s is called", __FUNCTION__);
    return context->device_fd >= 0;
}


int32_t ServerLibrary_writeToDeviceEEPROM(
        struct ServerLibraryContext* context,
        uint16_t address, const uint8_t* buffer, int32_t offset, uint32_t length)
{
    LOG_D("%s is called | address : %d offset : %d length : %d", __FUNCTION__, address, offset, length);
    LOG_D("%s is called | buffer : %s ", __FUNCTION__, buffer);

    if (address == 1)
    {
        int32_t result = 0;
        for (int i= offset; i< offset + length; i++)
        {
            offset += static_cast<int32_t>(buffer[i]);
        }
        return offset;
    }

    return -1;
}

int32_t ServerLibrary_readFromDeviceEEPROM(
        struct ServerLibraryContext* context,
        uint16_t address, uint8_t* buffer, int32_t offset, uint32_t length)
{

}

int32_t ServerLibrary_burstWrite(
        struct ServerLibraryContext* context,
        uint16_t address, const uint8_t *buf, int32_t offset, uint32_t length);

int32_t ServerLibrary_singleRead(
        struct ServerLibraryContext* context,
        uint16_t address, uint16_t* out_value)
{

}


int32_t ServerLibrary_singleWrite(
        struct ServerLibraryContext *context,
        uint16_t address, uint16_t value)
{

}

uint16_t ServerLibrary_getDeviceType(struct ServerLibraryContext* context);

int32_t ServerLibrary_getDeviceComponentAID(struct ServerLibraryContext* context);

int32_t ServerLibrary_getDeviceComponentBID(struct ServerLibraryContext* context);




// Callback
void ServerLibrary_registerDataCallback(struct ServerLibraryContext* context, void (*callback)(uint8_t* buf, int32_t length))
{
    context->onDataReceived.add(callback);
}
void ServerLibrary_unregisterDataCallback(struct ServerLibraryContext* context)
{
    context->onDataReceived.clear();
}

void ServerLibrary_registerDeviceDisconnectionCallback(struct ServerLibraryContext* context, void (*callback)())
{
    context->onDeviceDisconnected.add(callback);
}
void ServerLibrary_unregisterDeviceDisconnectionCallback(struct ServerLibraryContext* context)
{
    context->onDeviceDisconnected.clear();
}

// Socket
int32_t ServerLibrary_registerDataSocket(struct ServerLibraryContext* context)
{

}
void ServerLibrary_unregisterDataSocket()
{

}
#endif