const char* SSID = "";
const char* password = "";

void BeginWifi(){
  int count = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, password);

  Serial.print("A ligar: ");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count ++;
    if (count > 5){
      ESP.restart();
    }
  }

  Serial.println("\nWiFi ligado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void checkWifi(){
  if(WiFi.status() != WL_CONNECTED){
    BeginWifi();
  }
}

void obtaintime(){
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Falha ao obter hora");
    return;
  }

  Serial.print("Data/Hora: ");
  Serial.println(&timeinfo, "%d/%m/%Y %H:%M:%S");
}