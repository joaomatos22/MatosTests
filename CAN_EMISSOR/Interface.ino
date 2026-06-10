String comando = "";
bool comandoCompleto = false;

void Interface(){
   while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {           // ENTER pressionado
      comandoCompleto = true;
    } else if (c != '\r') {    // Ignora CR
      comando += c;
    }
  }

  // Quando a linha estiver completa
  if (comandoCompleto) {

    comando.trim(); //Remove espaços

    if(comando == "Assist1"){
      assistLevel = 1;
      Serial.println("Assistência 1 mandada");
    } else if (comando == "Assist2") {
      assistLevel = 2;
      Serial.println("Assistência 2 mandada");
    } else if (comando == "Assist3") {
      assistLevel = 3;
      Serial.println("Assistência 3 mandada");
    } else if (comando == "LAMPON") {
      lampStatus = true;
    } else if (comando == "LAMPOFF"){
      lampStatus = false;
    } else{
      Serial.println("Comando Inválido");
    }
    comando = "";
    comandoCompleto = false;
  }

}