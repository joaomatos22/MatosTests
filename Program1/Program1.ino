#include <WiFi.h>
#include <time.h>
#include <Adafruit_NeoPixel.h>

unsigned long WifiVerification = 0;

// Fuso horário de Portugal Continental
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600; // horário de verão

struct tm timeinfo;
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//LED
#define RGB_PIN 38      
#define NUMPIXELS 1
Adafruit_NeoPixel pixel(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
///////////////////////////////////////////////////////////


void setup() {

  Serial.begin(115200);
  BeginWifi();
  configTime(gmtOffset_sec, daylightOffset_sec,
  "pool.ntp.org",
  "time.nist.gov");
  pixel.begin();           
}

void loop() {
  interface();
  
  if(millis() - WifiVerification > 10000){
    checkWifi();
  }
}
