#ifndef HTTP_GATEWAY_H
#define HTTP_GATEWAY_H

// Pistache (HTTP)
#include <pistache/endpoint.h>
#include <pistache/router.h>
// gRPC y Protobuf
#include <grpcpp/grpcpp.h>
#include <google/protobuf/util/json_util.h>

// We import the gRPC service interface
#include "DeviceFleetManagerImpl.h"

class HttpGateway {
public:
    HttpGateway(DeviceFleetManagerServiceImpl* s);

    void setupRoutes(Pistache::Rest::Router& router);

private:
    void handleRegister(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handleList(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handleSetStatus(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handleInitiateAction(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handleGetActionStatus(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    DeviceFleetManagerServiceImpl* service;
};

#endif // HTTP_GATEWAY_H

