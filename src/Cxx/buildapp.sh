#!/bin/bash
#-----------------------------------------------------
# Script to compile Device Fleet Management Service
#
# Miguel Cetz González
# 
#-----------------------------------------------------
echo "Script to compile Device Fleet Management Service --"
echo "Compiling protofiles --"
make -C ./../proto #generate protofiles
echo "Compiling Service --"
cmake -B build  #CMake doing it's job
make -C ./build #Generating binary

