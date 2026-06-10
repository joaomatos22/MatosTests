#include <SPI.h>
#include <MFRC522.h>

// Pins (ajusta se necessário)
#define SS_PIN   27
#define RST_PIN  4

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() 
{
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("RFID Reader pronto. Aproxima um cartão...");
}

void loop() 
{
  // Verifica se há cartão
  if (!mfrc522.PICC_IsNewCardPresent() || 
      !mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  // Mostrar tamanho do UID
  Serial.print("UID size: ");
  Serial.print(mfrc522.uid.size);
  Serial.println(" bytes");

  // Converter para HEX contínuo
  char UIDString[30] = {0};

  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    char buffer[3];
    sprintf(buffer, "%02X", mfrc522.uid.uidByte[i]);
    strcat(UIDString, buffer);
  }

  Serial.print("UID HEX: ");
  Serial.println(UIDString);

  // garantir só 10 dígitos
  if (mfrc522.uid.size == 5)
  {
    Serial.println("✔ Cartão com 10 dígitos HEX");
  }
  else
  {
    Serial.println("✖ Não é 10 dígitos HEX");
  }

  Serial.println();

  delay(500); // pequeno debounce
}