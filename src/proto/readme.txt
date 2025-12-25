Modify make file variables so it points to the right path where proto compiler is located:
----------------------------------------------------------------------------------------------
PROTOC = `directory where libray was installed goes here../bin/protoc`
GRPC_PLUGIN = `directory where libray was installed goes here../bin/grpc_cpp_plugin`

then run make

This should generate the following files:
- DeviceFleetManager.pb.cc
- DeviceFleetManager.pb.h
- DeviceFleetManager.grpc.pb.cc
- DeviceFleetManager.grpc.pb.h



Info To generate the files manually:

# Generate files for C++ 
-----------------------------------------------------------------------------------
protoc --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` DeviceFleetManager.proto

# Generate files for Python, but do it from the activated python environment.
-----------------------------------------------------------------------------------
python3 -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. DeviceFleetManager.proto

