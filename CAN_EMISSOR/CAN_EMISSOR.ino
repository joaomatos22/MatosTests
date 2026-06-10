#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>

#define CAN_TX_PIN GPIO_NUM_14
#define CAN_RX_PIN GPIO_NUM_34

#define Power_En1 4
#define IO15_3V3_EN 15

unsigned long lastSend = 0;

uint16_t canbus_speed = 250;  // Set canbus speed (250, 500, 750, 1000)

CanFrame can_rx_frame;  // Create frame to read
CanFrame can_tx_frame;  // Create frame to send

unsigned long interval = 0;
unsigned long interval2 = 0;
int change = 0;

uint32_t voltage_mV = 0;
uint32_t current_mA = 0;
unsigned long last = 0;
int Assist = 0;
unsigned long gear = 0;
uint8_t electronicGear;
uint8_t lastFrameData[8];
bool lampSwitch = false; //Para saber se tem que acender a lampa ou não. True para ligar, false para desligar
bool lampOn;
bool gear_;

//Para construir trama 0x200//////////////
uint8_t assistLevel = 0;   // bits 0-3
bool lampStatus = false;   // bit 4
bool autoGear = false;     // bit 5
uint8_t gearNumber = 0;    // 4 bits (6-9)
bool pushMode = false;     // bit 14

uint16_t currentSpeed = 0; // 0.01 km/h
uint16_t avgSpeed = 0;
uint16_t maxSpeed = 0;
unsigned long last200 = 0;
///////////////////////////////////////////

//Para decifrar 0x201//////////////////////
uint8_t assistPercentage = 0;
uint16_t humanTorque_raw = 0;
float humanTorque = 0;

uint8_t humanCadence = 0;

uint16_t humanPower_raw = 0;
float humanPower = 0;
///////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(Power_En1, OUTPUT);
  pinMode(IO15_3V3_EN, OUTPUT);

  digitalWrite(Power_En1, HIGH);  //Turn power 4V ON
  Serial.println("4V ON");
  digitalWrite(IO15_3V3_EN, HIGH);  //Turn power 3.3V ON
  Serial.println("3.3V ON");

  //CAN Setup functions
  ESP32Can.setPins(CAN_TX_PIN, CAN_RX_PIN);
  if (ESP32Can.begin(ESP32Can.convertSpeed(canbus_speed))) {
    Serial.println("CAN bus started!");
  } else {
    Serial.println("CAN bus failed!");
  }
  can_tx_frame.data_length_code = 8;

}

void loop() {
  readCAN();
  /*if (millis() - last200 >= 100)
  {
    send0x200();
    last200 = millis();
  }*/

  Interface();
  
  if(millis() - last >= 1000){
    Information();
    last = millis();
  }
}

void readCAN() {
  if (!ESP32Can.readFrame(can_rx_frame, 200)){ 
    //Serial.println("Não leu nada");
    return;
  } 
  //Serial.println(can_rx_frame.identifier,HEX);
  switch (can_rx_frame.identifier) {
    case 0x200: can_decode_0x200(); break;
    case 0x201: can_decode_0x201(); break;
    case 0x300: can_decode_0x300(); break;
    case 0x400: can_decode_0x400(); break;
    //case 0x402: can_decode_0x402(); break;
    //case 0x433: can_decode_0x433(); break;
    //case 0x110: can_decode_0x110(); break;
    default: break;
  }
}

void can_decode_0x200() {
  
  for (int i = 0; i < 8; i++) {
    lastFrameData[i] = can_rx_frame.data[i];
  }
  electronicGear = can_rx_frame.data[0] & 0x0F;
  lampOn = (can_rx_frame.data[0] >> 4) & 0x01;

}

void can_decode_0x201()
{
  if (can_rx_frame.data_length_code != 8) return;

  /*Serial.print("ID 0x201 | DATA: ");
  for (int i = 0; i < 8; i++) {
    Serial.printf("%02X ", can_rx_frame.data[i]);
  }
  Serial.println();*/

  // Byte 1 → Assistance %
  assistPercentage = can_rx_frame.data[0];

  // Bytes 2-3 → Human torque (U16 little endian)
  humanTorque_raw =
      ((uint16_t)can_rx_frame.data[2] << 8) |
       (uint16_t)can_rx_frame.data[1];

  humanTorque = humanTorque_raw * 0.01f;   // 0.01 Nm

  // Byte 4 → Cadence
  humanCadence = can_rx_frame.data[3];

  // Bytes 5-6 → Human power (U16 little endian)
  humanPower_raw =
      ((uint16_t)can_rx_frame.data[5] << 8) |
       (uint16_t)can_rx_frame.data[4];

  humanPower = humanPower_raw * 0.1f;   // 0.1 W
}

void can_decode_0x300() {

}

void can_decode_0x400() {
  if (can_rx_frame.data_length_code != 8) return;

  Serial.print("ID 0x");
  Serial.print(can_rx_frame.identifier, HEX);
  Serial.print(" | DLC=");
  Serial.println(can_rx_frame.data_length_code);

  for (int i = 0; i < can_rx_frame.data_length_code; i++) {
    Serial.printf("%02X ", can_rx_frame.data[i]);
  }
  Serial.println();


  voltage_mV =
    ((uint32_t)can_rx_frame.data[3] << 24) |
    ((uint32_t)can_rx_frame.data[2] << 16) |
    ((uint32_t)can_rx_frame.data[1] << 8)  |
    ((uint32_t)can_rx_frame.data[0]);

  current_mA =
    ((uint32_t)can_rx_frame.data[7] << 24) |
    ((uint32_t)can_rx_frame.data[6] << 16) |
    ((uint32_t)can_rx_frame.data[5] << 8)  |
    ((uint32_t)can_rx_frame.data[4]);

}
  
void Information(){
  Serial.print("Voltagem em mV: ");
  Serial.print(voltage_mV);
  Serial.print("..........");
  Serial.print("Corrente em mA: ");
  Serial.println(current_mA);
  Serial.print("Lamp: ");
  Serial.println(lampOn ? "ON" : "OFF");
  Serial.print("Gear: ");
  Serial.println(electronicGear);
  /*Serial.print("0X200: ");
  for (int i = 0; i < 8; i++) {
    Serial.printf("%02X ", lastFrameData[i]);
  }*/
  Serial.print("Assist %: ");
  Serial.println(assistPercentage);

  Serial.print("Torque (Nm): ");
  Serial.println(humanTorque);

  Serial.print("Cadence (rpm): ");
  Serial.println(humanCadence);

  Serial.print("Power (W): ");
  Serial.println(humanPower);
}