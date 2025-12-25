/*----------------------------------------------------
Project: Device Fleet Management Service
by: Miguel Cetz González
Main file.
-----------------------------------------------------*/
#include <csignal>

// gRPC y Protobuf
#include <grpcpp/grpcpp.h>
#include <google/protobuf/util/json_util.h>

// Pistache (HTTP)
#include <pistache/endpoint.h>
#include <pistache/router.h>

#include "../proto/DeviceFleetManager.grpc.pb.h"
#include "DeviceFleetManagerImpl.h"
#include "HttpGateway.h"

//Variable for daemon stop control (systemd)
std::atomic<bool> keepRunning(true);

// Signal handler (Ctrl+C or 'systemctl stop' command)
void signalHandler(int signum) {
    std::cout << "\n[INFO] Signal (" << signum << ") received. Stopping service..." << std::endl;
    keepRunning = false;
}

int main() {
    // Register signal handlers
    signal(SIGINT, signalHandler);  // For Ctrl+C
    signal(SIGTERM, signalHandler); // For the 'kill' command Systemd

    DeviceFleetManagerServiceImpl myService;

    //1. Configure gRPC Server
    std::string server_address("0.0.0.0:50051");
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&myService);
    std::unique_ptr<grpc::Server> grpcServer(builder.BuildAndStart());

    //2. Configure Pistachio HTTP Server
    HttpGateway gateway(&myService);
    Pistache::Rest::Router router;
    gateway.setupRoutes(router);
    
    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(8080));
    Pistache::Http::Endpoint httpRest(addr);
    httpRest.init(Pistache::Http::Endpoint::options().threads(2));
    httpRest.setHandler(router.handler());
    
    //Start HTTP service in background threads
    httpRest.serveThreaded();

    std::cout << "[Daemon] DeviceFleetManager started." << std::endl;
    std::cout << "[Daemon] gRPC listening on: " << server_address << std::endl;
    std::cout << "[Daemon] HTTP Gateway listening on: 8080" << std::endl;

    //Keep the process alive until receiving a stop signal
    while (keepRunning) { 
        std::this_thread::sleep_for(std::chrono::milliseconds(250)); 
    }

    std::cout << "[Daemon] Starting off controlled..." << std::endl;
    
    grpcServer->Shutdown();
    httpRest.shutdown();
    
    return 0;
}