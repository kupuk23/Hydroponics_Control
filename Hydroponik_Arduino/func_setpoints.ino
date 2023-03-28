


void save_setPoint() {
  EEPROM.write(1, 12);
  EEPROM.write(2, 18);
  EEPROM.write(3, 15);
  EEPROM.write(4, 18);
  EEPROM.write(5, 9);
  EEPROM.write(6, 16);
  EEPROM.write(7, 20);
  EEPROM.write(8, 25);
  EEPROM.write(9, 10);
  EEPROM.write(10, 15);
  EEPROM.write(11, 20);
  EEPROM.write(12, 25);
  Serial.println("setpoint");
}

void call_setPoint() {
  for (int i = 0; i < 12; i++) {
    setpoint[i] = EEPROM.read(i + 1);
    //Serial.println(setpoint[i]);
  }

  //(start_setP1 + "  " + start_setP2 + "  " + end_setP1 + "  " + end_setP2 );

}

