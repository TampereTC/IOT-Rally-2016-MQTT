# Node-red

[Node-red] is a tool for wiring together hardware devices, APIs and online services in new and interesting ways. It integrates easily with MQTT server and can therefore be used in the robot project as remote controller.

### Receiving data from the robot
To receive data from MQTT add input MQTT node to the flow and configure it. MQTT receives the payload as a string. JSON node can used to parse the data to JSON format. Then it is easy to analyse the data and build driving logic. Note that the flow needs to be activated by deploying it. All changes need to be separately deployed, too. As default the robot sends data to iot-uplink topic. This can be changed in the Arduino code.

### Sending commands to robot
Injection node can be used to triggering data to MQTT server. The message needs to be built in a function node. There is an example of message building in the file [codeOrgToRobot.js]. Obviously a MQTT output node needs to be added to send the data to MQTT server and to the robot. As default the robot listens to data from iot-downlink topic. Again, this can be changed in the Arduino code.  

### Use code.org code to drive robot 

In code.org there are coding lessons that produce code like this:
```sh
moveForward()
turnLeft()
turnRight()
```
The code can be copied to [codeOrgToRobot.js] and when it is run the robot should go the same route as the characters in the code.org exercise. It may be necessary to adjust some of the commands in the file. See the robot interface specifications for instructions on the commands.

[codeOrgToRobot.js]: <codeOrgToRobot.js> 
[Node-red]: <http://nodered.org/>