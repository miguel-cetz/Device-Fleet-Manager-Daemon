/*----------------------------------------------------
Project: Device Fleet Management Service
by: Miguel Cetz González
-----------------------------------------------------*/

#include "Device.h"

void Device::display() const {
    std::cout << "Name: " << name << " | Status: " << statusToString(status) << std::endl;
}

std::string Device::statusToString(DeviceStatus s) {
    switch (s) {
        case DeviceStatus::IDLE: return "IDLE";
        case DeviceStatus::BUSY: return "BUSY";
        case DeviceStatus::OFFLINE: return "OFFLINE";
        case DeviceStatus::MAINTENANCE: return "MAINTENANCE";
        case DeviceStatus::UPDATING: return "UPDATING";
        case DeviceStatus::RECOVERING: return "RECOVERING";
        case DeviceStatus::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}



