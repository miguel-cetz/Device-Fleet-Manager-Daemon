#ifndef DEVICE_FLEET_MANAGER_IMPL_H
#define DEVICE_FLEET_MANAGER_IMPL_H


#include <map>
#include <mutex>
#include <atomic>
#include <grpcpp/grpcpp.h>
#include "../proto/DeviceFleetManager.grpc.pb.h"
#include "DeviceManager.h"

//Service implementation
class DeviceFleetManagerServiceImpl final : public devicefleetmanager::DeviceFleetManagerService::Service {
private:
    DeviceManager manager;
    std::mutex mtx;

public:
    DeviceFleetManagerServiceImpl() = default;

    grpc::Status RegisterDevice(grpc::ServerContext* context, 
                               const devicefleetmanager::DeviceRequest* request,
                               devicefleetmanager::DeviceResponse* reply) override;

    grpc::Status ListDevices(grpc::ServerContext* context, 
                            const devicefleetmanager::Empty* request,
                            devicefleetmanager::DeviceList* reply) override;
                            
    grpc::Status SetDeviceStatus(grpc::ServerContext* context, 
                                const devicefleetmanager::StatusRequest* request,
                                devicefleetmanager::DeviceResponse* reply);

    grpc::Status GetDeviceInfo(grpc::ServerContext* context, 
                                const devicefleetmanager::DeviceID* request,
                                devicefleetmanager::DeviceRequest* reply);

    grpc::Status InitiateAction(grpc::ServerContext* context, 
                                const devicefleetmanager::ActionRequest* request,
                                devicefleetmanager::ActionResponse* reply);

    grpc::Status GetActionStatus(grpc::ServerContext* context, 
                                const devicefleetmanager::ActionID* request,
                                devicefleetmanager::ActionStatusResponse* reply);

};

#endif // DEVICE_FLEET_MANAGER_IMPL_H

