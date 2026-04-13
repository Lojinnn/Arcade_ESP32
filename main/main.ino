#include "SmallText.h"

#include <SPI.h>
#include <TFT_eSPI.h>

#define SM_TXT SmallText

uint16_t text_x = 10;
uint16_t text_y = 40;

#define But_Wh 26
#define But_Bl 25
#define Sv 4
uint8_t But_state;

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();

  pinMode(But_Wh, INPUT);
  pinMode(Sv, OUTPUT);

  digitalWrite(Sv, HIGH);

  tft.setRotation(0);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //tft.setTextSize(1.5);


}

void menu(){
  buttonPressed(But_Wh);
  tft.loadFont(SM_TXT);
  tft.setCursor(65, 5);
  tft.print("ARCADE_ESP32");
  tft.setCursor(text_x, text_y);
  tft.print("Star_shooter");
  if(digitalRead(But_Wh)){
    tft.drawRect(text_x-5, text_y-5, text_x+50, text_y+10, TFT_WHITE);
    

    But_state = 0;
  }


  tft.unloadFont();

}

void loop() {

  menu();

  delay(15);
  
}

void buttonPressed(int But){
  long tau = millis();
  int buff;
  if(!digitalRead(But)){
    tau = buff;
  }
  if(digitalRead(But) and 1000 <= tau-buff){
    But_state = 1;
  }
}
