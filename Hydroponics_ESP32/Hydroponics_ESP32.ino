#include <NTPClient.h>

#include <WiFi.h> // for WiFi shield

#include <WiFiUdp.h>
#include "DHT.h"

#define DHTPIN 4      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
const char *ssid = "CIS SMU STRONG";
const char *password = "PTcis1234*";

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionally you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 25200, 60000);

char buffer[20];
char buff1[10];
char buff2[10];

void setup()
{
  // Start the Serial communication for debugging
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  dht.begin();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Start the SoftwareSerial communication with the ESP8266
}

void loop()
{
  float h = dht.readHumidity(); 
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t))
    Serial.println("0;0");
  else {
    dtostrf(h, 3, 2, buff1);
    dtostrf(t, 3, 2, buff2);
    sprintf(buffer, "%s;%s;7.00;800.59;20.3", buff1, buff2);
    Serial.println(buffer);
  }
  delay(1000);
}
