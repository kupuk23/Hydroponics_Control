#include "DS3231.h"
#include "DHT.h"
#include <Wire.h>
#include <EEPROM.h>


#define TIMER_INTERVAL_MS 1
#define LED_ON_TIME_MS 3000

#define SDA A4
#define SCL A5
#define DHTPIN 5
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define SELECTOR 4
#define PUMP 9
#define LIMIT_SW 10
#define LAMP1 11
#define LAMP2 12
#define LAMP3 13
#define LED_STRIP 2

byte year;
byte month;
byte date;
byte dOW;
byte hour;
byte minute;
byte second;
bool autoMode = true;

int exhaust_fan = 3; //PWM out pin 3

DS3231 myRTC;

const long interval = 1000;
unsigned long currentMillis;
unsigned long previousMillis;


float temperature, humidity;
volatile bool ledOn = false;
volatile int interruptCounter = 0;
bool door_opened = false;
bool limitSW_state, selector_state;

int setpoint[12];  // (Start setpoint 1, end setpoint 1, start setpoint 2, end setpoint 2 ...)
int incomingByte;
String incomingStr = "a";
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  //TIMER ISR
  // Configure Timer 2 for CTC mode
  TCCR2A = (1 << WGM21);                  // Enable CTC mode
  TCCR2B = (1 << CS22); //| (1 << CS21);     // Set prescaler to 64
  OCR2A = TIMER_INTERVAL_MS * 250 - 1;    // Set OCR2A for a 1ms timer interval

  // Enable Timer 2 Compare A Match interrupt
  TIMSK2 = (1 << OCIE2A);
  sei();



  pinMode(SELECTOR, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(LIMIT_SW, INPUT);
  pinMode(LAMP1, OUTPUT);
  pinMode(LAMP2, OUTPUT);
  pinMode(LAMP3, OUTPUT);
  pinMode(LED_STRIP, OUTPUT);
  pinMode(exhaust_fan, OUTPUT);
  pinMode(3, OUTPUT);



  Serial.begin(115200);
  Serial.setTimeout(100);
  Wire.begin();
  dht.begin();

  Serial.println(F("Integration of DHT22, RTC , Selector, and Limit Switch"));
  DateTime now = RTClib::now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  temperature, humidity = measureDHT22();

  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.println(F("°C "));

  save_setPoint();
  call_setPoint();
}


void loop() {
  if (Serial.available() > 0) {
    delay(100);
    // read the incoming byte:

    incomingStr = Serial.readStringUntil('\n');
    incomingByte = incomingStr.toInt();
    Serial.println(incomingByte);
  }


  currentMillis = millis();
  temperature, humidity = measureDHT22();

  if (currentMillis - previousMillis >= interval) {
    //now = myRTC.now();
    // Serial.print(F("Humidity: "));
    // Serial.print(humidity);
    // Serial.print(F("%  Temperature: "));
    // Serial.print(temperature);
    // Serial.println(F("°C "));
    LEDcontrol();
    previousMillis = currentMillis;
  }

  checkMode();


  // Wait a few seconds between measurements.
  delay(100);
}



void checkMode() {
  limitSW_state = digitalRead(LIMIT_SW);
  selector_state = digitalRead(SELECTOR);

  if (selector_state) { //AUTO MODE
    if (!autoMode)
    {
      autoMode = true;
      Serial.println("AUTO Mode");
      digitalWrite(3, HIGH);
      digitalWrite(6, LOW);
    }
    if (limitSW_state) {
      if (!door_opened) {
        digitalWrite(PUMP, LOW);
        LEDWarning();
        door_opened = true;
      }
    }
    else {
      //Serial.println("Limit Off");
      digitalWrite(PUMP, HIGH);
      digitalWrite(LED_STRIP, LOW);
      door_opened = false;
    }

  }
  else { //MANUAL MODE
    if (autoMode) {
      Serial.println("MANUAL Mode, PUMP ALWAYS ON");

      digitalWrite(3, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(PUMP, HIGH);
      digitalWrite(LED_STRIP, LOW);
      door_opened = false;
      autoMode = false;
    }
  }
}


void LEDWarning() {
  digitalWrite(LED_STRIP, HIGH);
  ledOn = true;
  interruptCounter = 0;
}

ISR(TIMER2_COMPA_vect) {
  // Check if LED is on
  if (ledOn == true) {
    //Serial.println(currentMillis);

    // Increment interrupt counter
    interruptCounter++;

    // Check if on-time has elapsed
    if (interruptCounter >= LED_ON_TIME_MS / TIMER_INTERVAL_MS) {
      // Turn off LED and reset interrupt counter
      digitalWrite(LED_STRIP, LOW);
      ledOn = false;
      interruptCounter = 0;
      Serial.println("3 Seconds Elapsed!");
    }
  }
}