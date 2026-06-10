#include <Arduino.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>    //https://github.com/plerup/espsoftwareserial

/****************************************************************
 Structures received from the Extender board with it's Status: */

struct BatteryStruct {
  int TotalVoltage;          // Value read from the Serial port of battery module in decimes of voltage (Example: 40,3V is 403dV)
  int ElementVoltage[16];    // Voltage of each battery element in mV
  int TemperatureSensors[2]; // Temperature in each sensor in decimes of Centigrate degree (Example: 12,3C is 123)
  byte WarningLevel[3];      // There are 3 levels of warning. Each is represented by one byte
  int Current;               // Current in delimes of Ampere
  byte WorkMode;             // Working mode (Idle=?? or Status=??)
  byte kp;                   // ??
  byte km;                   // ??
  byte k_;                   // ??
};
BatteryStruct BatteryStatus;

SoftwareSerial SerialToExtenderBrd;

//************** JSON API Declarations
StaticJsonDocument<1000> jsonDocument; // JSON data buffer
//char buffer[1000];
//**********************************

#define RXD3         16  // Used in Serial port for Battery (Input from ESP32)
#define TXD3         17  // Used in Serial port for Battery (Output from ESP32)

unsigned long TimeOut1;
char IoTMessageBuffer[1000];

void setup()
{
  Serial.begin(115200);           // start serial for output
  SerialToExtenderBrd.begin(115200, SWSERIAL_8N1, RXD3, TXD3, false, 95, 11);
  TimeOut1=millis()+500;
  randomSeed(analogRead(0));

  BatteryStatus.TotalVoltage=0;

  for(int i=0;i<10;i++)
  {
    BatteryStatus.ElementVoltage[i]=random(3300,3350);
    BatteryStatus.TotalVoltage+=BatteryStatus.ElementVoltage[i];
  }
  for(int i=10;i<16;i++)
    BatteryStatus.ElementVoltage[i]=0;
  
  BatteryStatus.TemperatureSensors[0]=165;
  BatteryStatus.TemperatureSensors[1]=214;
  BatteryStatus.WarningLevel[0]=2;
  BatteryStatus.WarningLevel[1]=2;
  BatteryStatus.WarningLevel[2]=2;
  BatteryStatus.Current=34;
  BatteryStatus.WorkMode=1;
  BatteryStatus.kp=2;
  BatteryStatus.km=1;
  BatteryStatus.k_=2;
}

void loop()
{
  if(TimeOut1<millis())
  {
    SendBatteryData();
    TimeOut1=millis()+500;
  }
}

//******************************
//*******************************
void SendBatteryData()
{
  int n;
  int maximum;
  int maximumIndex;
  int minimum;
  int minimumIndex;
  String CellId;
  
  jsonDocument.clear();
  jsonDocument["total"] = BatteryStatus.TotalVoltage;
  
  maximum=BatteryStatus.ElementVoltage[0];
  minimum=BatteryStatus.ElementVoltage[0];
  maximumIndex=0;
  minimumIndex=0;
  
  for(n=0;n<16;n++)
  {
    if( (minimum>BatteryStatus.ElementVoltage[n]) && (BatteryStatus.ElementVoltage[n]!=0) )
    {
      minimum=BatteryStatus.ElementVoltage[n];
      minimumIndex=n;
    }
    if(maximum<BatteryStatus.ElementVoltage[n])
    {
      maximum=BatteryStatus.ElementVoltage[n];
      maximumIndex=n;
    }
  }

  jsonDocument["max"] = String(maximum)+" in "+String(maximumIndex+1);
  jsonDocument["min"] = String(minimum)+" in "+String(minimumIndex+1);
  
  for(n=0;n<16;n++)
  {
    CellId="Cell"+String(n+1);
    jsonDocument[CellId] = BatteryStatus.ElementVoltage[n];
  } 

  jsonDocument["temp1"] = BatteryStatus.TemperatureSensors[0];
  jsonDocument["temp2"] = BatteryStatus.TemperatureSensors[1];
  
  jsonDocument["lv1"] = BatteryStatus.WarningLevel[0];
  jsonDocument["lv2"] = BatteryStatus.WarningLevel[1];
  jsonDocument["lv3"] = BatteryStatus.WarningLevel[2];

  jsonDocument["current"] = BatteryStatus.Current;

  jsonDocument["workmode"] = BatteryStatus.WorkMode;

  jsonDocument["kp"] = BatteryStatus.kp;
  jsonDocument["km"] = BatteryStatus.km;
  jsonDocument["k_"] = BatteryStatus.k_;
  
  serializeJson(jsonDocument, IoTMessageBuffer);

  Serial.print("Sending ");
  Serial.println(IoTMessageBuffer);
  SerialToExtenderBrd.println(IoTMessageBuffer);
}
