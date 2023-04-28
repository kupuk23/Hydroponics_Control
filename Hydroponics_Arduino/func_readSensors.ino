float measureDHT22() {  //DHT22 Sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.print("humidity.txt=\"");
  Serial.print(humidity);
  Serial.print("\"");
  //Serial.write(buffer2);
  clearNextionSerial();

  Serial.print("suhu.txt=\"");
  Serial.print(temperature);
  Serial.print("\"");
  clearNextionSerial();
  return (humidity);
}

float measureTDS() {
}

float measurePH() {
}
