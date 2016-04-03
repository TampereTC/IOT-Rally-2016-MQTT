/* 
 *  Sensor reading and json publish functions
 *  
 */

// Read temperature (C) and humidity values from DHT11 and publish json 

void JsonReportSensorDHT() {
  // Create Json and publish
  String jsonDHT = String("{\"sensor\":\"temp_hum\",\"time\":\"" + TimeStr + "\",\"data\":[" + dht.readTemperature() + "," + dht.readHumidity() + "]}");
  Serial.println(jsonDHT);
  memset(bufff, 0, 512);  jsonDHT.toCharArray(bufff, 512);
  mqtt.publish("iot-uplink", bufff);
  return;
}

// Read distance values from ultrasonic and publish json 

void JsonReportSensorDistance(){
  // Create Json and publish
  String jsonDist = String("{\"sensor\":\"distance\",\"time\":\"" + TimeStr + "\",\"data\":[" + ultrasonic.Ranging(CM) + "]}");
  Serial.println(jsonDist); // CM or INC
  memset(bufff, 0, 512); jsonDist.toCharArray(bufff, 512);
  mqtt.publish("iot-uplink", bufff);
  return;
}  

// Read accelerometer and gyroscope raw values and publish json 

void JsonReportSensorAccAndGyro(){
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  // Create Json and publish
  String jsonAccGyro = String("{\"sensor\":\"acc_gyro\",\"time\":\"" + TimeStr + "\",\"data\":[" + ax + "," + ay + "," + az +"," + gx +"," + gy + "," + gz + "]}");
  Serial.println(jsonAccGyro);
  memset(bufff, 0, 512); jsonAccGyro.toCharArray(bufff, 512);
  mqtt.publish("iot-uplink", bufff);
  return;
}

// Read magneto raw values and publish json 

void JsonReportSensorMagneto(){
  // read raw magneto measurements from device
  magneto.getHeading(&mx, &my, &mz);
  // Create Json and publish
  String jsonMagneto = String("{\"sensor\":\"magneto\",\"time\":\"" + TimeStr + "\",\"data\":[" + mx + "," + my + "," + mz + "]}");
  Serial.println(jsonMagneto);
  memset(bufff, 0, 512); jsonMagneto.toCharArray(bufff, 512);
  mqtt.publish("iot-uplink", bufff);
  return;
}

// Read line tracker values and publish json 

void JsonReportSensorEdge() {
  // Create Json and publish
  String jsonEdge = String("{\"sensor\":\"edge\",\"time\":\"" + TimeStr + "\",\"data\":[" + left_edge + "," + right_edge + "]}");
  Serial.println(jsonEdge);
  memset(bufff, 0, 512); jsonEdge.toCharArray(bufff, 512);
  mqtt.publish("iot-uplink", bufff);
  return;
}  

// Read RFID tag value and publish json 

void JsonReportSensorRFID() {
  if ( ! rfid.PICC_ReadCardSerial()) return;
  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));
  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  if (rfid.uid.uidByte[0] != nuidPICC[0] || rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || rfid.uid.uidByte[3] != nuidPICC[3] ) {
    // Serial.println(F("A new card has been detected."));
    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) { nuidPICC[i] = rfid.uid.uidByte[i];
    }   
    byte *buffer = rfid.uid.uidByte; 
    byte bufferSize = rfid.uid.size;
    String NUID = "";
    for (byte i = 0; i < bufferSize; i++) {
      NUID = String(NUID + String(buffer[i], DEC)); 
    }
    // Create Json and publish
    String jsonRfid = String("{\"sensor\":\"rfid\",\"data\":[" + NUID + "]}");
    Serial.println(jsonRfid);
    memset(bufff, 0, 512); jsonRfid.toCharArray(bufff, 512);
    mqtt.publish("iot-uplink", bufff);
    return;
  }
  else Serial.println(F("Card read previously."));
  rfid.PICC_HaltA(); // Halt PICC
  rfid.PCD_StopCrypto1(); // Stop encryption on PCD
  return;  
}
