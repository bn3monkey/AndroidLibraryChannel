//
// Created by hansono_bn3monkey on 2024-10-22.
//
#include "serverlibrarydevicecommunicator.hpp"
#include "Log.hpp"

#define LOG_TAG "SERVERLIB"
#define LOG_D(...) Bn3Monkey::Log::D(LOG_TAG, __VA_ARGS__);
#define LOG_E(...) Bn3Monkey::Log::E(LOG_TAG, __VA_ARGS__);


bool ServerLibrary_DeviceCommunicator::connect(int32_t fd) {
    {
        if (!_device_task_runner)
        {
            _device_task_runner = std::unique_ptr<std::thread>(
                    new std::thread { [&]() {onDeviceConnected(fd); } }
                    );
        }
    }
}

void ServerLibrary_DeviceCommunicator::disconnect() {
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _state = ServerLibrary_DeviceState::DISCONNECTED;    
    }

    if (_device_task_runner) {
        _device_task_runner->join();
        _device_task_runner.release();            
    }
}

void ServerLibrary_DeviceCommunicator::onDeviceConnected(int32_t fd) {
    bool is_connected = isConnected();
    while(is_connected)
    {

    }
}

bool ServerLibrary_DeviceCommunicator::isConnected() {
    bool ret {false};
    {
        std::lock_guard<std::mutex> lock(_mtx);
        ret = _state == ServerLibrary_DeviceState::CONNECTED;
    }
    return ret;
}

bool ServerLibrary_DeviceCommunicator::isActivated() {
    bool ret {false};
    {
        std::lock_guard<std::mutex> lock(_mtx);
        ret = _state == ServerLibrary_DeviceState::ACTIVATED;
    }
    return ret;
}

int32_t
ServerLibrary_DeviceCommunicator::writeToDeviceEEPROM(uint16_t address, const uint8_t *buffer,
                                                      int32_t offset, uint32_t length) {
    return 0;
}

int32_t ServerLibrary_DeviceCommunicator::readFromDeviceEEPROM(uint16_t address, uint8_t *buffer,
                                                               int32_t offset, uint32_t length) {
    return 0;
}

int32_t
ServerLibrary_DeviceCommunicator::burstWrite(uint16_t address, const uint8_t *buf, int32_t offset,
                                             uint32_t length) {
    return 0;
}

int32_t ServerLibrary_DeviceCommunicator::singleRead(uint16_t address, uint16_t *out_value) {
    return 0;
}

int32_t ServerLibrary_DeviceCommunicator::singleWrite(uint16_t address, uint16_t value) {
    return 0;
}

uint16_t ServerLibrary_DeviceCommunicator::getDeviceID() {
    int32_t ret {0};
    {
        std::lock_guard<std::mutex> lock(_mtx);
        ret = _device_id;
    }
    return ret;
}

int32_t ServerLibrary_DeviceCommunicator::getDeviceComponentAID() {
    int32_t ret {0};
    {
        std::lock_guard<std::mutex> lock(_mtx);
        ret = _device_component_a_id;
    }
    return ret;
}

int32_t ServerLibrary_DeviceCommunicator::getDeviceComponentBID() {
        int32_t ret {0};
    {
        std::lock_guard<std::mutex> lock(_mtx);
        ret = _device_component_b_id;
    }
    return ret;
}

void ServerLibrary_DeviceCommunicator::readDeviceInformation(int32_t fd) {
    {
        std::lock_guard<std::mutex> lock(_mtx);
        {
            int flag = fd & 1;
            switch (flag)
            {
                case 0 :
                    _device_id = 1;
                    break;
                case 1:
                    _device_id = 2;
                    break;
            }
        }
        {
            int flag = fd & 2;
            switch(flag)
            {
                case 0:
                    _device_component_a_id = 1;
                    break;
                case 2:
                    _device_component_a_id = 2;
                    break;
            }
        }
        {
            int flag = fd & 4;
            switch(flag)
            {
                case 0:
                    _device_component_b_id = 1;
                    break;
                case 2:
                    _device_component_b_id = 2;
                    break;
            }
        }
        LOG_D("device id : %d device_component_a_id : %d device_component_b_id : %d",
            _device_id, _device_component_a_id, _device_component_b_id);

        _state = ServerLibrary_DeviceState::ACTIVATED;
    }
}

void ServerLibrary_DeviceCommunicator::checkDescriptor(int32_t fd) {
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (fd < 0)
        {
            _state = ServerLibrary_DeviceState::DISCONNECTED;
        }
        else {
            _state = ServerLibrary_DeviceState::CONNECTED;
        }
        LOG_D("fd is %s", fd < 0 ? "invalid" : "valid");
    }
}
