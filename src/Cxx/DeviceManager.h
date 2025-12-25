#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include "Device.h"

enum class ActionStatus { PENDING, RUNNING, COMPLETED, FAILED };

class Action {
public:
    int actionId;
    ActionStatus status;
    std::string type;
    
    Action() : actionId(0), status(ActionStatus::PENDING) {}
    Action(int id, std::string t) : actionId(id), status(ActionStatus::PENDING), type(t) {}
};

class DeviceManager {
private:
    std::map<int, Device> devices;
    std::map<int, Action> actions;
    int nextActionId = 1;
    std::mutex actionMtx;

public:
    void registerDevice(int id, std::string name, int status);
    void setDeviceStatus(int id, DeviceStatus status);
    Device getDeviceInfo(int id);
    int initiateDeviceAction(int deviceId, std::string actionType, std::string params);
    ActionStatus getDeviceActionStatus(int actionId);
    std::string listDevicesText();
    std::map<int, Device> listDevices();
    void findDevice(int id);
    bool isIntValidStatus(int i);
};

#endif