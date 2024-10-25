//
// Created by hansono_bn3monkey on 2024-10-22.
//
#include "datasourcelibrarydevicecommunicator.hpp"

#include <memory>
#include "Log.hpp"
#include <algorithm>

#define LOG_TAG "DeviceCommunicator"
#define LOG_D(...) Bn3Monkey::Log::D(LOG_TAG, __VA_ARGS__);
#define LOG_E(...) Bn3Monkey::Log::E(LOG_TAG, __VA_ARGS__);

bool DataSourceLibrary_DeviceCommunicator::connect(int32_t fd)
{
    LOG_D("%s : fd : %d", __FUNCTION__, fd);
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _state = DataSourceLibrary_DeviceState::CONNECTED;
    }

    if (!_device_task_runner)
    {
        _device_task_runner = std::make_unique<std::thread>(
            [&, fd]()
            { onDeviceStateChanged(fd); });
    }
    return true;
}

void DataSourceLibrary_DeviceCommunicator::disconnect()
{
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _state = DataSourceLibrary_DeviceState::DISCONNECTED;
    }

    if (_device_task_runner)
    {
        _device_task_runner->join();
        _device_task_runner.reset();
    }
}

void DataSourceLibrary_DeviceCommunicator::onDeviceStateChanged(int32_t fd)
{
    LOG_D("%s : fd : %d", __FUNCTION__, fd);

    DataSourceLibrary_DeviceState previous_state { DataSourceLibrary_DeviceState::NONE };
    while (true)
    {
        DataSourceLibrary_DeviceState state;
        {
            std::lock_guard<std::mutex> lock(_mtx);
            state = _state;
        }
        if (previous_state != state) {
            previous_state = state;
            _onDeviceStateChanged((int32_t) state);
        }

        switch (state)
        {
        case DataSourceLibrary_DeviceState::DISCONNECTED:
            _onDeviceDisconnected();
            return;
        case DataSourceLibrary_DeviceState::CONNECTED:
            state = checkDescriptor(fd);
            break;
        case DataSourceLibrary_DeviceState::ACTIVATED:
            state = readDeviceInformation(fd);
            break;
        case DataSourceLibrary_DeviceState::STREAMING:
            state = readData(fd);
            break;
        }

        {
            std::lock_guard<std::mutex> lock(_mtx);
            _state = state;
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
}

bool DataSourceLibrary_DeviceCommunicator::isConnected()
{
    bool ret{false};
    {
        std::lock_guard<std::mutex> lock(_mtx);
        ret = _state == DataSourceLibrary_DeviceState::CONNECTED;
    }
    return ret;
}

bool DataSourceLibrary_DeviceCommunicator::isActivated()
{
    bool ret{false};
    {
        std::lock_guard<std::mutex> lock(_mtx);
        ret = _state == DataSourceLibrary_DeviceState::ACTIVATED;
    }
    return ret;
}

bool DataSourceLibrary_DeviceCommunicator::isStreaming()
{
    bool ret{false};
    {
        std::lock_guard<std::mutex> lock(_mtx);
        ret = _state == DataSourceLibrary_DeviceState::STREAMING;
    }
    return ret;
}
int32_t
DataSourceLibrary_DeviceCommunicator::writeToDeviceEEPROM(uint16_t address, const uint8_t *buffer,
                                                          int32_t offset, uint32_t length)
{
    std::copy(buffer + offset, buffer + offset + length, _eeprom + address);
    return 0;
}

int32_t DataSourceLibrary_DeviceCommunicator::readFromDeviceEEPROM(uint16_t address, uint8_t *buffer,
                                                                   int32_t offset, uint32_t length)
{
    std::copy(_eeprom + address, _eeprom + address + length, buffer + offset);
    return 0;
}

int32_t
DataSourceLibrary_DeviceCommunicator::burstWrite(uint16_t address, const uint8_t *buf, int32_t offset,
                                                 uint32_t length)
{
    std::copy(buf + offset, buf + offset + length, _bulkmemory + address);
    return 0;
}

int32_t DataSourceLibrary_DeviceCommunicator::singleRead(uint16_t address, uint16_t *out_value)
{
    *out_value = _singlememory[address];
    return 0;
}

int32_t DataSourceLibrary_DeviceCommunicator::singleWrite(uint16_t address, uint16_t value)
{
    _singlememory[address] = value;
    return 0;
}

int32_t DataSourceLibrary_DeviceCommunicator::getDeviceID()
{
    int32_t ret{0};
    {
        std::lock_guard<std::mutex> lock(_mtx);
        ret = _device_id;
    }
    return ret;
}

int32_t DataSourceLibrary_DeviceCommunicator::getDeviceComponentAID()
{
    int32_t ret{0};
    {
        std::lock_guard<std::mutex> lock(_mtx);
        ret = _device_component_a_id;
    }
    return ret;
}
int32_t DataSourceLibrary_DeviceCommunicator::getDeviceComponentBID()
{
    int32_t ret{0};
    {
        std::lock_guard<std::mutex> lock(_mtx);
        ret = _device_component_b_id;
    }
    return ret;
}

static bool isDataSendable(uint8_t *eeprom, uint8_t *bulkmemory, uint16_t *singlememory)
{
    return eeprom[1] == 1 && bulkmemory[2] == 2 && singlememory[3] == 3;
}
DataSourceLibrary_DeviceState DataSourceLibrary_DeviceCommunicator::readData(int32_t fd)
{
    constexpr size_t length = 4196;
    int8_t buffer[length]{0};

    if (isDataSendable(_eeprom, _bulkmemory, _singlememory))
    {
        char* convertedBuffer = (char *)buffer;
        sprintf(convertedBuffer, "data : %d", ++data_content);
        _onDataReceived(buffer, strlen(convertedBuffer));
    }
    return DataSourceLibrary_DeviceState::STREAMING;
}

DataSourceLibrary_DeviceState DataSourceLibrary_DeviceCommunicator::readDeviceInformation(int32_t fd)
{
    {
        int flag = fd & 1;
        switch (flag)
        {
        case 0:
            _device_id = 1;
            break;
        case 1:
            _device_id = 2;
            break;
        default:
            _device_id = -1;
            break;
        }
    }
    {
        int flag = fd & 2;
        switch (flag)
        {
        case 0:
            _device_component_a_id = 1;
            break;
        case 2:
            _device_component_a_id = 2;
            break;
        default:
            _device_component_a_id = -1;
            break;
        }
    }
    {
        int flag = fd & 4;
        switch (flag)
        {
        case 0:
            _device_component_b_id = 1;
            break;
        case 4:
            _device_component_b_id = 2;
            break;
        default:
            _device_component_b_id = -1;
            break;
        }
    }
    LOG_D("device id : %d device_component_a_id : %d device_component_b_id : %d",
          _device_id, _device_component_a_id, _device_component_b_id);

    return DataSourceLibrary_DeviceState::STREAMING;
}

DataSourceLibrary_DeviceState DataSourceLibrary_DeviceCommunicator::checkDescriptor(int32_t fd)
{
    if (fd < 0)
    {
        LOG_D("fd is invalid");
        _device_id = -1;
        _device_component_a_id = -1;
        _device_component_b_id = -1;
        return DataSourceLibrary_DeviceState::DISCONNECTED;
    }
    LOG_D("fd is valid");
    return DataSourceLibrary_DeviceState::ACTIVATED;
}