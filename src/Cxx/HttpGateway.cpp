/*----------------------------------------------------
Project: Device Fleet Management Service
by: Miguel Cetz González
-----------------------------------------------------*/

#include "DeviceFleetManagerImpl.h"

#include "HttpGateway.h"

HttpGateway::HttpGateway(DeviceFleetManagerServiceImpl* s) 
    : service(s) {}

void HttpGateway::setupRoutes(Pistache::Rest::Router& router) {
    Pistache::Rest::Routes::Post(router, "/device/register", Pistache::Rest::Routes::bind(&HttpGateway::handleRegister, this));
    Pistache::Rest::Routes::Get(router, "/devices", Pistache::Rest::Routes::bind(&HttpGateway::handleList, this));
  // Nuevas Rutas
    Pistache::Rest::Routes::Post(router, "/device/status", Pistache::Rest::Routes::bind(&HttpGateway::handleSetStatus, this));
    Pistache::Rest::Routes::Post(router, "/device/action", Pistache::Rest::Routes::bind(&HttpGateway::handleInitiateAction, this));
    Pistache::Rest::Routes::Get(router, "/action/:id/status", Pistache::Rest::Routes::bind(&HttpGateway::handleGetActionStatus, this));
}

void HttpGateway::handleRegister(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    devicefleetmanager::DeviceRequest protoReq;
    
    // Transcoding JSON -> Protobuf using the official Google utility
    auto status = google::protobuf::util::JsonStringToMessage(request.body(), &protoReq);
    
    if (!status.ok()) {
        response.send(Pistache::Http::Code::Bad_Request, "Error: Malformed JSON or incorrect fields");
        return;
    }

    devicefleetmanager::DeviceResponse protoRes;
    grpc::ServerContext ctx;
    
    // Internal call to native gRPC service
    service->RegisterDevice(&ctx, &protoReq, &protoRes);
    
    std::string jsonOut;
    google::protobuf::util::MessageToJsonString(protoRes, &jsonOut);
    
    response.setMime(MIME(Application, Json));
    response.send(Pistache::Http::Code::Ok, jsonOut);
}

void HttpGateway::handleList(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    devicefleetmanager::Empty empty;
    devicefleetmanager::DeviceList list;
    grpc::ServerContext ctx;
    
    service->ListDevices(&ctx, &empty, &list);
    
    std::string jsonOut;
    google::protobuf::util::MessageToJsonString(list, &jsonOut);
    
    response.setMime(MIME(Application, Json));
    response.send(Pistache::Http::Code::Ok, jsonOut);
}

void HttpGateway::handleSetStatus(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    devicefleetmanager::StatusRequest protoReq;
    auto status = google::protobuf::util::JsonStringToMessage(request.body(), &protoReq);
    
    if (!status.ok()) {
        response.send(Pistache::Http::Code::Bad_Request, "Invalid JSON");
        return;
    }

    devicefleetmanager::DeviceResponse protoRes;
    grpc::ServerContext ctx;
    service->SetDeviceStatus(&ctx, &protoReq, &protoRes);
    
    std::string jsonOut;
    google::protobuf::util::MessageToJsonString(protoRes, &jsonOut);
    response.send(Pistache::Http::Code::Ok, jsonOut);
}

void HttpGateway::handleInitiateAction(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    devicefleetmanager::ActionRequest protoReq;
    google::protobuf::util::JsonStringToMessage(request.body(), &protoReq);

    devicefleetmanager::ActionResponse protoRes;
    grpc::ServerContext ctx;
    auto grpcStatus = service->InitiateAction(&ctx, &protoReq, &protoRes);

    if (!grpcStatus.ok()) {
        response.send(Pistache::Http::Code::Not_Found, grpcStatus.error_message());
        return;
    }

    std::string jsonOut;
    google::protobuf::util::MessageToJsonString(protoRes, &jsonOut);
    response.send(Pistache::Http::Code::Ok, jsonOut);
}

void HttpGateway::handleGetActionStatus(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    int id = request.param(":id").as<int>();
    devicefleetmanager::ActionID protoReq;
    protoReq.set_id(id);

    devicefleetmanager::ActionStatusResponse protoRes;
    grpc::ServerContext ctx;
    service->GetActionStatus(&ctx, &protoReq, &protoRes);

    std::string jsonOut;
    google::protobuf::util::MessageToJsonString(protoRes, &jsonOut);
    response.send(Pistache::Http::Code::Ok, jsonOut);
}