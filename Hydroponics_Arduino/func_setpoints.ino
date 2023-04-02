

void save_setPoint() {
  EEPROM.write(1, 12); //Hour setp1
  EEPROM.write(2, 18);
  EEPROM.write(3, 15);
  EEPROM.write(4, 17);
  EEPROM.write(5, 9);
  EEPROM.write(6, 16);
  EEPROM.write(7, 1); //Minute setpoint 1
  EEPROM.write(8, 30);
  EEPROM.write(9, 0);
  EEPROM.write(10, 30);
  EEPROM.write(11, 0);
  EEPROM.write(12, 30);

  Serial.println("setpoint");
}

void call_setPoint() {
  int j=6;
  for (int i = 0; i < 6; i++) {
    setpoint[0][i] = EEPROM.read(i + 1); //read hour
    
    setpoint[1][i] = EEPROM.read(j + 1); //read minute
    j++;
//     sprintf(buffer, "Hour : %d \t  Minute : %d ", setpoint[0][i], setpoint[1][i]);
//     Serial.println(buffer);
    
  }
  

  //(start_setP1 + "  " + start_setP2 + "  " + end_setP1 + "  " + end_setP2 );

}
