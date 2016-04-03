// IOT RalliDroid  
// Copyright Jukka Raivio and Jussi Salonen 2016
// MIT License
// 
// Example code is based on Arduino Mega2560 
//
// Debugging mode, results is serial monitor (true = on, false = off)
boolean debugging = true;

/* Temperature & Humidity sensor DHT11 lib & setup
 * Adafruit version, https://github.com/adafruit/DHT-sensor-library  
 */
#include "DHT.h" 
#define DHTPIN  2       // 2 digital pin 
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

// I2C drivers
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
#include <I2Cdev.h>

/*  ELClient settings (MQTT)
 *  ELClient drivers by Jeelabs https://github.com/jeelabs/el-client 
 */
#include <ELClient.h>
#include <ELClientMqtt.h>
char bufff[512];
bool connected;
ELClient esp(&Serial1, &Serial); // Mega board Serial1 to ESP
// Initialize the MQTT client
ELClientMqtt mqtt(&esp);

/*  Distance sensor
 *  Ultrasonic HC-SR04 library for Arduino 
 *  by J.Rodrigo https://github.com/JRodrigoTech/Ultrasonic-HC-SR04
 */
#include <Ultrasonic.h> //
#define TRIG_PIN 10     // Configurable Arduino pins for HC-SR04 Trig PIN
#define ECHO_PIN 9  // Configurable Arduino pins for HC-SR04 Echo pins
Ultrasonic ultrasonic(TRIG_PIN,ECHO_PIN); 

/* Json parser: https://github.com/bblanchon/ArduinoJson
 */
#include <ArduinoJson.h>  
const char* command;
// Serial data handling, global variables
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
char inChar; // a char to handle incoming data

/*  Multitasking functions by Arduino millis
 *  Initialization to enable multithreading features by using millis() functions  
 */
unsigned long previousMillis = 0;       // will store last time sensor updated
unsigned long tuneMillis = 0;


// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
const long interval = 1000;            // Sensor data sending interval

// Tune settings
const long tempo = 180;
bool tuneOn = false;
static uint8_t phase = 0;
unsigned int tune[32] = {659,587,370,370,415,415,
                         554,494,294,294,330,330,
                         494,440,277,277,330,330,
                         440,440,0,0,
                         0,0,0,0
                         };  //

/* RFID tag reader (NFC) MFRC522 by Miki Balboa  https://github.com/miguelbalboa/rfid
 */
#include <MFRC522.h> 
#define RST_PIN   49     // Configurable, see typical pin layout above (valk)
#define SS_PIN    53    // Configurable, see typical pin layout above (vihr)
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 
// Init array that will store new NUID 
byte nuidPICC[3];

/* Gyroscope + accelerometer MPU-6050 library for Arduino by Jeff Rowberg
 * Compass (magneto) HMC5883L.h library for Arduino by Jeff Rowberg
 * https://github.com/jrowberg/i2cdevlib
 */
#include <SPI.h> // Arduino default SPI library
#include <MPU6050.h> //  Gyroscope + accelerometer
MPU6050 accelgyro(0x69); // Due to this AD0 pin must be pulled high otherwise 0x68 as default

#include <HMC5883L.h> // Magneto
HMC5883L magneto;
// Global variables to store the values
int16_t ax, ay, az; // ACC
int16_t gx, gy, gz; // Gyro
int16_t mx, my, mz; // Megneto


// Line tracker (Edge) sensor pinouts
#define right_edge 23 // Right sensor
#define left_edge 22 // Left sensor

// math library for motor control
#include <math.h>

// Motor driver Setup
// Pins L298N -> Mega board
int enA = 8; int in1 = 4; int in2 = 5; int in3 = 6; int in4 = 7; int enB = 3;
boolean motor_active = false;
// for motor delay
unsigned long mStartMillis = 0;      // will store last time motor delay update
int rightpower; // right motor power (0-100%) 
int leftpower; // left motor power (0-100%)
int mdelay; // motor delay
// millis for sensor reading interval while moving, delay 500 ms
boolean sensor_moving = false;
int sensor_moving_reports = 500;
unsigned long sensor_moving_previousMillis = 0;      // will store last time light delay update

// Driving lights setup
boolean pin13_blinking = false;
boolean pin12_blinking = false;
boolean pin11_blinking = false;
int pin13_delay; // pin13 for main lights
int pin12_delay; // pin12 for left lights
int pin11_delay; // pin11 for right lights
unsigned long pin13_previousMillis = 0;      // will store last time light delay update
unsigned long pin12_previousMillis = 0;      // will store last time light delay update
unsigned long pin11_previousMillis = 0;      // will store last time light delay update

/*
 * Real time clock (RTC) library by 
 * adafruit https://github.com/adafruit/RTClib 
 */
#include "RTClib.h" // 
RTC_DS1307 RTC;
String TimeStr ="";

// Reading the time from RTC function
void readTime() {

    DateTime now = RTC.now();
    String Year = String(now.year(), DEC);
    String Month = String(now.month(), DEC);
    String Day = String(now.day(), DEC);
    String Hour = String(now.hour(), DEC);
    String Minutes = String(now.minute(), DEC);
    String Seconds = String(now.second(), DEC);
    TimeStr = Year + "/" + Month + "/" + Day + ":" + Hour + ":" + Minutes + ":" + Seconds;
    // Debugging
    // Serial.print(TimeStr);
    // Serial.println();
}

