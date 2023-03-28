/* setpoints:
    LAMP 1 : 12 - 18
    LAMP 2 : 15 - 18
    LAMP 3 : 09 - 16,
*/


struct Object {
  int hour;
};

Object time2;
void LEDcontrol () {
  //Datetime2 time2 = RTClib::now();

  time2.hour = incomingByte;
  // Serial.print(setpoint[0]);
  // Serial.print("  ");
  // Serial.print(time2.hour );
  // Serial.print("  ");
  // Serial.println(setpoint[1]);
  if (setpoint[0] <= time2.hour && time2.hour < setpoint[1])
  {
    digitalWrite(LAMP1, HIGH);

  }
  else digitalWrite(LAMP1, LOW);

  if (setpoint[2] <= time2.hour && time2.hour < setpoint[3])
  {
    digitalWrite(LAMP2, HIGH);
  }
  else digitalWrite(LAMP2, LOW);

  if (setpoint[4] <= time2.hour && time2.hour < setpoint[5])
  {
    digitalWrite(LAMP3, HIGH);
  }
  else digitalWrite(LAMP3, LOW);

  //Serial.println(time2.minute(), DEC);
}
