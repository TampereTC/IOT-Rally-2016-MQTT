/* 
 *  Parsing functions for down-link json strings
 *  Including led controlling functions
 */

void HandleIncomingJson() {
    
    StaticJsonBuffer<512> jsonInBuffer;                 
    const char *JsonChar = inputString.c_str(); // 1 KB
    JsonObject& root = jsonInBuffer.parseObject(JsonChar);
    int pin; int value; int ldelay;
    // Verify Json 
    if (JsonChar!=NULL && !root.success()) {
      Serial.println("parseObject() failed: ");
      Serial.println(JsonChar);
    }
    else {
        // Led pins 13-11 control
        command = root["command"];
        pin = root["data"][0];
        value = root["data"][1];
        ldelay = root["data"][2];
        // motor control
        rightpower = root["mdata"][0];
        leftpower = root["mdata"][1];
        mdelay = root["mdata"][2];
    if (memcmp(command, "lights", 5) == 0) {
    // Lights on, not blinking
      if (value == 1 && (ldelay == 0 || ldelay == NULL)){
        if (pin == 13) { pin13_blinking = false; }
        if (pin == 12) { pin12_blinking = false; }   
        if (pin == 11) { pin11_blinking = false; }  
        digitalWrite(pin, HIGH);              
      }
      // blinking lights
      else if (value == 1 && (!ldelay == 0 || !ldelay == NULL)){
        if (pin == 13) { pin13_blinking = true; pin13_delay = ldelay;}
        if (pin == 12) { pin12_blinking = true; pin12_delay = ldelay;}   
        if (pin == 11) { pin11_blinking = true; pin11_delay = ldelay;}              
      }
      // Lights off
      else if (value == 0 ){
        if (pin == 13) { pin13_blinking = false; }
        if (pin == 12) { pin12_blinking = false; }   
        if (pin == 11) { pin11_blinking = false; }  
        digitalWrite(pin, LOW);
        }
      }
    if (memcmp(command, "drive", 5) == 0) {
      motor_active = true; 
      mStartMillis = millis();
    }
    if (memcmp(command, "music", 5) == 0) {
      if(pin) {
        tuneOn = true;
        phase = 0;
      }
      else {
        tuneOn = false;
        noTone(24);
      }
    }
      // Debugging
      Serial.println("-------");
      Serial.println(command);
      Serial.println(motor_active, DEC);
      Serial.println(rightpower, DEC);
      Serial.println(leftpower, DEC);
      Serial.println(mdelay, DEC);
    
    }
  // returning the default values
  stringComplete = false;
  // clean json incoming data buffers
  pin = NULL; value = NULL; ldelay = NULL;
  inputString="";
  inChar = NULL; JsonChar = NULL;
  return;      
}
