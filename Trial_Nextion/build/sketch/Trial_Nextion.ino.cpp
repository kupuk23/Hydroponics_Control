#include <Arduino.h>
#line 1 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"

#include <Nextion.h>

#include <DS3231.h>
#include <Wire.h>
#define SDA A4
#define SCL A5

DS3231 myRTC;

int year = 0;
int month = 0;
int date = 0;
int hour = 0;
int minute = 0;
int second = 0;

int CurrentPage = 0;
NexButton bSetting = NexButton(0, 7, "bSetting");  // Button added
NexButton bBack = NexButton(1, 21, "bBack");       // Button added
NexButton bUpdate = NexButton(1, 26, "bUpdate");   // Button added
NexDSButton btLamp = NexDSButton(1, 38, "btLamp"); // Dual state button added
NexPage page0 = NexPage(0, 0, "page0"); // Page added as a touch event
NexPage page1 = NexPage(1, 0, "page1"); // Page added as a touch event

char buffer[100] = {0};
int setpoint[2][6] = {{1, 2, 3, 4, 5, 6},
  {7, 8, 9, 10, 11, 12}
};

char nex_SPvar[2][6][10] = {{"SP1_h", "SP1_m", "SP2_h", "SP2_m", "SP3_h", "SP3_m"},
  {"SP1End_h", "SP1End_m", "SP2End_h", "SP2End_m", "SP3End_h", "SP3End_m"}
};

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

byte set_year = 23;
byte set_month = 03;
byte set_date = 31;
byte set_hour = 13;
byte set_minute = 41;
byte set_second = 0;
bool century = false;
bool h24 = false;
bool time_pm = false;
bool setP_sent = false;
const long interval = 1000;
unsigned long currentMillis;
unsigned long previousMillis;
char buffer2[40];


#line 62 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"
void nex_sendSP();
#line 85 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"
void clearNextionSerial();
#line 91 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"
void bSettingPushCallback(void *ptr);
#line 97 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"
void bUpdatePushCallback(void *ptr);
#line 108 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"
void btLampPushCallback(void *ptr);
#line 114 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"
void page0PushCallback(void *ptr);
#line 121 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"
void page1PushCallback(void *ptr);
#line 129 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"
void setup();
#line 185 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"
void nex_SendTime(int yr, int m, int d, int h, int mnt, int s);
#line 211 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"
void loop();
#line 62 "D:\\TAFARREL\\Hydroponics\\Hydroponics_Control\\Trial_Nextion\\Trial_Nextion.ino"
void nex_sendSP() {


  int j = 6;
  for (int i = 0; i < 6; i++) {
    memset(buffer2, 0, sizeof(buffer2));
    //setpoint[0][i] = EEPROM.read(i + 1); //read hour
    sprintf (buffer2, "%s.val=%d", nex_SPvar[0][i], setpoint[0][i] );
    Serial.print(buffer2);
    clearNextionSerial();


    //setpoint[1][i] = EEPROM.read(j + 1); //read minute
    sprintf (buffer2, "%s.val=%d", nex_SPvar[0][j], setpoint[0][j] );
    Serial.print(buffer2);
    clearNextionSerial();
    j++;
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
  int num = 10;
  //  sprintf (buffer2, "SP1_m.val=%d", num);
  //  Serial.print(buffer2);
  //  clearNextionSerial();
  Serial.print("n3.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  Serial.print(num);  // This is the value you want to send to that object and atribute mentioned before.
  clearNextionSerial();
}  // End of press event

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




void setup()
{
  Wire.begin();

  Serial.begin(9600);  // Start serial comunication at baud=9600


  // I am going to change the Serial baud to a faster rate.
  // The reason is that the slider have a glitch when we try to read it"s value.
  // One way to solve it was to increase the speed of the serial port.
  delay(500);  // This dalay is just in case the nextion display didn't start yet, to be sure it will receive the following command.
  Serial.print("baud=115200");  // Set new baud rate of nextion to 115200, but it's temporal. Next time nextion is power on,
  // it will retore to default baud of 9600.
  // To take effect, make sure to reboot the arduino (reseting arduino is not enough).
  // If you want to change the default baud, send the command as "bauds=115200", instead of "baud=115200".
  // If you change the default baud, everytime the nextion is power ON is going to have that baud rate, and
  // would not be necessery to set the baud on the setup anymore.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to nextion.
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.end();  // End the serial comunication of baud=9600

  Serial.begin(115200);  // Start serial comunication at baud=115200
  //Serial.println(F("Integration of DHT22, RTC , Selector, and Limit Switch"));
  //    myRTC.setYear(set_year);
  //    myRTC.setMonth(set_month);
  //    myRTC.setDate(set_date);
  //    myRTC.setHour(set_hour);
  //    myRTC.setMinute(set_minute);
  //    myRTC.setSecond(set_second);
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

  bSetting.attachPush(bSettingPushCallback);
  bUpdate.attachPush(bUpdatePushCallback);
  btLamp.attachPush(btLampPushCallback);
  page0.attachPush(page0PushCallback);  // Page press event
  page1.attachPush(page1PushCallback);  // Page press event

}





void nex_SendTime(int yr, int m, int d, int h, int mnt, int s) {
  memset(buffer2, 0, sizeof(buffer2));
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



void loop()
{
  // put your main code here, to run repeatedly:
  currentMillis = millis();

  if (CurrentPage == 1) {

    //    int num = 10;
    //    memset(buffer2, 0, sizeof(buffer2));
    //    sprintf(buffer2, "va0.val=%d", num);
    //    Serial.print(buffer2);
    //    clearNextionSerial();

  }

  if (currentMillis - previousMillis >= interval) {
    //now = myRTC.now();
    // Serial.print(F("Humidity: "));
    // Serial.print(humidity);
    // Serial.print(F("%  Temperature: "));
    // Serial.print(temperature);
    // Serial.println(F("°C "));
    //Serial.println(setpoint[1][1]);
    //LEDcontrol();
    year = int(myRTC.getYear());
    month = int(myRTC.getMonth(century));
    date =  int(myRTC.getDate());
    hour =  int(myRTC.getHour(h24, time_pm));
    minute = int(myRTC.getMinute());
    second = int(myRTC.getSecond());
    previousMillis = currentMillis;
    if (CurrentPage == 0) nex_SendTime(year, month, date, hour, minute, second);
    //
    //        sprintf (buffer2, "%d/%d/%d\t%d:%d:%d", year, month, date, hour, minute, second);
    //    Serial.println(buffer2);
  }

  delay(30);
  nexLoop(nex_listen_list);
}

