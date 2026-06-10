////////////////////////////////////////////////////////////////////
//Exemplo de envio de uma mensagem por CAN
////////////////////////////////////////////////////////////////////
void sendMessage (uint32_t voltage_mV, uint32_t current_mA) {

  CanFrame Message1 = {0};
  Message1.identifier = 0x110;
  Message1.extd             = 0;     // Standard 11-bit
  Message1.data_length_code = 8;

    // Bits 0–31 → tensão (mV)
  Message1.data[0] = (voltage_mV >> 24) & 0xFF;
  Message1.data[1] = (voltage_mV >> 16) & 0xFF;
  Message1.data[2] = (voltage_mV >> 8)  & 0xFF;
  Message1.data[3] =  voltage_mV        & 0xFF;

  // Bits 32–63 → corrente (mA)
  Message1.data[4] = (current_mA >> 24) & 0xFF;
  Message1.data[5] = (current_mA >> 16) & 0xFF;
  Message1.data[6] = (current_mA >> 8)  & 0xFF;
  Message1.data[7] =  current_mA        & 0xFF;

  ESP32Can.writeFrame(Message1);
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//Trocar assistência do motor
/////////////////////////////////////////////////////////////////////
void sendAssist(int gear) {
  CanFrame f = {0};
  f.identifier = 0x300;
  f.extd = 0;
  f.data_length_code = 8;
  if(gear == 1){
    //Serial.println("Ajuda 1");
    f.data[0] = 0x01;
    f.data[1] = 0x28;                 // assist level válido
    for (int i = 2; i < 8; i++) f.data[i] = 0x00;
  }
  if(gear == 2){
    //Serial.println("Ajuda 2");
    f.data[0] = 0x02;                 // assist level válido
    f.data[1] = 0x28;
    for (int i = 2; i < 8; i++) f.data[i] = 0x00;
  }
  if(gear == 3){
    //Serial.println("Ajuda 3");
    f.data[0] = 0x03;                 // assist level válido
    f.data[1] = 0x28;
    for (int i = 2; i < 8; i++) f.data[i] = 0x00;
  }

  ESP32Can.writeFrame(f);
  //Serial.println("Frame enviado");

  /*if(gear == 1){
    CanFrame txMotor;
    txMotor.identifier = 0x200;
    txMotor.extd = 0;
    txMotor.data_length_code = 8;
    txMotor.data[0] = gear & 0x0F;
    for (int i = 1; i < 8; i++) txMotor.data[i] = 0;

    ESP32Can.writeFrame(txMotor);
  }
    
  if(gear == 2){
    CanFrame txMotor;
    txMotor.identifier = 0x200;
    txMotor.extd = 0;
    txMotor.data_length_code = 8;
    txMotor.data[0] = gear & 0x0F;
    for (int i = 1; i < 8; i++) txMotor.data[i] = 0;

    ESP32Can.writeFrame(txMotor);
  }

  if(gear == 3){
    CanFrame txMotor;
    txMotor.identifier = 0x200;
    txMotor.extd = 0;
    txMotor.data_length_code = 8;
    txMotor.data[0] = gear & 0x0F;
    for (int i = 1; i < 8; i++) txMotor.data[i] = 0;

    ESP32Can.writeFrame(txMotor);
  }*/


}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//Acender lâmpada
//////////////////////////////////////////////////////////////////
void switchlamp(){
  CanFrame f = {0};
  f.identifier = 0x300;
  f.extd = 0; 
  f.data_length_code = 8;

  Serial.println("Acender lâmpada");
  f.data[0] = 0x10;
  f.data[1] = 0x28;
  for (int i = 2; i < 8; i++) f.data[i] = 0x00;
  ESP32Can.writeFrame(f);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void switchlampController(){
  CanFrame f = {0};
  f.identifier = 0x200;
  f.extd = 0;
  f.data_length_code = 8;

  f.data[0] = 0x10;
  f.data[1] = 0x01;
  for (int i = 2; i < 8; i++) f.data[i] = 0x00;
  ESP32Can.writeFrame(f);
  Serial.println("Controlador lâmpada");
}

void send0x200()
{
    CanFrame f = {0};
    f.identifier = 0x200;
    f.extd = 0;
    f.data_length_code = 8;

    // ---------- BYTE 1 ----------
    f.data[0] = 0;

    f.data[0] |= (assistLevel & 0x0F);          // bits 0-3
    f.data[0] |= (lampStatus << 4);             // bit 4
    f.data[0] |= (autoGear << 5);               // bit 5
    f.data[0] |= ((gearNumber & 0x03) << 6);    // bits 6-7

    // ---------- BYTE 2 ----------
    f.data[1] = 0;
    f.data[1] |= ((gearNumber >> 2) & 0x03);    // bits 8-9
    f.data[1] |= (pushMode << 6);               // bit 14

    // ---------- Velocidades (Little Endian) ----------
    f.data[2] = currentSpeed & 0xFF;
    f.data[3] = currentSpeed >> 8;

    f.data[4] = avgSpeed & 0xFF;
    f.data[5] = avgSpeed >> 8;

    f.data[6] = maxSpeed & 0xFF;
    f.data[7] = maxSpeed >> 8;

    ESP32Can.writeFrame(f);
}