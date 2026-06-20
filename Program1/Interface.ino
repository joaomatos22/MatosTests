String c;

void interface(){
  if(Serial.available()){

    c = Serial.readString();
    c.trim();

  }

  if (c == "time"){
    obtaintime();
  }

  if(c == "RED"){
    pixel.setPixelColor(0, pixel.Color(255, 0, 0));
    pixel.show();
  }

  if(c == "GREEN"){
    pixel.setPixelColor(0, pixel.Color(0, 255, 0));
    pixel.show();
  }

  if(c == "BLUE"){
    pixel.setPixelColor(0, pixel.Color(0, 0, 255));
    pixel.show();
  }
  c = "";
}