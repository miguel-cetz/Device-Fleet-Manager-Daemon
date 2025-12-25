#!/bin/bash

#-----------------------------------------------------
# Script to install gRPC
# Miguel Cetz González
# run it as "install_grpc ." or "source install_grpc" 
#-----------------------------------------------------

export MY_INSTALL_DIR=$HOME/gRPC

#Ensure that the directory exists:
mkdir -p $MY_INSTALL_DIR

#Add the local bin folder to your path variable:
export PATH="$MY_INSTALL_DIR/bin:$PATH"

#Install the basic tools required to build gRPC and also adding pistache and nlohmann_json:
sudo apt install -y build-essential autoconf libtool pkg-config nlohmann-json3-dev libpistache-dev

# Go to the install directory:
cd $MY_INSTALL_DIR

#Clone the grpc repo and its submodules
git clone --recurse-submodules -b v1.76.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc


# Build and install gRPC and Protocol Buffers
# While not mandatory, gRPC applications usually leverage Protocol Buffers 
# for service definitions and data serialization, and the example code uses proto3.
# (takes a while running....)
cd grpc
mkdir -p cmake/build
pushd cmake/build
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_CXX_STANDARD=17 \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
make -j 4
make install
popd

#Also add next line to ~/.bashrc so we can run protoc from any where:
# nano ~/.bashrc
# export PATH=$PATH:$HOME/gRPC/bin










