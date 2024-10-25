//
// Created by hansono_bn3monkey on 2024-10-22.
//

#ifndef ANDROIDNATIVELIBRARYCHANNEL_SERVERNATIVELIBRARY_V1_0_0_HPP
#define ANDROIDNATIVELIBRARYCHANNEL_SERVERNATIVELIBRARY_V1_0_0_HPP

#include <cstdint>

#include "datasourcelibraryversion.hpp"
#if DATASOURCELIBRARY_MAJOR_VERSION == 1 && DATASOURCELIBRARY_MINOR_VERSION == 0 && DATASOURCELIBRARY_REVISION_VERSION == 0

#ifdef __cplusplus
}
#endif


const char* DATASOURCELIBRARY_version();

struct datasourcelibraryContext;

// Should be called in an application

struct ServerLibrarayContext* DATASOURCELIBRARY_initialize();
void DATASOURCELIBRARY_release(struct datasourcelibraryContext* context);

bool DATASOURCELIBRARY_connectDevice(struct ServerLibrarayContext* context, int32_t fd);
void DATASOURCELIBRARY_disconnectDevice(struct ServerLibrarayContext* context);

// Should be called in other library

bool DATASOURCELIBRARY_isDeviceConnected(struct ServerLibrarayContext* context);

int32_t DATASOURCELIBRARY_writeToDeviceEEPROM(
        struct ServerLibrarayContext* context,
        uint16_t address, const uint8_t* buffer, int32_t offset, uint32_t length);

int32_t DATASOURCELIBRARY_readFromDeviceEEPROM(
        struct ServerLibrarayContext* context,
        uint16_t address, uint8_t* buffer, int32_t offset, uint32_t length);

int32_t DATASOURCELIBRARY_burstWrite(-
        struct ServerLibrarayContext* context,
        uint16_t address, const uint8_t *buf, int32_t offset, uint32_t length);

int32_t DATASOURCELIBRARY_singleRead(
        struct ServerLibrarayContext* context,
        uint16_t address, uint16_t* out_value);

uint16_t DATASOURCELIBRARY_getDeviceID(struct ServerLibrarayContext* context);


// Callback
void DATASOURCELIBRARY_registerDataCallback(struct ServerLibrarayContext* context, void (*callback)(uint8_t* buf, int32_t length));
void DATASOURCELIBRARY_unregisterDataCallback(struct ServerLibrarayContext* context);

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
