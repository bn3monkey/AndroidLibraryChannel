//
// Created by hansono_bn3monkey on 2024-10-22.
//

#ifndef ANDROIDNATIVELIBRARYCHANNEL_SERVERNATIVELIBRARY_V1_0_0_HPP
#define ANDROIDNATIVELIBRARYCHANNEL_SERVERNATIVELIBRARY_V1_0_0_HPP

#include <cstdint>

#include "serverlibraryversion.hpp"
#if SERVERLIBRARY_MAJOR_VERSION == 1 && SERVERLIBRARY_MINOR_VERSION == 0 && SERVERLIBRARY_REVISION_VERSION == 0

#ifdef __cplusplus
}
#endif


const char* ServerLibrary_version();

struct ServerLibraryContext;

// Should be called in an application

struct ServerLibrarayContext* ServerLibrary_initialize();
void ServerLibrary_release(struct ServerLibraryContext* context);

bool ServerLibrary_connectDevice(struct ServerLibrarayContext* context, int32_t fd);
void ServerLibrary_disconnectDevice(struct ServerLibrarayContext* context);

// Should be called in other library

bool ServerLibrary_isDeviceConnected(struct ServerLibrarayContext* context);

int32_t ServerLibrary_writeToDeviceEEPROM(
        struct ServerLibrarayContext* context,
        uint16_t address, const uint8_t* buffer, int32_t offset, uint32_t length);

int32_t ServerLibrary_readFromDeviceEEPROM(
        struct ServerLibrarayContext* context,
        uint16_t address, uint8_t* buffer, int32_t offset, uint32_t length);

int32_t ServerLibrary_burstWrite(
        struct ServerLibrarayContext* context,
        uint16_t address, const uint8_t *buf, int32_t offset, uint32_t length);

int32_t ServerLibrary_singleRead(
        struct ServerLibrarayContext* context,
        uint16_t address, uint16_t* out_value);

uint16_t ServerLibrary_getDeviceID(struct ServerLibrarayContext* context);


// Callback
void ServerLibrary_registerDataCallback(struct ServerLibrarayContext* context, void (*callback)(uint8_t* buf, int32_t length));
void ServerLibrary_unregisterDataCallback(struct ServerLibrarayContext* context);

void registerProbeDisconnectionCallback(struct ServerLibrarayContext* context, void (*callback)());
void unregisterProbeDisconnectionCallback(struct ServerLibrarayContext* context);

// Socket
int32_t registerDataSocket(struct ServerLibrarayContext* context);
void unregisterDataSocket();

#ifdef __cplusplus
}
#endif

#endif // VERSION_CHECK

#endif //ANDROIDNATIVELIBRARYCHANNEL_SERVERNATIVELIBRARY_V1_0_0_HPP
