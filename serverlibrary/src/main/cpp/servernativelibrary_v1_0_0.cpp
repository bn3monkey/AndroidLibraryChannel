//
// Created by hansono_bn3monkey on 2024-10-22.
//
#include "servernativelibrary_v1_0_0.hpp"

#if SERVERLIBRARY_MAJOR_VERSION == 1 && SERVERLIBRARY_MINOR_VERSION == 0 && SERVERLIBRARY_REVISION_VERSION == 0

const char* ServerLibrary_version();

struct ServerLibraryContext;

struct ServerLibrarayContext* ServerLibrary_initialize();
void ServerLibrary_release(struct ServerLibraryContext* context);

bool ServerLibrary_connectDevice(struct ServerLibrarayContext* context, int32_t fd);
void ServerLibrary_disconnectDevice(struct ServerLibrarayContext* context);


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

uint16_t ServerLibrary_getDeviceType(struct ServerLibrarayContext* context);

int32_t ServerLibrary_getDeviceComponentAID(struct ServerLibrarayContext* context);

int32_t ServerLibrary_getDeviceComponentBID(struct ServerLibrarayContext* context);




// Callback
void ServerLibrary_registerDataCallback(struct ServerLibrarayContext* context, void (*callback)(uint8_t* buf, int32_t length));
void ServerLibrary_unregisterDataCallback(struct ServerLibrarayContext* context);

void registerProbeDisconnectionCallback(struct ServerLibrarayContext* context, void (*callback)());
void unregisterProbeDisconnectionCallback(struct ServerLibrarayContext* context);

// Socket
int32_t registerDataSocket(struct ServerLibrarayContext* context);
void unregisterDataSocket();

#endif