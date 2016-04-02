/* 
 *  Motor handling functions
 *  
 */

void driveMotors()
{
  Serial.println(__func__);
  
    /* This function will run the motors across the range of possible speeds.
     * Note that maximum speed is determined by the motor itself and the operating voltage. 
     * the PWM values sent by analogWrite() are fractions of the maximum speed possible by your hardware.
     * As an input value the relative definition of the used power range 0-100% which is converted to the corresponding PWM range 75-255. 
     * In theory the PWM range is 0-255. However in practice with this setup the operating range will be around 75-255 PWM. 
     * Formula: "PWM value to Motor driver" = "Min PWM" + (("Max PWM" - "Min PWM")/100))* Power%    
     * Driving direction can be defined by +/- values. Value (+) -> forward and Value(-) -> backward
     * Note that PWM operating range will be depending on several factors like capacity of batteries, overall system load, etc.
     * And finally each of the droid setup is different. Each of the devices are unique and requires case-by-case fine tunings.
     */
    boolean in1ena = (rightpower < 0)?false:true;
    boolean in2ena = (rightpower < 0)?true:false;
    boolean in3ena = (leftpower < 0)?false:true;
    boolean in4ena = (leftpower < 0)?true:false;
 
    int16_t rpower = (abs(rightpower*1.80))+75;  
    int16_t lpower = (abs(leftpower*1.80))+75; 

    if (rpower < 256 && lpower < 256 ) {
      digitalWrite(in1, in1ena); digitalWrite(in2, in2ena); digitalWrite(in3, in3ena); digitalWrite(in4, in4ena);
      if ( rightpower == 0 && leftpower == 0 ) { delay(100); }
      analogWrite(enA, lpower); analogWrite(enB, rpower);
    }
}

void stopMotors() {
    // now turn off motors
    // clean up & return
    digitalWrite(in1, HIGH); digitalWrite(in2, HIGH); digitalWrite(in3, HIGH); digitalWrite(in4, HIGH);
    delay(100);
    analogWrite(enA, 0); analogWrite(enB, 0);
    rightpower = 0;
    leftpower = 0;
    mdelay = 0;
    motor_active = false;
    // return;
}
/*
int fix_angle(int delta) {
  Serial.println(delta);
  if (delta < 5)  return 0;    
  if (delta > 0) {turn_left(); }
  if (delta < 0) {turn_right();}
  return 1;
}

int get_angle(void) {
  return 15;
}

int get_angle_delta(int orig_angle) {
  static int debug_error = 20;
    
  return orig_angle - get_angle() + (debug_error--);
}

void turn_left(void) {
    digitalWrite(in1, HIGH); digitalWrite(in2, LOW); digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
    analogWrite(enA, 255); analogWrite(enB, 255);
    delay(100);
    digitalWrite(in1, HIGH); digitalWrite(in2, HIGH); digitalWrite(in3, HIGH); digitalWrite(in4, HIGH);
    delay(100);
    analogWrite(enA, 0); analogWrite(enB, 0);
}

void turn_right(void) {
    digitalWrite(in1, LOW); digitalWrite(in2, HIGH); digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
    analogWrite(enA, 255); analogWrite(enB, 255);
    delay(100);
    digitalWrite(in1, HIGH); digitalWrite(in2, HIGH); digitalWrite(in3, HIGH); digitalWrite(in4, HIGH);
    delay(100);
    analogWrite(enA, 0); analogWrite(enB, 0);
}

*/
