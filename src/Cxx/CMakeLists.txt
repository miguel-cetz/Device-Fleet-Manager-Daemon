#-----------------------------------------------------
# Device Fleet Management Service project
#
# With gRPC-HTTP Transcoding pattern
# Server talks gRPC but also accepts HTTP/JSON requests from Clients.
# via GateWay
#
# Miguel Cetz González
#-----------------------------------------------------
cmake_minimum_required(VERSION 3.10)

# Use this name as the name of the final executable
project(DevFleetManServ)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Place where the pistache library was installed:
find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)

# Find and bind Threads:
find_package(Threads REQUIRED)
find_package(nlohmann_json REQUIRED)

# Setting the local gRPC installation path because I installed it manually
# in {HOME} direcory and in this way cmake can locate it:
#-------------------------------------------------
set(GRPC_INSTALL_PATH $ENV{HOME}/gRPC)
message(STATUS "Using gRPC path from environment variable: $HOME= $ENV{HOME}")
message(STATUS "Using route for gRPC: ${GRPC_INSTALL_PATH}")
#Add the path to CMake searches
list(APPEND CMAKE_PREFIX_PATH "${GRPC_INSTALL_PATH}")
# IMPORTANT: Look for Protobuf BEFORE gRPC
# gRPCConfig.cmake requires Protobuf targets to already exist
find_package(Protobuf CONFIG REQUIRED HINTS "${GRPC_INSTALL_PATH}/grpc")
message(STATUS "Protobuf found in: ${Protobuf_DIR}")
#continue with gRPC
find_package(gRPC CONFIG REQUIRED HINTS "${GRPC_INSTALL_PATH}/grpc")
message(STATUS "gRPC configuration path: ${gRPC_DIR}")
# Locate the binary executables
find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin HINTS "${GRPC_INSTALL_PATH}/bin")
find_program(_PROTOBUF_PROTOC protoc HINTS "${GRPC_INSTALL_PATH}/bin")
#-------------------------------------------------

# To compile the proto file:
set(PROTO_FILE_NAME DeviceFleetManager)
set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../proto)
message(STATUS "PROTO_FILE_NAME es: ${PROTO_FILE_NAME}")
message(STATUS "PROTO_DIR es: ${PROTO_DIR}")
set(proto_file "${PROTO_DIR}/${PROTO_FILE_NAME}.proto")
set(proto_srcs "${PROTO_DIR}/${PROTO_FILE_NAME}.pb.cc")
set(proto_hdrs "${PROTO_DIR}/${PROTO_FILE_NAME}.pb.h")
set(grpc_srcs  "${PROTO_DIR}/${PROTO_FILE_NAME}.grpc.pb.cc")
set(grpc_hdrs  "${PROTO_DIR}/${PROTO_FILE_NAME}.grpc.pb.h")

include_directories("${PROTO_DIR}")

#Define the executable with all source files:
add_executable(${PROJECT_NAME}
    main.cpp
    DeviceFleetManagerImpl.cpp
    HttpGateway.cpp
    DeviceManager.cpp 
    Device.cpp
    ${proto_srcs}
    ${grpc_srcs}
)

# Link the libraries to the project:
target_link_libraries(${PROJECT_NAME}
    PRIVATE 
    PkgConfig::Pistache
    nlohmann_json::nlohmann_json
    gRPC::grpc++
    protobuf::libprotobuf
    Threads::Threads
)

