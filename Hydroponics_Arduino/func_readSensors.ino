void measureDHT22() { //DHT22 Sensor
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  Serial.print("humidity.txt=\"");
  Serial.print(humidity);
  Serial.print("\"");
  //Serial.write(buffer2);
  clearNextionSerial();

  Serial.print("suhu.txt=\"");
  Serial.print(temperature);
  Serial.print("\"");
  clearNextionSerial();
}

float measureTDS() {

}

float measurePH() {

}
