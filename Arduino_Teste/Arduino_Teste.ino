#include <ArduinoJson.h>


int seg[] = {2,3,4,5,6,7,8};
int ValorRand;
unsigned long bat = 0;
unsigned long discharge;

#define LED_RED 10
#define LED_YELLOW 11
#define LED_GREEN 12

//////////////////////////////////Serials///////////////////////////////////////////////
#define SerialESP Serial
////////////////////////////////////////////////////////////////////////////////////////

//JSON/////////////////////////////////////////////////////////////////////////////////////////////////////////
StaticJsonDocument<256> jsonDocument;    // JSON data buffer
char buffer[256];
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  SerialESP.begin(9600);
  for (int i = 0; i < 7; i++){
    pinMode(seg[i], OUTPUT);
    digitalWrite(seg[i],LOW);
  }

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  Convertion();
  discharge = millis();
}

void loop() {
  
  if(bat < 30000){
    Convertion();
  }

  if(bat > 30000 && bat <= 33000){
    digitalWrite(LED_RED,HIGH);
    digitalWrite(LED_YELLOW,LOW);
    digitalWrite(LED_GREEN,LOW);
  } else if (bat > 33000 && bat <= 36000){
    digitalWrite(LED_RED,LOW);
    digitalWrite(LED_YELLOW,HIGH);
    digitalWrite(LED_GREEN,LOW);
  } else if (bat > 36000 && bat <= 40000){
    digitalWrite(LED_RED,LOW);
    digitalWrite(LED_YELLOW,LOW);
    digitalWrite(LED_GREEN,HIGH);
  } else{
    digitalWrite(LED_RED,LOW);
    digitalWrite(LED_YELLOW,LOW);
    digitalWrite(LED_GREEN,LOW);
  }

  if(bat >= 30000 && bat < 31000){
   digitalWrite(seg[0], LOW);
   digitalWrite(seg[1], HIGH);
   digitalWrite(seg[2], HIGH);
   digitalWrite(seg[3], HIGH);
   digitalWrite(seg[4], HIGH);
   digitalWrite(seg[5], HIGH);
   digitalWrite(seg[6], HIGH);      
  } else if (bat >= 31000 && bat < 32000){
    digitalWrite(seg[0], LOW);
    digitalWrite(seg[1], LOW);
    digitalWrite(seg[2], LOW);
    digitalWrite(seg[3], HIGH);
    digitalWrite(seg[4], LOW);
    digitalWrite(seg[5], LOW);
    digitalWrite(seg[6], HIGH);       
  } else if (bat >= 32000 && bat < 33000){
    digitalWrite(seg[0], HIGH);
    digitalWrite(seg[1], LOW);
    digitalWrite(seg[2], HIGH);
    digitalWrite(seg[3], HIGH);
    digitalWrite(seg[4], HIGH);
    digitalWrite(seg[5], HIGH);
    digitalWrite(seg[6], LOW); 
  } else if (bat >= 33000 && bat < 34000){
    digitalWrite(seg[0], HIGH);
    digitalWrite(seg[1], LOW);
    digitalWrite(seg[2], HIGH);
    digitalWrite(seg[3], HIGH);
    digitalWrite(seg[4], LOW);
    digitalWrite(seg[5], HIGH);
    digitalWrite(seg[6], HIGH); 
  } else if (bat >= 34000 && bat < 35000){
    digitalWrite(seg[0], HIGH);
    digitalWrite(seg[1], HIGH);
    digitalWrite(seg[2], LOW);
    digitalWrite(seg[3], HIGH);
    digitalWrite(seg[4], LOW);
    digitalWrite(seg[5], LOW);
    digitalWrite(seg[6], HIGH); 
  } else if (bat >= 35000 && bat < 36000){
    digitalWrite(seg[0], HIGH);
    digitalWrite(seg[1], HIGH);
    digitalWrite(seg[2], HIGH);
    digitalWrite(seg[3], LOW);
    digitalWrite(seg[4], LOW);
    digitalWrite(seg[5], HIGH);
    digitalWrite(seg[6], HIGH);
  } else if (bat >= 36000 && bat < 37000){
    digitalWrite(seg[0], HIGH);
    digitalWrite(seg[1], HIGH);
    digitalWrite(seg[2], HIGH);
    digitalWrite(seg[3], LOW);
    digitalWrite(seg[4], HIGH);
    digitalWrite(seg[5], HIGH);
    digitalWrite(seg[6], HIGH); 
  } else if (bat >= 37000 && bat < 38000){
    digitalWrite(seg[0], LOW);
    digitalWrite(seg[1], LOW);
    digitalWrite(seg[2], HIGH);
    digitalWrite(seg[3], HIGH);
    digitalWrite(seg[4], LOW);
    digitalWrite(seg[5], LOW);
    digitalWrite(seg[6], HIGH); 
  } else if (bat >= 38000 && bat < 39000){
    digitalWrite(seg[0], HIGH);
    digitalWrite(seg[1], HIGH);
    digitalWrite(seg[2], HIGH);
    digitalWrite(seg[3], HIGH);
    digitalWrite(seg[4], HIGH);
    digitalWrite(seg[5], HIGH);
    digitalWrite(seg[6], HIGH); 
  } else if (bat >= 39000 && bat < 40000){
    digitalWrite(seg[0], HIGH);
    digitalWrite(seg[1], HIGH);
    digitalWrite(seg[2], HIGH);
    digitalWrite(seg[3], HIGH);
    digitalWrite(seg[4], LOW);
    digitalWrite(seg[5], LOW);
    digitalWrite(seg[6], HIGH); 
  } else {
    digitalWrite(seg[0], LOW);
    digitalWrite(seg[1], HIGH);
    digitalWrite(seg[2], HIGH);
    digitalWrite(seg[3], HIGH);
    digitalWrite(seg[4], HIGH);
    digitalWrite(seg[5], HIGH);
    digitalWrite(seg[6], HIGH); 
  }
  if (millis() - discharge > 5000){
    bat = bat - 100;
    //Serial.println(bat);
    discharge = millis();
  }
  
  jsonDocument.clear();
  jsonDocument["Command"] = "UNO";
  jsonDocument["Battery"] = bat;
  serializeJson(jsonDocument,SerialESP);

  SerialESP.println();   // importante: '\n'

  //Serial.println("JSON enviado");

  delay(1000);
}
