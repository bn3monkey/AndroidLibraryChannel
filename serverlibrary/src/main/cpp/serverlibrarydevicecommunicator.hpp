//
// Created by hansono_bn3monkey on 2024-10-22.
//

#ifndef ANDROIDNATIVELIBRARYCHANNEL_SERVERLIBRARYDEVICECOMMUNICATOR_HPP
#define ANDROIDNATIVELIBRARYCHANNEL_SERVERLIBRARYDEVICECOMMUNICATOR_HPP

#include <cstdint>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "serverlibrarylistener.hpp"

enum class ServerLibrary_DeviceState
{
    DISCONNECTED,
    CONNECTED,
    ACTIVATED
};

class ServerLibrary_DeviceCommunicator
{
public:

    ServerLibrary_DeviceCommunicator(
            ServerLibraryListener<uint8_t*, int32_t>& onDataReceived,
            ServerLibraryListener<void>& onDeviceDisconnected) :
            _device_task_runner(nullptr),
            _onDataReceived(onDataReceived),
            _onDeviceDisconnected(onDeviceDisconnected)
    {

    }
    ServerLibrary_DeviceCommunicator(const ServerLibrary_DeviceCommunicator& other) = delete;
    virtual ~ServerLibrary_DeviceCommunicator() {}

    bool connect(int32_t fd);
    void disconnect();

    bool isConnected();
    bool isActivated();

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

    uint16_t getDeviceID();

    int32_t getDeviceComponentAID();

    int32_t getDeviceComponentBID();

private:
    std::unique_ptr<std::thread> _device_task_runner;
    std::mutex _mtx;
    ServerLibrary_DeviceState _state;

    uint16_t _device_id;
    int32_t _device_component_a_id;
    int32_t _device_component_b_id;


    void onDeviceConnected(int32_t fd);

    void checkDescriptor(int32_t fd);
    void readDeviceInformation(int32_t fd);

    ServerLibraryListener<uint8_t*, int32_t>& _onDataReceived;
    ServerLibraryListener<void>& _onDeviceDisconnected;
};

#endif //ANDROIDNATIVELIBRARYCHANNEL_SERVERLIBRARYDEVICECOMMUNICATOR_HPP
