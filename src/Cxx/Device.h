#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <iostream>

enum class DeviceStatus {
    IDLE,
    BUSY,
    OFFLINE,
    MAINTENANCE,
    UPDATING,
    RECOVERING,
    ERROR
};

class Device {
public:
    std::string name;
    DeviceStatus status;

    Device() : name("Unknown"), status(DeviceStatus::IDLE) {}
    Device(std::string n, DeviceStatus s) : name(n), status(s) {}

    void display() const;
    static std::string statusToString(DeviceStatus s);

};

#endif