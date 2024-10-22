//
// Created by hansono_bn3monkey on 2024-10-22.
//



#ifndef ANDROIDNATIVELIBRARYCHANNEL_SERVERNATIVELIBRARY_V2_0_0_HPP
#define ANDROIDNATIVELIBRARYCHANNEL_SERVERNATIVELIBRARY_V2_0_0_HPP
#include <cstdint>

#include "serverlibraryversion.hpp"
#if SERVERLIBRARY_MAJOR_VERSION == 2 && SERVERLIBRARY_MINOR_VERSION == 0 && SERVERLIBRARY_REVISION_VERSION == 0

#ifdef __cplusplus
extern "C" {
#endif

const char *ServerLibrary_version();

struct ServerLibraryContext;

// Should be called in an application

struct ServerLibraryContext *ServerLibrary_initialize();
void ServerLibrary_release(struct ServerLibraryContext *context);

bool ServerLibrary_connectDevice(struct ServerLibraryContext *context, int32_t fd);
void ServerLibrary_disconnectDevice(struct ServerLibraryContext *context);


// Should be called in other library

bool ServerLibrary_isDeviceConnected(struct ServerLibraryContext *context);

int32_t ServerLibrary_writeToDeviceEEPROM(
        struct ServerLibraryContext *context,
        uint16_t address, const uint8_t *buffer, int32_t offset, uint32_t length);

int32_t ServerLibrary_readFromDeviceEEPROM(
        struct ServerLibraryContext *context,
        uint16_t address, uint8_t *buffer, int32_t offset, uint32_t length);

int32_t ServerLibrary_burstWrite(
        struct ServerLibraryContext *context,
        uint16_t address, const uint8_t *buf, int32_t offset, uint32_t length);

int32_t ServerLibrary_singleRead(
        struct ServerLibraryContext *context,
        uint16_t address, uint16_t *out_value);

int32_t ServerLibrary_singleWrite(
        struct ServerLibraryContext *context,
        uint16_t address, uint16_t value);

uint16_t ServerLibrary_getDeviceType(struct ServerLibraryContext *context);

int32_t ServerLibrary_getDeviceComponentAID(struct ServerLibraryContext *context);

int32_t ServerLibrary_getDeviceComponentBID(struct ServerLibraryContext *context);


// Callback
void ServerLibrary_registerDataCallback(struct ServerLibraryContext *context,
                                        void (*callback)(uint8_t *buf, int32_t length));
void ServerLibrary_unregisterDataCallback(struct ServerLibraryContext *context);

void ServerLibrary_registerDeviceDisconnectionCallback(struct ServerLibraryContext *context, void (*callback)());
void ServerLibrary_unregisterDeviceDisconnectionCallback(struct ServerLibraryContext *context);

// Socket
int32_t ServerLibrary_registerDataSocket(struct ServerLibraryContext *context);
void ServerLibrary_unregisterDataSocket();

#ifdef __cplusplus
}
#endif

#endif // VERSION_CHECK

#endif //ANDROIDNATIVELIBRARYCHANNEL_SERVERNATIVELIBRARY_V2_0_0_HPP
