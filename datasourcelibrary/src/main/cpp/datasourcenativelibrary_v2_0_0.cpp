//
// Created by hansono_bn3monkey on 2024-10-22.
//
#include "datasourcenativelibrary_v2_0_0.hpp"

#if DATASOURCELIBRARY_MAJOR_VERSION == 2 && DATASOURCELIBRARY_MINOR_VERSION == 0 && DATASOURCELIBRARY_REVISION_VERSION == 0

#include <new>
#include <chrono>
#include <thread>
#include "Log.hpp"

#include "datasourcelibrarylistener.hpp"
#include "datasourcelibrarydevicecommunicator.hpp"
#include "datasourcelibrarydataclient.h"

#define LOG_TAG "SERVERLIB"
#define LOG_D(...) Bn3Monkey::Log::D(LOG_TAG, __VA_ARGS__);
#define LOG_E(...) Bn3Monkey::Log::E(LOG_TAG, __VA_ARGS__);

const char* DataSourceLibrary_version()
{
    return DATASOURCELIBRARY_VERSION_STRING;
}

struct DataSourceLibraryContext
{
    int32_t data_socket;

    DataSourceLibraryListener<int32_t> onDeviceStateChanged;
    DataSourceLibraryListener<int8_t*, int32_t> onDataReceived;
    DataSourceLibraryListener<void> onDeviceDisconnected;

    DataSourceLibrary_DeviceCommunicator deviceCommunicator;

    DataSourceLibraryContext() : data_socket(-1), deviceCommunicator(onDeviceStateChanged, onDataReceived, onDeviceDisconnected)
    {

    }

    virtual ~DataSourceLibraryContext() {

    }
};

char context_container[sizeof(struct DataSourceLibraryContext)];

struct DataSourceLibraryContext* DataSourceLibrary_initialize()
{
    LOG_D("%s is called", __FUNCTION__);
    struct DataSourceLibraryContext* ret = new (context_container) DataSourceLibraryContext();
    return ret;
}

void DataSourceLibrary_release(struct DataSourceLibraryContext* context)
{
    LOG_D("%s is called", __FUNCTION__);
    if (context)
    {
        context->~DataSourceLibraryContext();
    }
}

bool DataSourceLibrary_connectDevice(struct DataSourceLibraryContext* context, int32_t fd)
{
    LOG_D("%s is called | fd : %d", __FUNCTION__, fd);
    return context->deviceCommunicator.connect(fd);
}

void DataSourceLibrary_disconnectDevice(struct DataSourceLibraryContext* context)
{
    LOG_D("%s is called", __FUNCTION__);
    context->deviceCommunicator.disconnect();
}


bool DataSourceLibrary_isDeviceConnected(struct DataSourceLibraryContext* context)
{
    LOG_D("%s is called", __FUNCTION__);
    return context->deviceCommunicator.isActivated();
}


int32_t DataSourceLibrary_writeToDeviceEEPROM(
        struct DataSourceLibraryContext* context,
        uint16_t address, const uint8_t* buffer, int32_t offset, uint32_t length)
{
    LOG_D("%s is called | address : %d buffer : %p offset : %d length : %d", __FUNCTION__, address, buffer, offset, length);
    return context->deviceCommunicator.writeToDeviceEEPROM(address, buffer, offset, length);
}

int32_t DataSourceLibrary_readFromDeviceEEPROM(
        struct DataSourceLibraryContext* context,
        uint16_t address, uint8_t* buffer, int32_t offset, uint32_t length)
{
    LOG_D("%s is called | address : %d buffer : %p offset : %d length : %d", __FUNCTION__, address, buffer, offset, length);
    return context->deviceCommunicator.readFromDeviceEEPROM(address, buffer, offset, length);
}

int32_t DataSourceLibrary_burstWrite(
        struct DataSourceLibraryContext* context,
        uint16_t address, const uint8_t *buffer, int32_t offset, uint32_t length)
{
    LOG_D("%s is called | address : %d buffer : %p offset : %d length : %d", __FUNCTION__, address, buffer, offset, length);
    return context->deviceCommunicator.burstWrite(address, buffer, offset, length);
}

int32_t DataSourceLibrary_singleRead(
        struct DataSourceLibraryContext* context,
        uint16_t address, uint16_t* out_value)
{
    LOG_D("%s is called | address : %d value : %p", __FUNCTION__, address, out_value);
    return context->deviceCommunicator.singleRead(address, out_value);
}


int32_t DataSourceLibrary_singleWrite(
        struct DataSourceLibraryContext *context,
        uint16_t address, uint16_t value)
{
    LOG_D("%s is called | address : %d value : %p", __FUNCTION__, address, value);
    return context->deviceCommunicator.singleWrite(address, value);
}

int32_t DataSourceLibrary_getDeviceID(struct DataSourceLibraryContext* context)
{
    LOG_D("%s is called", __FUNCTION__);
    return context->deviceCommunicator.getDeviceID();
}

int32_t DataSourceLibrary_getDeviceComponentAID(struct DataSourceLibraryContext* context)
{
    LOG_D("%s is called", __FUNCTION__);
    return context->deviceCommunicator.getDeviceComponentAID();
}

int32_t DataSourceLibrary_getDeviceComponentBID(struct DataSourceLibraryContext* context)
{
    LOG_D("%s is called", __FUNCTION__);
    return context->deviceCommunicator.getDeviceComponentBID();
}


// Callback
void DataSourceLibrary_registerDataCallback(struct DataSourceLibraryContext* context,
        void* user_context,
        void (*callback)(void* user_context, int8_t* buf, int32_t length),
        void (*releaseUserContext)(void* user_context))
{
    context->onDataReceived.add(user_context, callback, releaseUserContext);
}
void DataSourceLibrary_unregisterDataCallback(struct DataSourceLibraryContext* context)
{
    context->onDataReceived.clear();
}

void DataSourceLibrary_registerDeviceDisconnectionCallback(struct DataSourceLibraryContext* context,
       void* user_context,
        void (*callback)(void* user_context),
       void (*releaseUserContext)(void* user_context))
{
    context->onDeviceDisconnected.add(user_context, callback, releaseUserContext);
}
void DataSourceLibrary_unregisterDeviceDisconnectionCallback(struct DataSourceLibraryContext* context)
{
    context->onDeviceDisconnected.clear();
}
void DataSourceLibrary_registerDeviceStateCallback(struct DataSourceLibraryContext *context,
                                                   void *user_context, void (*callback)(void* user_context, int32_t),
                                                   void (*releaseUserContext)(void* user_context)) {
    context->onDeviceStateChanged.add(user_context, callback, releaseUserContext);
}

void
DataSourceLibrary_unregisterDeviceStateCallback(struct DataSourceLibraryContext *context) {
    context->onDeviceStateChanged.clear();
}



#endif