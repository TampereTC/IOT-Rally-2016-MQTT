# IoT-Ralli-2016
![alt tag] (https://github.com/jraivio/IoT-Ralli-Vempain/blob/master/Hardware/Pictures/Cloudyralli_mid.png) ![alt tag] (https://github.com/jraivio/IoT-Ralli-Vempain/blob/master/Hardware/Pictures/_DSC5885_small.JPG)

###Description of project:
This project aims to develop [Internet of Things (IoT)] (https://en.wikipedia.org/wiki/Internet_of_Things) specific development environment for educational institutions. Easy availability and low cost components are the design principles of this gadget. In addition, an important factor is the availability of the source code for hardware drivers. This project is focusing to produce a prototype device with required IoT features and interfaces. The brains to control a robot can be placed in the cloud server, if necessary.
Source code type of approach is mandatory. All used 3rd party libraries in this repository are also available from the github repositories as well. Including selected hardware e.g [Arduino] (https://www.arduino.cc) and [esp8266] (http://www.esp8266.com) both are based on the model of [open source electronics] (http://www.open-electronics.org/). 

###Technology:    

This robot is a Genuine IOT device with large set of sensors. Arduino Mega2560 board is managing the sensor interfaces and logic of the Droid functions. Internet access is based on the ESP8266 Wifi-bridge chipset. Arduino is connected to ESP unit via serial communication port. Communication with the cloud server is takes place via a bi-directional MQTT protocol. The robot is act as MQTT client and MQTT broker is located on the cloud server. MQTT logic from Droid point of view. Droid publish (sending) the sensor data to the cloud location and subscribe (receiving) control commands from then MQTT broker. Controlling the Droid is happening via the other MQTT client which is connected to the same mqtt broker as the device. Typical MQTT clients are like web browsers or mobile apps.    

![alt tag] (http://www.ibm.com/developerworks/cloud/library/cl-mqtt-bluemix-iot-node-red-app/image001.png)
