//
// Created by hansono_bn3monkey on 2024-10-22.
//

#ifndef ANDROIDNATIVELIBRARYCHANNEL_DATASOURCELIBRARYDEVICECOMMUNICATOR_HPP
#define ANDROIDNATIVELIBRARYCHANNEL_DATASOURCELIBRARYDEVICECOMMUNICATOR_HPP

#include <cstdint>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "datasourcelibrarylistener.hpp"
#include "datasourcelibrarydataclient.h"

enum class DataSourceLibrary_DeviceState
{
    DISCONNECTED,
    CONNECTED,
    ACTIVATED,
    STREAMING,

    NONE = -1
};

class DataSourceLibrary_DeviceCommunicator
{
public:

    DataSourceLibrary_DeviceCommunicator(
            DataSourceLibraryListener<int32_t>& onDeviceStateChanged,
            DataSourceLibraryListener<int8_t*, int32_t>& onDataReceived,
            DataSourceLibraryListener<void>& onDeviceDisconnected) :
            _device_task_runner(nullptr),
            _onDeviceStateChanged(onDeviceStateChanged),
            _onDataReceived(onDataReceived),
            _onDeviceDisconnected(onDeviceDisconnected),
            _state(DataSourceLibrary_DeviceState::DISCONNECTED),
            _device_id(-1),
            _device_component_a_id(-1),
                _device_component_b_id(-1)
    {

    }
    DataSourceLibrary_DeviceCommunicator(const DataSourceLibrary_DeviceCommunicator& other) = delete;
    virtual ~DataSourceLibrary_DeviceCommunicator() {
        disconnect();
    }

    bool connect(int32_t fd);
    void disconnect();

    bool isConnected();
    bool isActivated();
    bool isStreaming();

    int32_t writeToDeviceEEPROM(
            uint16_t address, const uint8_t *buffer, int32_t offset, uint32_t length);

    int32_t readFromDeviceEEPROM(
            uint16_t address, uint8_t *buffer, int32_t offset, uint32_t length);

    int32_t burstWrite(
            uint16_t address, const uint8_t *buf, int32_t offset, uint32_t length);

    int32_t singleRead(
            uint16_t address, uint16_t *out_value);

    int32_t singleWrite(
            uint16_t address, uint16_t value);

    int32_t getDeviceID();

    int32_t getDeviceComponentAID();

    int32_t getDeviceComponentBID();

private:
    std::unique_ptr<std::thread> _device_task_runner;
    std::mutex _mtx;
    DataSourceLibrary_DeviceState _state;

    int32_t _device_id;
    int32_t _device_component_a_id;
    int32_t _device_component_b_id;

    uint8_t _eeprom[1000] {0};
    uint8_t _bulkmemory[1000] {0};
    uint16_t _singlememory[1000] {0};

    int32_t data_content {0};

    void onDeviceStateChanged(int32_t fd);

    DataSourceLibrary_DeviceState checkDescriptor(int32_t fd);
    DataSourceLibrary_DeviceState readDeviceInformation(int32_t fd);
    DataSourceLibrary_DeviceState readData(int32_t fd);

    DataSourceLibraryListener<int32_t>& _onDeviceStateChanged;
    DataSourceLibraryListener<int8_t*, int32_t>& _onDataReceived;
    DataSourceLibraryListener<void>& _onDeviceDisconnected;
};

#endif //ANDROIDNATIVELIBRARYCHANNEL_datasourcelibraryDEVICECOMMUNICATOR_HPP
