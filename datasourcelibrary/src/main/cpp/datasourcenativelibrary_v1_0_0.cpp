//
// Created by hansono_bn3monkey on 2024-10-22.
//

#if DATASOURCELIBRARY_MAJOR_VERSION == 1 && DATASOURCELIBRARY_MINOR_VERSION == 0 && DATASOURCELIBRARY_REVISION_VERSION == 0

const char* datasourcelibrary_version();

struct datasourcelibraryContext;

struct ServerLibrarayContext* datasourcelibrary_initialize();
void datasourcelibrary_release(struct datasourcelibraryContext* context);

bool datasourcelibrary_connectDevice(struct ServerLibrarayContext* context, int32_t fd);
void datasourcelibrary_disconnectDevice(struct ServerLibrarayContext* context);


bool datasourcelibrary_isDeviceConnected(struct ServerLibrarayContext* context);

int32_t datasourcelibrary_writeToDeviceEEPROM(
        struct ServerLibrarayContext* context,
        uint16_t address, const uint8_t* buffer, int32_t offset, uint32_t length);

int32_t datasourcelibrary_readFromDeviceEEPROM(
        struct ServerLibrarayContext* context,
        uint16_t address, uint8_t* buffer, int32_t offset, uint32_t length);

int32_t datasourcelibrary_burstWrite(
        struct ServerLibrarayContext* context,
        uint16_t address, const uint8_t *buf, int32_t offset, uint32_t length);

int32_t datasourcelibrary_singleRead(
        struct ServerLibrarayContext* context,
        uint16_t address, uint16_t* out_value);

uint16_t datasourcelibrary_getDeviceType(struct ServerLibrarayContext* context);

int32_t datasourcelibrary_getDeviceComponentAID(struct ServerLibrarayContext* context);

int32_t datasourcelibrary_getDeviceComponentBID(struct ServerLibrarayContext* context);




// Callback
void datasourcelibrary_registerDataCallback(struct ServerLibrarayContext* context, void (*callback)(uint8_t* buf, int32_t length));
void datasourcelibrary_unregisterDataCallback(struct ServerLibrarayContext* context);

void registerProbeDisconnectionCallback(struct ServerLibrarayContext* context, void (*callback)());
void unregisterProbeDisconnectionCallback(struct ServerLibrarayContext* context);

// Socket
int32_t registerDataSocket(struct ServerLibrarayContext* context);
void unregisterDataSocket();

#endif