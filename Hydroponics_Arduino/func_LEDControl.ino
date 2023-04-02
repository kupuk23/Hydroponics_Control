/* setpoints:
    LAMP 1 : 12:01 - 18:30
    LAMP 2 : 15:00 - 17:30
    LAMP 3 : 09:00 - 16:30,
*/


struct Object {
  int hour;
  int minute;
};

Object time2;
void LEDcontrol () {

  //Datetime2 time2 = RTClib::now();
  time2.hour = 10;
  time2.minute = 0;
  // Serial.print(setpoint[0]);
  // Serial.print("  ");
  // Serial.print(time2.hour );
  // Serial.print("  ");
  // Serial.println(setpoint[1]);
  

  for (int a = 0; a <= 4; a = a + 2)
  { if (setpoint[0][a] <= time2.hour && time2.hour <= setpoint[0][a + 1])
    {

      if (time2.hour == setpoint[0][a]) {
        if (time2.minute >= setpoint[1][a])
          growLight_ON(a);
        else growLight_OFF(a);

      }
      else if (time2.hour == setpoint[0][a + 1]) {
        if (time2.minute < setpoint[1][a + 1]) growLight_ON(a);
        else  growLight_OFF(a);
      }

      else growLight_ON(a);

    }
  
  else  growLight_OFF(a);
}
}



void growLight_ON(int rack)
{
  switch (rack) {
    case 0:
      digitalWrite(LAMP1, HIGH);
      break;
    case 2:
      digitalWrite(LAMP2, HIGH);
      break;
    case 4:
      digitalWrite(LAMP3, HIGH);
      break;
  }
}

void growLight_OFF(int rack)
{
  switch (rack) {
    case 0:
      digitalWrite(LAMP1, LOW);
      break;
    case 2:
      digitalWrite(LAMP2, LOW);
      break;
    case 4:
      digitalWrite(LAMP3, LOW);
      break;
  }
}
