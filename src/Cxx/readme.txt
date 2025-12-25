To build the service application just run:
---------------------------------------
./buildapp.sh

and that will run cmake and make 

the bin file will be generated in:
../src/bin/DevFleetManServ

Remember to install all needed libraries:
---------------------------------------------
follow the instruction in readme file located in ../src/proto/ directory.

There is a script there called "install_grpc.sh" that does the libraries installation.

Run that before building the the application!

Also after installing the libraries we need to create the proto files.
so go to ../src/proto and follow the instructions in the readme.txt file
to generate the proto files.

And only then we can run ./buildapp.sh


To run the service
---------------------
Follow the instruction in readme.txt file located in ../src/systemd directory


Testing the service
--------------------------------
Once the service is running...


We can do testing with curl


Stastus:
    IDLE=0
    BUSY=1
    OFFLINE=2
    MAINTENANCE=3
    UPDATING=4
    RECOVERING=5
    ERROR=6


#List all devices:
curl -G http://localhost:8080/devices

#Register devices:
curl -X POST http://localhost:8080/device/register \
     -H "Content-Type: application/json; charset=UTF-8" \
     -d '{"id": 1, "name": "Thermal Sensor 01", "status": 0}'

curl -X POST http://localhost:8080/device/register \
     -H "Content-Type: application/json; charset=UTF-8" \
     -d '{"id": 2, "name": "security camera 01", "status": 0}'

curl -X POST http://localhost:8080/device/register \
     -H "Content-Type: application/json; charset=UTF-8" \
     -d '{"id": 3, "name": "Device01", "status": 0}'


#To update the device status:
curl -X POST http://localhost:8080/device/status \
     -H "Content-Type: application/json" \
     -d '{"id": 1, "status": 3}'


# Launch action to execute:
curl -X POST http://localhost:8080/device/action \
     -H "Content-Type: application/json" \
     -d '{"device_id": 1, "action_type": "SOFTWARE_UPDATE", "params": "v2.0.4"}'

curl -X POST http://localhost:8080/device/action -d '{"device_id": 1, "action_type": "UPDATE"}'     

#status of the launched action:
curl -X GET http://localhost:8080/action/:id/status | jq
curl http://localhost:8080/action/5/status


Action states:
0 (PENDING): The action was created but the thread has not yet started the heavy work.
1 (RUNNING): The thread is executing the 15 second sleep_for.
2 (COMPLETED): The thread ended and the device state returned to IDLE.
3 (FAILED): There was an error or the ID does not exist and the device status returned to IDLE.



