float measureDHT22() { //DHT22 Sensor
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return (0,0);
  }
  else return (temperature, humidity);
}

float measureTDS() {

}

float measurePH() {
  
}