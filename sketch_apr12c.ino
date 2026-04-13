#include "SmallText.h"

#include <SPI.h>
#include <TFT_eSPI.h>

#define SM_TXT SmallText

#define But_Wh 26
#define But_Bl 25
#define Sv 4

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
  tft.setTextSize(1.5);


}

void loop() {

  tft.setCursor(1, 100);
  tft.print("This is test text");
  tft.setCursor(1, 150);
  tft.setTextSize(2);
  tft.print("This is test text");
  tft.setCursor(1, 200);
  tft.setTextSize(1);
  tft.print("This is test text");
  
  tft.setCursor(1, 50);
  tft.loadFont(SM_TXT);
  tft.print("This is test text");
  tft.unloadFont();

  delay(10);
  
}
