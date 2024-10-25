//
// Created by hansono_bn3monkey on 2024-10-22.
//



#ifndef ANDROIDNATIVELIBRARYCHANNEL_DATASOURCENATIVELIBRARY_V2_0_0_HPP
#define ANDROIDNATIVELIBRARYCHANNEL_DATASOURCENATIVELIBRARY_V2_0_0_HPP
#include <cstdint>

#include "datasourcelibraryversion.hpp"
#if DATASOURCELIBRARY_MAJOR_VERSION == 2 && DATASOURCELIBRARY_MINOR_VERSION == 0 && DATASOURCELIBRARY_REVISION_VERSION == 0

#ifdef __cplusplus
extern "C" {
#endif

const char *DataSourceLibrary_version();

struct DataSourceLibraryContext;


// Should be called in an application

struct DataSourceLibraryContext *DataSourceLibrary_initialize();
void DataSourceLibrary_release(struct DataSourceLibraryContext *context);

bool DataSourceLibrary_connectDevice(struct DataSourceLibraryContext *context, int32_t fd);
void DataSourceLibrary_disconnectDevice(struct DataSourceLibraryContext *context);


// Should be called in other library

bool DataSourceLibrary_isDeviceConnected(struct DataSourceLibraryContext *context);

int32_t DataSourceLibrary_writeToDeviceEEPROM(
        struct DataSourceLibraryContext *context,
        uint16_t address, const uint8_t *buffer, int32_t offset, uint32_t length);

int32_t DataSourceLibrary_readFromDeviceEEPROM(
        struct DataSourceLibraryContext *context,
        uint16_t address, uint8_t *buffer, int32_t offset, uint32_t length);

int32_t DataSourceLibrary_burstWrite(
        struct DataSourceLibraryContext *context,
        uint16_t address, const uint8_t *buffer, int32_t offset, uint32_t length);

int32_t DataSourceLibrary_singleRead(
        struct DataSourceLibraryContext *context,
        uint16_t address, uint16_t *out_value);

int32_t DataSourceLibrary_singleWrite(
        struct DataSourceLibraryContext *context,
        uint16_t address, uint16_t value);

int32_t DataSourceLibrary_getDeviceID(struct DataSourceLibraryContext *context);

int32_t DataSourceLibrary_getDeviceComponentAID(struct DataSourceLibraryContext *context);

int32_t DataSourceLibrary_getDeviceComponentBID(struct DataSourceLibraryContext *context);


// Callback
void DataSourceLibrary_registerDataCallback(struct DataSourceLibraryContext *context,
                                            void* user_context,
                                            void (*callback)(void* user_context, int8_t *buf, int32_t length),
                                            void (*releaseUserContext)(void* user_context)
                                        );
void DataSourceLibrary_unregisterDataCallback(struct DataSourceLibraryContext *context);

void DataSourceLibrary_registerDeviceDisconnectionCallback(struct DataSourceLibraryContext *context,
       void* user_context,
       void (*callback)(void* user_context),
       void (*releaseUserContext)(void* user_context));
void DataSourceLibrary_unregisterDeviceDisconnectionCallback(struct DataSourceLibraryContext *context);

void DataSourceLibrary_registerDeviceStateCallback(struct DataSourceLibraryContext* context,
        void* user_context,
        void (*callback)(void* user_context, int32_t),
        void (*releaseUserContext)(void* user_context));
void DataSourceLibrary_unregisterDeviceStateCallback(struct DataSourceLibraryContext* context);

#ifdef __cplusplus
}
#endif

#endif // VERSION_CHECK

#endif //ANDROIDNATIVELIBRARYCHANNEL_SERVERNATIVELIBRARY_V2_0_0_HPP
