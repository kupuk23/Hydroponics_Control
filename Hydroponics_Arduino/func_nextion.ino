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
  //Serial.println(buffer);
  splitSP(buffer);
  call_setPoint();
  delay(500);
  //  for (int i = 0; i < 12; i++) {
  //    sprintf (buffer2, "n%d.val=%d", i, EEPROM.read(i + 1));
  //    Serial.print(buffer2);
  //    clearNextionSerial();
  //  }
  nex_sendSP();
}



void splitSP(char* setP) {
  int retrievedSP[12];
  char* token = strtok(setP, ",");
  int i = 0;
  //  int charLength= strlen(setP);
  while (token != NULL) {
    //Serial.println(retrievedSP[i]);
    retrievedSP[i] = atoi(token);
    token = strtok(NULL, ",");
    i++;

  }
  for (int i = 0; i < 12; i++)  EEPROM.write(i + 1, retrievedSP[i]);
}

void btLampPushCallback(void *ptr)  // Press event for button btLamp
{
  
  btLamp.getValue(&force_lamp);
  Serial.print(force_lamp);
  clearNextionSerial();
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
