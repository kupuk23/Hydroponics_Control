
#include <Nextion.h>

int CurrentPage = 0;
NexButton bSetting = NexButton(0, 7, "bSetting");  // Button added
NexButton bBack = NexButton(1, 21, "bBack");       // Button added
NexButton bUpdate = NexButton(1, 26, "bUpdate");   // Button added
NexDSButton btLamp = NexDSButton(1, 38, "btLamp"); // Dual state button added
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
NexText nexTime_h= NexText(0,12,"hour");
NexText nexTime_m= NexText(0,13,"min");
NexText nexTime_s= NexText(0,14,"sec");
NexText nexDate_day= NexText(0,8,"day");
NexText nexDate_month= NexText(0,9,"month");
NexText nexDate_year= NexText(0,10,"year");


NexPage page0 = NexPage(0, 0, "page0");  // Page added as a touch event
NexPage page1 = NexPage(1, 0, "page1");  // Page added as a touch event
//NexPage page2 = NexPage(2, 0, "page2");  // Page added as a touch event


char buffer[100] = {0}; 

NexTouch *nex_listen_list[] = 
{
  &bSetting,  // Button added
  &bBack,  // Button added
  &bUpdate,  // Button added
  &btLamp,  // Button added
  &page0,  // Page added as a touch event
  &page1,  // Page added as a touch event
  NULL  // String terminated
};  // End of touch event 

void setup()
{
  // put your setup code here, to run once:
}

void loop()
{
  // put your main code here, to run repeatedly:
}