void setup() {
  delay(2500);
    // initialize both serial ports:
    Serial.begin(115200); // Debugging
    delay(1000);
    Serial1.begin(115200);

    Serial.println(SERIAL_RX_BUFFER_SIZE);
    esp.wifiCb.attach(wifiCb); // wifi status change callback, optional (delete if not desired)
    bool ok;
    do {
      ok = esp.Sync();      // sync up with esp-link, blocks for up to 2 seconds
      if (!ok) Serial.println("EL-Client sync failed!");
    } while(!ok);
    Serial.println("EL-Client synced!");
  
    // Set-up callbacks for events and initialize with es-link.
    mqtt.connectedCb.attach(mqttConnected);
    mqtt.disconnectedCb.attach(mqttDisconnected);
    mqtt.publishedCb.attach(mqttPublished);
    mqtt.dataCb.attach(mqttData);
    mqtt.setup();
  
    //Serial.println("ARDUINO: setup mqtt lwt");
    Serial.println("EL-MQTT ready");
    
    delay(1000);
    Wire.begin(); // start I2C & RTC

    magneto.initialize();
    accelgyro.initialize();
    RTC.begin();
    if (! RTC.isrunning()) {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      //RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    // initialize digital pin 13-11 as an output (for LEDs)
    pinMode(13, OUTPUT); // Main lights leds
    pinMode(12, OUTPUT); // Direction lights
    pinMode(11, OUTPUT);
     // initialize digital pin 4-7 as an output (for Motor driver)
    pinMode(4, OUTPUT); 
    pinMode(5, OUTPUT); 
    pinMode(6, OUTPUT);   
    pinMode(7, OUTPUT);    
    // Ensure that motors are stopped while initialization phase
    digitalWrite(in1, HIGH); digitalWrite(in2, HIGH); digitalWrite(in3, HIGH); digitalWrite(in4, HIGH);  
    // Egde sensor input pins
    pinMode(right_edge,INPUT); // Right sensor
    pinMode(left_edge,INPUT); // Left sensor
    inputString.reserve(256); // reserve 256 bytes for the inputString:
    dht.begin(); // Init DHT
    // RFID setup
    SPI.begin(); // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522 
    for (byte i = 0; i < 6; i++) { key.keyByte[i] = 0xFF; } // RFID byte handling
}


void loop() {
  // update interval
  unsigned long currentMillis = millis();
  //int orig_angle = 0;
  //int too_much = 0;

  // MQTT sync up
  esp.Process();
  
  if (currentMillis - previousMillis >= interval && connected) {
    // save the last time of sensor reporting
    readTime();
    previousMillis = currentMillis;    
    if (motor_active == false) {
     JsonReportSensorDistance();
     JsonReportSensorDHT();  // DHT Sensor reporting 
     JsonReportSensorAccAndGyro(); // Gyro & ACC sensor reporting
     JsonReportSensorMagneto(); // Compass sensor reporting
    } 
  }

  if (tuneOn && currentMillis - tuneMillis >= tempo) {
    // save the last time of sensor reporting
    tuneMillis = currentMillis;
    phase++;
    if(tune[phase] == 0)
      noTone(24);
    else
      tone(24, tune[phase % 32]);
  }
  
  if (motor_active == true && connected) { // speed up reporting frequency in case of moving
    currentMillis = millis();
    if (currentMillis - mStartMillis >= sensor_moving_reports ) {
      readTime();
      JsonReportSensorDistance();
      JsonReportSensorEdge();
      JsonReportSensorAccAndGyro();
      JsonReportSensorMagneto();
    }
  }
  
  // Look for new RFID cards
  if ( rfid.PICC_IsNewCardPresent()) { JsonReportSensorRFID(); }
  
  if (stringComplete == true) { HandleIncomingJson(); }
  
  // Light blinking
  if ( pin13_blinking == true){
    currentMillis = millis();
    if ( currentMillis - pin13_previousMillis >= pin13_delay ) {
        pin13_previousMillis = currentMillis; 
        digitalWrite(13, digitalRead(13)^1);
    }
  }                       
  if ( pin12_blinking == true){
    currentMillis = millis();
    if (currentMillis - pin12_previousMillis >= pin12_delay) {
      pin12_previousMillis = currentMillis; 
      digitalWrite(12, digitalRead(12)^1);
    }
  }
  if ( pin11_blinking == true){
    currentMillis = millis();
    if ( currentMillis - pin11_previousMillis >= pin11_delay) {
      pin11_previousMillis = currentMillis; 
      digitalWrite(11, digitalRead(11)^1);
    }
  }     
  // Drive motors
  if (motor_active == true ) {
     currentMillis = millis();
     //orig_angle = get_angle();
     driveMotors();
     if (currentMillis - mStartMillis >= mdelay) {       
      stopMotors();
      //too_much = 0;
      /*
       while(fix_angle(get_angle_delta(orig_angle))) {
        if (too_much++ > 10)
          break;
      }*/
      mqtt.publish("iot-uplink", "{\"sensor\":\"drivedone\"}");
     }           
  }  
}
