# Post installation steps

### 1. Increasing size Arduino RX buffer   
_Default size of the Arduino RX buffer is 64Byte. This setup requires at least 256 Bytes_

###Hardware Serial Buffer Expansion   
- The change for hardware serial ports require a simple modification of the file: 
- Please backup the file before modification e.g copy the orginal file to name `HardwareSerial.bak`
- `<base Arduino folder>\hardware\arduino\avr\cores\arduino\HardwareSerial.h`   

Change the serial buffer size from 64 up to 256 as described below (Instructions below are valid for Windows OS):       
![alt tag] (https://github.com/TampereTC/IOT-Rally-2016-MQTT/blob/master/Hardware/Pictures/rxbuffer.png)

![alt tag] (https://github.com/TampereTC/IOT-Rally-2016-MQTT/blob/master/Hardware/Pictures/rxbuffer_2.png)



Couple of usefull links.   
- http://internetofhomethings.com/homethings/?p=927     
- http://www.hobbytronics.co.uk/arduino-serial-buffer-size    
