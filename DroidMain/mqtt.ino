// Callback made from esp-link to notify of wifi status changes
// Here we just print something out for grins

void wifiCb(void* response) {
  ELClientResponse *res = (ELClientResponse*)response;
  if (res->argc() == 1) {
    uint8_t status;
    res->popArg(&status, 1);

    if(status == STATION_GOT_IP) {
      if (debugging == true) { Serial.println("WIFI CONNECTED");}
    } else {
      if (debugging == true) {
        Serial.print("WIFI NOT READY: ");
        Serial.println(status); }
    }
  }
}

// Callback when MQTT is connected
void mqttConnected(void* response) {
  if (debugging == true) { Serial.println("MQTT connected!"); }
  mqtt.subscribe("iot-downlink");
  connected = true;
}

// Callback when MQTT is disconnected
void mqttDisconnected(void* response) {
  if (debugging == true) { Serial.println("MQTT disconnected!"); }
  connected = false;
}

// Callback when an MQTT message arrives for one of our subscriptions
void mqttData(void* response) {
  ELClientResponse *res = (ELClientResponse *)response;
  if (debugging == true) { Serial.print("Received: topic=");}
  String topic = res->popString();
  if (debugging == true) { Serial.println(topic);}
  
  String data = res->popString();
  inputString = data.c_str();
  stringComplete = true;
  if (debugging == true) { 
    Serial.print("data=");
    Serial.println(data); }
}

void mqttPublished(void* response) {
  if (debugging == true) { Serial.println("MQTT published");}
}
