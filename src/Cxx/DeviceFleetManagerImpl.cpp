/*----------------------------------------------------
Project: Device Fleet Management Service
by: Miguel Cetz González
-----------------------------------------------------*/

#include <iostream>

#include "DeviceFleetManagerImpl.h"


grpc::Status DeviceFleetManagerServiceImpl::RegisterDevice(grpc::ServerContext* context, 
                                                         const devicefleetmanager::DeviceRequest* request,
                                                         devicefleetmanager::DeviceResponse* reply) {
    // Lock to protect concurrent access between gRPC and HTTP
    std::lock_guard<std::mutex> lock(mtx);

    manager.registerDevice(request->id(), request->name(), request->status() );
    
    reply->set_success(true);
    reply->set_message("Device '" + request->name() + "' registered in the fleet.");
    
    std::cout << "[gRPC] Added device: " << request->name() << " (ID: " << request->id() << ")" << std::endl;
    return grpc::Status::OK;
}

grpc::Status DeviceFleetManagerServiceImpl::ListDevices(grpc::ServerContext* context, 
                                                      const devicefleetmanager::Empty* request,
                                                      devicefleetmanager::DeviceList* reply) {
    std::lock_guard<std::mutex> lock(mtx);

    //put the manager's information here in the request... 
    devicefleetmanager::DeviceRequest devx;
    manager.listDevices();
    for (const auto& [id, dev] : manager.listDevices()) {

        devx.set_id(id);
        devx.set_name(dev.name);
        devx.set_status(static_cast<int>(dev.status));
        std::cout << "\n[DEBUG] devx.status()= (" << devx.status() << ") compared to static_cast<int>(dev.status)= "<< static_cast<int>(dev.status)<< std::endl;
        *reply->add_devices() = devx;

    }
    return grpc::Status::OK;
}

grpc::Status DeviceFleetManagerServiceImpl::SetDeviceStatus(grpc::ServerContext* context, 
                                                           const devicefleetmanager::StatusRequest* request,
                                                           devicefleetmanager::DeviceResponse* reply) {
    std::lock_guard<std::mutex> lock(mtx);
    //Cast int32 from proto to C++ enum
    manager.setDeviceStatus(request->id(), static_cast<DeviceStatus>(request->status()));
    
    reply->set_success(true);
    reply->set_message("Device Status updated.");
    return grpc::Status::OK;
}

grpc::Status DeviceFleetManagerServiceImpl::GetDeviceInfo(grpc::ServerContext* context, 
                                                         const devicefleetmanager::DeviceID* request,
                                                         devicefleetmanager::DeviceRequest* reply) {
    std::lock_guard<std::mutex> lock(mtx);
    Device dev = manager.getDeviceInfo(request->id());
    
    reply->set_id(request->id());
    reply->set_name(dev.name);
    reply->set_status(static_cast<int32_t>(dev.status));
    return grpc::Status::OK;
}

grpc::Status DeviceFleetManagerServiceImpl::InitiateAction(grpc::ServerContext* context, 
                                                          const devicefleetmanager::ActionRequest* request,
                                                          devicefleetmanager::ActionResponse* reply) {
    // Note: initiateDeviceAction is safe as it manages its own thread internally
    int aid = manager.initiateDeviceAction(request->device_id(), request->action_type(), request->params());
    
    if (aid == -1) return grpc::Status(grpc::StatusCode::NOT_FOUND, "Device not found");
    
    reply->set_action_id(aid);
    return grpc::Status::OK;
}

grpc::Status DeviceFleetManagerServiceImpl::GetActionStatus(grpc::ServerContext* context, 
                                                           const devicefleetmanager::ActionID* request,
                                                           devicefleetmanager::ActionStatusResponse* reply) {
    ActionStatus s = manager.getDeviceActionStatus(request->id());
    reply->set_status(static_cast<int32_t>(s));
    return grpc::Status::OK;
}

