void getSensorVal(String data) {
  char* token2 = strtok(data.c_str(), ";");
  int i = 0;
  while (token2 != NULL) {
    //Serial.println(retrievedSP[i]);
    switch (i) {
      case 0:
        humid = String(token2);
        break;
      case 1:
        temp = String(token2);
        break;
      case 2:
        tds = String(token2);
        break;
      case 3:
        ph = String(token2);
        break;
      case 4:
        dist = String(token2);
        break;
    }
    token2 = strtok(NULL, ";");
    i++;
  }
//  Serial.print("Sensor Value : ");
//  Serial.print(humid);
//  Serial.print(" , ");
//  Serial.print(temp);
//  Serial.print(" , ");
//  Serial.print(tds);
//  Serial.print(" , ");
//  Serial.print(ph);
//  Serial.print(" , ");
//  Serial.println(dist);
}

float measurePH() {
}
