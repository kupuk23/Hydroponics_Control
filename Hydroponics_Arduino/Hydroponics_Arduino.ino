#include "DS3231.h"
#include "DHT.h"
#include <Wire.h>
#include <EEPROM.h>
#include <Nextion.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

//DECLARE DHT
#define DHTPIN 6
#define DHTTYPE DHT22

//DECLARE ULTRASONIC
#define trigPin 7 //Out
#define echoPin 8 //In

#define TIMER_INTERVAL_MS 1
#define LED_ON_TIME_MS 3000


#define SDA A4
#define SCL A5
#define SELECTOR 4
#define PUMP 9
#define LIMIT_SW 10
#define LAMP1 11
#define LAMP2 12
#define LAMP3 13
#define LED_STRIP 2

//byte set_year = 23;
//byte set_month = 03;
//byte set_date = 31;
//byte set_hour = 13;
//byte set_minute = 41;
//byte set_second = 0;

int hour, minute;
bool autoMode = true;

int exhaust_fan = 3; //PWM out pin 3

DS3231 myRTC;
DHT dht(DHTPIN, DHTTYPE);


const long interval = 500;
unsigned long currentMillis;
unsigned long previousMillis;


float temperature, humidity;
volatile bool ledOn = false;
volatile int interruptCounter = 0;
bool door_opened = false;
bool limitSW_state, selector_state;
char buffer[100] = {0};

/*
  [0][x] = Hour, [1][x] = Minute,
  x = [Start setpoint 1, end setpoint 1, start setpoint 2, end setpoint 2 ...]
*/
int setpoint[2][6] = {{0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0}
};



//NEXTION VARIABLE DECLARATIONS
const char st_SP1_h[] PROGMEM = "SP1_h";
const char st_SP1End_h[] PROGMEM = "SP1End_h";
const char st_SP2_h[] PROGMEM = "SP2_h";
const char st_SP2End_h[] PROGMEM = "SP2End_h";
const char st_SP3_h[] PROGMEM = "SP3_h";
const char st_SP3End_h[] PROGMEM = "SP3End_h";
const char st_SP1_m[] PROGMEM = "SP1_m";
const char st_SP1End_m[] PROGMEM = "SP1End_m";
const char st_SP2_m[] PROGMEM = "SP2_m";
const char st_SP2End_m[] PROGMEM = "SP2End_m";
const char st_SP3_m[] PROGMEM = "SP3_m";
const char st_SP3End_m[] PROGMEM = "SP3End_m";

const char *const nex_SPvar[][6] PROGMEM = {{st_SP1_h, st_SP1End_h, st_SP2_h, st_SP2End_h, st_SP3_h, st_SP3End_h},
  {st_SP1_m, st_SP1End_m, st_SP2_m, st_SP2End_m, st_SP3_m, st_SP3End_m}
};

int CurrentPage = 0;
char buffer2[40];
NexNumber nexSP1_h = NexNumber(1, 10, "SP1_h");
NexNumber nexSP1_m = NexNumber(1, 11, "SP1_m");
NexNumber nexSP1End_h = NexNumber(1, 12, "SP1End_h");
NexNumber nexSP1End_m = NexNumber(1, 13, "SP1End_m");
NexNumber nexSP2_h = NexNumber(1, 14, "SP2_h");
NexNumber nexSP2_m = NexNumber(1, 15, "SP2_m");
NexNumber nexSP2End_h = NexNumber(1, 16, "SP2End_h");
NexNumber nexSP2End_m = NexNumber(1, 17, "SP2End_m");
NexNumber nexSP3_h = NexNumber(1, 18, "SP3_h");
NexNumber nexSP3_m = NexNumber(1, 19, "SP3_m");
NexNumber nexSP3End_h = NexNumber(1, 20, "SP3End_h");
NexNumber nexSP3End_m = NexNumber(1, 21, "SP3End_m");
NexText nex_setpoints = NexText(1, 45, "setpoints");
NexButton bSetting = NexButton(0, 7, "bSetting");  // Button added
NexButton bBack = NexButton(1, 21, "bBack");       // Button added
NexButton bUpdate = NexButton(1, 26, "bUpdate");   // Button added
NexDSButton btLamp = NexDSButton(1, 37, "btLamp"); // Dual state button added
NexPage page0 = NexPage(0, 0, "page0"); // Page added as a touch event
NexPage page1 = NexPage(1, 0, "page1"); // Page added as a touch event

