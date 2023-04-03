void nex_SendTime(int yr, int m, int d, int h, int mnt, int s) {
  yr += 2000;
  sprintf (buffer2, "year.txt=\"%d\"", yr);
  Serial.print(buffer2);
  clearNextionSerial();
  sprintf (buffer2, "month.txt=\"%d\"", m);
  Serial.print(buffer2);
  clearNextionSerial();
  sprintf (buffer2, "day.txt=\"%d\"", d);
  Serial.print(buffer2);
  clearNextionSerial();
  sprintf (buffer2, "hour.txt=\"%d\"", h);
  Serial.print(buffer2);
  clearNextionSerial();
  sprintf (buffer2, "min.txt=\"%d\"", mnt);
  Serial.print(buffer2);
  clearNextionSerial();
  sprintf (buffer2, "sec.txt=\"%d\"", s);
  Serial.print(buffer2);
  clearNextionSerial();

}

void nex_updateSP() {
  uint32_t retrievedSP[12];

  //  nexSP1_h.getValue(&retrievedSP[0]);
  //  delay(100);
  //  nexSP1End_h.getValue(&retrievedSP[1]);
  //  delay(100);
  //  nexSP2_h.getValue(&retrievedSP[2]);
  //  delay(100);
  //  nexSP2End_h.getValue(&retrievedSP[3]);
  //  delay(100);
  //  nexSP3_h.getValue(&retrievedSP[4]);
  //  delay(100);
  //  nexSP3End_h.getValue(&retrievedSP[5]);
  //  delay(100);
  //  nexSP1_m.getValue(&retrievedSP[6]);
  //  nexSP1End_m.getValue(&retrievedSP[7]);
  //  nexSP2_m.getValue(&retrievedSP[8]);
  //  nexSP2End_m.getValue(&retrievedSP[9]);
  //  nexSP3_m.getValue(&retrievedSP[10]);
  //  nexSP3End_m.getValue(&retrievedSP[11]);
  //Serial.println(retrievedSP[0]);
  for (int i = 0; i < 6; i++) {
    EEPROM.write(i + 1, retrievedSP[i]);
    //Serial.print(retrievedSP[i]);

  }
  delay(100);
  call_setPoint();
}


void nex_sendSP() {


  for (int i = 0; i < 6; i++) {
    strcpy_P(buffer, (char *)pgm_read_word(&(nex_SPvar[0][i])));
    sprintf (buffer2, "%s.val=%d", buffer, setpoint[0][i] );
    Serial.print(buffer2);
    clearNextionSerial();

    strcpy_P(buffer, (char *)pgm_read_word(&(nex_SPvar[1][i])));
    sprintf (buffer2, "%s.val=%d", buffer, setpoint[1][i] );
    Serial.print(buffer2);
    clearNextionSerial();
    // sprintf(buffer, "Hour : %d \t  Minute : %d ", setpoint[0][i], setpoint[1][i]);
    // Serial.println(buffer);

  }
}

void clearNextionSerial() {
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void bSettingPushCallback(void *ptr)  // Press event for button b1
{
  nex_sendSP();
}  // End of press event


void bUpdatePushCallback(void *ptr)  // Press event for button b1
{
  nex_setpoints.getText(buffer, sizeof(buffer));
  char str_setpoints[sizeof(buffer)] = buffer;
  //Serial.println(buffer);
  //writeStringToEEPROM(20,str_setpoints);
  splitSP(str_setpoints);
  //  nex_updateSP();
  //  delay(500);
  //  nex_sendSP();
}  // End of press event

void splitSP(char setP[]) {

  char* token = strtok(setP, ",");
  int i=0;
  while (token != NULL) {

    Serial.println(token);
    //EEPROM.write(20+i,atoi(token));
    token = strtok(NULL, ",");
    i++;
  }

}
void btLampPushCallback(void *ptr)  // Press event for button b1
{
  digitalWrite(13, HIGH);  // Turn ON internal LED
}  // End of press event


void page0PushCallback(void *ptr)  // If page 0 is loaded on the display, the following is going to execute:
{
  CurrentPage = 0;  // Set variable as 0 so from now on arduino knows page 0 is loaded on the display
}  // End of press event


// Page change event:
void page1PushCallback(void *ptr)  // If page 1 is loaded on the display, the following is going to execute:
{
  CurrentPage = 1;  // Set variable as 1 so from now on arduino knows page 1 is loaded on the display
}  // End of press event
