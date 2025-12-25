/*----------------------------------------------------
Project: Device Fleet Management Service
by: Miguel Cetz González
-----------------------------------------------------*/

#include <iostream>
#include <chrono>

#include "DeviceManager.h"

void DeviceManager::registerDevice(int id, std::string name, int status) {
    DeviceStatus ds;
    ds=isIntValidStatus(status)?static_cast<DeviceStatus>(status):DeviceStatus::IDLE;
    devices[id] = Device(name, ds);
}

void DeviceManager::setDeviceStatus(int id, DeviceStatus status) {
    if (devices.count(id)) {
        devices[id].status = status;
    }
}

Device DeviceManager::getDeviceInfo(int id) {
    if (devices.count(id)) return devices[id];
    return Device("Not Found", DeviceStatus::OFFLINE);
}

int DeviceManager::initiateDeviceAction(int deviceId, std::string actionType, std::string params) {
    if (devices.find(deviceId) == devices.end()) return -1;

    std::lock_guard<std::mutex> lock(actionMtx);
    int aid = nextActionId++;
    actions[aid] = Action(aid, actionType);

    //Asynchronous simulation
    std::thread([this, deviceId, aid]() {
        // PENDING -> RUNNING
        {
            std::lock_guard<std::mutex> lck(actionMtx);
            actions[aid].status = ActionStatus::RUNNING;
            devices[deviceId].status = DeviceStatus::UPDATING;
        }

        std::cout << "[Action " << aid << "] Running " << actions[aid].type << "..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(15)); // Simulación

        // RUNNING -> COMPLETED
        {
            std::lock_guard<std::mutex> lck(actionMtx);
            actions[aid].status = ActionStatus::COMPLETED;
            devices[deviceId].status = DeviceStatus::IDLE;
        }
        std::cout << "[Action " << aid << "] Finished." << std::endl;
    }).detach();

    return aid;
}

ActionStatus DeviceManager::getDeviceActionStatus(int actionId) {
    std::lock_guard<std::mutex> lock(actionMtx);
    if (actions.count(actionId)) return actions[actionId].status;
    return ActionStatus::FAILED;
}

std::string DeviceManager::listDevicesText() {
    std::string devicesListed;
    if (devices.empty()) {
        devicesListed = "No devices registered.";
        std::cout << devicesListed << std::endl;
        return devicesListed;
    }

    std::cout << "\n---Device List ---" << std::endl;
    for (auto const &[id, dev] : devices) {
        std::cout << "ID: " << id << " -> ";
        devicesListed+= std::to_string(id) + " | " + dev.name + " | " + dev.statusToString(dev.status)+ "\n"; 
    }

    return devicesListed;
}

std::map<int, Device>  DeviceManager::listDevices() {
    return devices;
}

void DeviceManager::findDevice(int id) {
    auto const &it = devices.find(id);
    if (it != devices.end()) {
        std::cout << "Found: ";
        it->second.display();
    } else {
        std::cout << "Device with ID " << id << " not found." << std::endl;
    }
}

bool DeviceManager::isIntValidStatus(int i)
{
    switch (static_cast<DeviceStatus>(i)) {
        case DeviceStatus::IDLE:
        case DeviceStatus::BUSY:
        case DeviceStatus::OFFLINE:
        case DeviceStatus::MAINTENANCE:
        case DeviceStatus::UPDATING:
        case DeviceStatus::RECOVERING:
        case DeviceStatus::ERROR: 
            return true;
        default: 
            return false;
    }
    return false;
}