NexTouch *nex_listen_list[] =
{
  &bSetting, // Button added
  &bBack,    // Button added
  &bUpdate,  // Button added
  &btLamp,   // Button added
  &page0,    // Page added as a touch event
  &page1,    // Page added as a touch event
  NULL       // String terminated
};                 // End of touch event

// END OF NEXTION DECLARATION


//int incomingByte;
//String incomingStr = "a";
//String parsedHour, parsedMin;
//int ind1, ind2, hour_now, min_now;

uint32_t force_lamp = 0;
String str = "1,2,3,4,5,6,7,8";

void setup() {
  //TIMER ISR
  // Configure Timer 2 for CTC mode
  TCCR2A = (1 << WGM21);                  // Enable CTC mode
  TCCR2B = (1 << CS22); //| (1 << CS21);     // Set prescaler to 64
  OCR2A = TIMER_INTERVAL_MS * 250 - 1;    // Set OCR2A for a 1ms timer interval

  // Enable Timer 2 Compare A Match interrupt
  TIMSK2 = (1 << OCIE2A);
  sei();

  //Initialize PinModes

  pinMode(SELECTOR, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(LIMIT_SW, INPUT);
  pinMode(LAMP1, OUTPUT);
  pinMode(LAMP2, OUTPUT);
  pinMode(LAMP3, OUTPUT);
  pinMode(LED_STRIP, OUTPUT);
  pinMode(exhaust_fan, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(DHTPIN, INPUT);

  Wire.begin();
  Serial.begin(115200);  // Start serial comunication at baud=115200
  dht.begin();

  bSetting.attachPush(bSettingPushCallback);
  bUpdate.attachPush(bUpdatePushCallback);
  btLamp.attachPush(btLampPushCallback);
  page0.attachPush(page0PushCallback);  // Page press event
  page1.attachPush(page1PushCallback);  // Page press event

  //dht.begin();

  //Serial.println(F("Integration of DHT22, RTC , Selector, and Limit Switch"));


  //  Serial.print(myRTC.getYear(), DEC);
  //  Serial.print('/');
  //  Serial.print(myRTC.getMonth(century), DEC);
  //  Serial.print('/');
  //  Serial.print(myRTC.getDate(), DEC);
  //  Serial.print(' ');
  //  Serial.print(myRTC.getHour(h24, time_pm), DEC);
  //  Serial.print(':');
  //  Serial.print(myRTC.getMinute(), DEC);
  //  Serial.print(':');
  //  Serial.print(myRTC.getSecond(), DEC);
  //  Serial.println();

  // Serial.print(F("Humidity: "));
  // Serial.print(humidity);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(temperature);
  // Serial.println(F("°C "));

  //save_setPoint();
  //splitSP(str_setpoints);
  call_setPoint();
  delay(1000);
}




void loop() {


  currentMillis = millis();
  //temperature, humidity = measureDHT22();

  if (currentMillis - previousMillis >= interval) {
//    getTime();
    //    sprintf (buffer2, "%d/%d/%d\t%d:%d:%d", year, month, date, hour, minute, second);
    //    Serial.println(buffer2);
    measureDHT22();
    LEDcontrol();
    previousMillis = currentMillis;
  }

  checkMode();


  delay(50);
  nexLoop(nex_listen_list);
}



void checkMode() {
  limitSW_state = digitalRead(LIMIT_SW);
  selector_state = digitalRead(SELECTOR);

  if (selector_state) { //AUTO MODE
    if (!autoMode)
    {
      autoMode = true;
      //Serial.println("AUTO Mode");
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
      //Serial.println("MANUAL Mode, PUMP ALWAYS ON");

      digitalWrite(3, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(PUMP, HIGH);
      digitalWrite(LED_STRIP, LOW);
      door_opened = false;
      autoMode = false;
    }
  }
}


void getTime() {
  int year, month, date, second;
  bool century = false;
  bool h24 = false;
  bool time_pm = false;
  year = int(myRTC.getYear());
  month = int(myRTC.getMonth(century));
  date =  int(myRTC.getDate());
  hour =  int(myRTC.getHour(h24, time_pm));
  minute = int(myRTC.getMinute());
  second = int(myRTC.getSecond());
  if (CurrentPage == 0) nex_SendTime(year, month, date, hour, minute, second);
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
      //Serial.println("3 Seconds Elapsed!");
    }
  }
}
