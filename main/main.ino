#include <Bounce2.h>

#include <SPI.h>
#include <TFT_eSPI.h>

uint16_t text_h = 20;
uint16_t start_text_y = 40;
uint8_t sel_item = 0;

int now_scr = 0;
const char* SCREEN[] = {"MENU", "STAR_SHOOTER", "OPTIONS", "HIGH_SCORES"};
const char* Menu_str[] = {"Star shooter", "Options", "Your game statistics"};
Bounce But_UP, But_DOWN, But_RIGHT, But_LEFT;

#define But_Wh 26
#define But_Bl 25
#define Sv 4

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  tft.init();

  But_UP.attach(But_Wh, INPUT);    
  But_UP.interval(25);
  But_DOWN.attach(But_Bl, INPUT);    
  But_DOWN.interval(25);

  digitalWrite(Sv, HIGH);

  tft.setRotation(0);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

}

void menu(){

  tft.setFreeFont(&FreeSans9pt7b);
  tft.setCursor(65, 5);
  tft.print("ARCADE_ESP32");

 for(int i = 0; i < 3; i++){
    uint16_t yPos = start_text_y + (i * text_h);
    tft.setCursor(20, yPos);
    tft.print(Menu_str[i]);

    if(i == sel_item){
      tft.drawRect(10, yPos-12, 160, 18, TFT_YELLOW);
    }

  }


}

void loop() {
  But_UP.update();
  But_DOWN.update();

  menu();

  if(But_UP.fell() and SCREEN[now_scr] == "MENU"){
    sel_item--;
    if(sel_item < 0){
      sel_item = 2;
    }
    tft.fillScreen(TFT_BLACK);
  }
  if(But_DOWN.fell() and SCREEN[now_scr] == "MENU"){
    sel_item++;
    if(sel_item > 2){
      sel_item = 0;
    }
    tft.fillScreen(TFT_BLACK);
  }

}

