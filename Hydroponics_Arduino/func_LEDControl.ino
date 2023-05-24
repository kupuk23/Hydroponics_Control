/* setpoints:
    LAMP 1 : 12:01 - 18:30
    LAMP 2 : 15:00 - 17:30
    LAMP 3 : 09:00 - 16:30,
*/




void LEDcontrol () {


  if (force_lamp != 1) {
    digitalWrite(13,LOW);
    for (int a = 0; a <= 4; a = a + 2)
    { if (setpoint[0][a] <= hour && hour <= setpoint[0][a + 1])
      {
        if (((hour == setpoint[0][a]) && (hour == setpoint[0][a + 1])) || (hour == setpoint[0][a + 1])) {
          if (minute < setpoint[1][a + 1]) growLight_ON(a);
          else  growLight_OFF(a);
          
          }
        else if (hour == setpoint[0][a]) {
          if (minute >= setpoint[1][a])
            growLight_ON(a);
          else growLight_OFF(a);

        }

        else growLight_ON(a);

      }

      else  growLight_OFF(a);
    }
  }
  else {
    digitalWrite(13,HIGH);
    digitalWrite(LAMP1, !HIGH);  // Turn ON internal LED
    digitalWrite(LAMP2, !HIGH);
    digitalWrite(LAMP3, !HIGH);
  }
}



void growLight_ON(int rack)
{
  switch (rack) {
    case 0:
      digitalWrite(LAMP1, !HIGH);
      break;
    case 2:
      digitalWrite(LAMP2, !HIGH);
      break;
    case 4:
      digitalWrite(LAMP3, !HIGH);
      break;
  }
}

void growLight_OFF(int rack)
{
  switch (rack) {
    case 0:
      digitalWrite(LAMP1, !LOW);
      break;
    case 2:
      digitalWrite(LAMP2, !LOW);
      break;
    case 4:
      digitalWrite(LAMP3, !LOW);
      break;
  }
}
