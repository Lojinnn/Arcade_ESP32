#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <Fonts/FreeSans9pt7b.h>
#include <SPI.h>
#include <TFT_eSPI.h>
long tau = 0;
long buff = 0;

uint16_t text_x = 10;
uint16_t text_y = 40;
int i;

#define But_Wh 26
#define But_Bl 25
#define Sv 4
uint8_t But_state;

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  tft.init();

  pinMode(But_Wh, INPUT);
  pinMode(Sv, OUTPUT);

  digitalWrite(Sv, HIGH);

  tft.setRotation(0);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

}

void menu(){
  //buttonPressed(But_Wh);
  tft.setFreeFont(&FreeSans9pt7b);
  tft.setCursor(65, 5);
  tft.print("ARCADE_ESP32");
  tft.setCursor(text_x, text_y);
  tft.print("Star_shooter");
  if(But_state){
   // tft.clear();
    tft.drawRect(text_x-5, text_y-i, text_x+150, text_y-i-5, TFT_WHITE);
    
    i= i-20;

  }

}

void loop() {

  click(But_Wh);
  menu();
  Serial.println(buff);
  
}

void click(int Button){
  tau = millis();
  
  if(!digitalRead(Button)){
    buff = tau;
  }
}