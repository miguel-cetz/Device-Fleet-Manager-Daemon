
How to install dependencies
----------------------------------------------------------------------------
Always use a virtual environment (venv) before installing dependencies to avoid conflicts with other system libraries.

python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt


Linux Bash
----------------------------------------------------------------------------
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt


Windows
----------------------------------------------------------------------------
admin shell:
Set-ExecutionPolicy RemoteSigned

python -m venv venv
.venv\Scripts\activate
pip install -r requirements.txt


Generate the code for python:
----------------------------------------------------------------------------
You have to do it from the folder where the .proto file is located:

python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. DeviceFleetManager.proto

after that copy the files to the folder where python dfm.py program is located:
cp *.py ..\pycli\


To test:
=====================================================================

python dfm.py register --id 50 --name "Room thermostat" --status 0
python dfm.py register --id 10 --name "Scout Drone" --status 0
python dfm.py register --id 11 --name "Base Station" --status 0

python dfm.py list

python dfm.py  action --id 2 --type "FIRMWARE_UPGRADE" --watch


