To start
--------------------------------------------
Move configuration file:  DevFleetManServ.service 
to directory: /etc/systemd/system/


Commands to control the service:
------------------------------------------------
Start the service:
 sudo systemctl start DevFleetManServ

Check if it is running:
 sudo systemctl status DevFleetManServ

View the logs (the console output):
journalctl -u DevFleetManServ -f

Stop it:
 sudo systemctl stop DevFleetManServ


To check if file is correct:
-----------------------------------------
Systemd will tell you exactly which line is bothering it if you run this command:

systemd-analyze verify devicemanagerapp.service
