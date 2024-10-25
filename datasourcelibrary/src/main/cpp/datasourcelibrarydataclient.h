//
// Created by bn3mo on 2024-10-23.
//

#ifndef ANDROIDNATIVELIBRARYCHANNEL_DATASOURCELIBRARYDATACLIENT_H
#define ANDROIDNATIVELIBRARYCHANNEL_DATASOURCELIBRARYDATACLIENT_H

#include <cstdint>

class DataSourceLibrary_DataClient {
public:
    virtual ~DataSourceLibrary_DataClient() {
        close();
    }

    bool open(int32_t port);
    void close();

    int32_t send(const uint8_t* buffer, size_t length);

    inline bool isOpened() { return _is_opened; }

private:
    bool _is_opened;
    // socket _sock;
};


#endif //ANDROIDNATIVELIBRARYCHANNEL_DATASOURCELIBRARYDATACLIENT_H
