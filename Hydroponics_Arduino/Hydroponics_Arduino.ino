#include "DS3231.h"
#include <Wire.h>
#include <EEPROM.h>
#include <Nextion.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(7, 6);
#define TIMER_INTERVAL_MS 1
#define LED_ON_TIME_MS 3000


#define SDA A4
#define SCL A5
#define SELECTOR 4
#define PUMP 9
#define LIMIT_SW 10
#define LAMP1 11
#define LAMP2 12
#define LAMP3 5 //D13 pindah ke 5
#define LED_STRIP 2
#define exhaust_fan 3
// byte set_year = 23;
// byte set_month = 03;
// byte set_date = 31;
// byte set_hour = 13;
// byte set_minute = 41;
// byte set_second = 0;

int hour, minute;
bool autoMode = true;


DS3231 myRTC;

String temp, humid, tds, ph, dist;
const long interval = 500;
unsigned long currentMillis;
unsigned long previousMillis;


volatile bool ledOn = false;
volatile int interruptCounter = 0;
bool door_opened = false;
bool limitSW_state, selector_state;
char buffer[40] = {0};
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
NexText nex_setpoints = NexText(1, 44, "setpoints");
NexButton bSetting = NexButton(0, 7, "bSetting");  // Button added
NexButton bBack = NexButton(1, 22, "bBack");       // Button added
NexButton bUpdate = NexButton(1, 57, "bUpdate");   // Button added
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

  Wire.begin();
  Serial.begin(115200);  // Start serial comunication at baud=115200
  Serial.println("starting...");
  espSerial.begin(9600);
  bSetting.attachPush(bSettingPushCallback);
  bUpdate.attachPush(bUpdatePushCallback);
  btLamp.attachPush(btLampPushCallback);
  page0.attachPush(page0PushCallback);  // Page press event
  page1.attachPush(page1PushCallback);  // Page press event
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

  //save_setPoint(); //TO RESET EEPROM VALUE
  call_setPoint();
  delay(1000);
}




void loop() {

  currentMillis = millis();
  if (espSerial.available()) {
    String receivedString = espSerial.readStringUntil('\n');  // Read the string until a newline character is received
    //    Serial.println(receivedString);                          // Print the received string on Serial Monitor
    getSensorVal(receivedString);
  }

  if (currentMillis - previousMillis >= interval) {
    getTime();
    if (CurrentPage == 0) nex_SendData();
    LEDcontrol();
    previousMillis = currentMillis;

  }

  checkMode();
  //Serial.println(digitalRead(LED_STRIP));


  delay(10);
  nexLoop(nex_listen_list);
}



void checkMode() {
  limitSW_state = digitalRead(LIMIT_SW);
  selector_state = digitalRead(SELECTOR);

  if (selector_state) { //AUTO MODE
    //Serial.println("AUTO Mode");
    if (!autoMode)
    {
      autoMode = true;
      digitalWrite(3, !HIGH);
      digitalWrite(6, !LOW);
      //Serial.println("AUTO");
    }

    if (limitSW_state) {
      //Serial.println("LIMIT ON");
      if (!door_opened) {
        digitalWrite(PUMP, !LOW);
        LEDWarning();
        door_opened = true;
      }
    }
    else {
      //Serial.println("Limit Off");
      digitalWrite(PUMP, !HIGH);
      digitalWrite(LED_STRIP, !LOW);
      door_opened = false;
    }

  }
  else { //MANUAL MODE
    //Serial.println("MANUAL Mode, PUMP ALWAYS ON");
    digitalWrite(3, !LOW);
    digitalWrite(6, !HIGH);
    digitalWrite(PUMP, !HIGH);
    digitalWrite(LED_STRIP, !LOW);
    door_opened = false;
    autoMode = false;
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
  digitalWrite(LED_STRIP, !HIGH);
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
      digitalWrite(LED_STRIP, !LOW);
      ledOn = false;
      interruptCounter = 0;
      //Serial.println("3 Seconds Elapsed!");
    }
  }
}
