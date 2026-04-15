#include <Bounce2.h>

#include <SPI.h>
#include <TFT_eSPI.h>

uint16_t text_h = 25;
uint16_t start_text_y = 70;
int sel_item = 0;

int now_scr = 0;
const char* SCREEN[] = {"MENU", "STAR_SHOOTER", "OPTIONS", "HIGH_SCORES"};
const char* Menu_str[] = {"Star shooter", "Options", "Your game statistics"};
Bounce But_UP, But_DOWN, But_RIGHT, But_LEFT;

#define JX 35
#define JY 34
#define But_Wh 26
#define But_Bl 25
#define But_Gr 27
#define Sv 4

TFT_eSPI tft = TFT_eSPI();

int x_s = 100;
int y_s = 100;

void setup() {
  Serial.begin(115200);
  tft.init();

  But_UP.attach(But_Wh, INPUT);    
  But_UP.interval(25);
  But_DOWN.attach(But_Bl, INPUT);    
  But_DOWN.interval(25);
  But_RIGHT.attach(But_Gr, INPUT);
  But_RIGHT.interval(25);

  digitalWrite(Sv, HIGH);

  x_s = random(-15, 15);
  y_s = random(-15, 15);

  tft.setRotation(0);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

}
/////////////////////////////////////////////// может сделать более одного кораблика?
void star_shooter_oneship(){



}

void star_start(){

}

int x_a = 120;
int y_a = 140;

void star_aim(int x, int y){
  tft.drawCircle(x, y, 15, TFT_RED);
  tft.drawRect(x-1, y-1, 3, 3, TFT_RED);


}
/* это в битах
const uint8_t im_ship[] {
  B00000000, B11111111, B11111100, B00000000,
  B00000000, B11111111, B11111100, B00000000,
  
  B00001111, B00110000, B00110011, B11000000,
  B00001111, B00110000, B00110011, B11000000,

  B00111100, B00110000, B00110000, B11110000,
  B00111100, B00110000, B00110000, B11110000,

  B00000011, B11110000, B00111111, B00000000,
  B00000011, B11110000, B00111111, B00000000,

  B00000000, B00111111, B11110000, B00000000,
  B00000000, B00111111, B11110000, B00000000,

  B00000000, B00001111, B11000000, B00000000,
  B00000000, B00001111, B11000000, B00000000,

  B00000000, B00000011, B00000000, B00000000,
  B00000000, B00000011, B00000000, B00000000

};
*/ // А это в HEX коде

const uint8_t im_ship[] = {
  0x00, 0xFF, 0xFC, 0x00,
  0x00, 0xFF, 0xFC, 0x00,
  
  0x0F, 0x30, 0x33, 0xC0,
  0x0F, 0x30, 0x33, 0xC0,
  
  0x3C, 0x30, 0x30, 0xF0,
  0x3C, 0x30, 0x30, 0xF0,
  
  0x03, 0xF0, 0x3F, 0x00,
  0x03, 0xF0, 0x3F, 0x00,
  
  0x00, 0x3F, 0xF0, 0x00,
  0x00, 0x3F, 0xF0, 0x00,
  
  0x00, 0x0F, 0xC0, 0x00,
  0x00, 0x0F, 0xC0, 0x00,
  
  0x00, 0x03, 0x00, 0x00,
  0x00, 0x03, 0x00, 0x00
};



void star_ship(){
  tft.drawBitmap(x_s, x_s, im_ship, 32, 14, TFT_GREEN);  //центр x = x+16; y = y-7
}

void star_game(){
  int JVX = map(JX, 0, 1024, -20, 20);
  int JVY = map(JY, 0, 1024, -20, 20);

  star_aim(x_a, x_a);
  x_a = x_a + JVX;
  y_a = y_a + JVY;

}

void star_end(){



}



///////////////////////////////////////////////

void menu(){



  tft.setFreeFont(&FreeSans9pt7b);
  tft.setCursor(50, 15);
  tft.print("ARCADE_ESP32");

 for(int i = 0; i < 3; i++){
    uint16_t yPos = start_text_y + (i * text_h);
    
    tft.setCursor(20, yPos);
    tft.print(Menu_str[i]);

    if(i == sel_item){
      tft.drawRect(10, yPos-15, 185, 20, TFT_YELLOW);
    }

  }


}

void loop() {
  But_UP.update();
  But_DOWN.update();

  star_aim(150, 150);

  tft.drawBitmap(150, 150, im_ship, 32, 14, TFT_GREEN);
  Serial.println("Hi");
  menu();

  if(But_UP.fell() and SCREEN[now_scr] == "MENU"){
    sel_item--;
    if(sel_item <= 0){
      sel_item = 0;

    }
    tft.fillScreen(TFT_BLACK);
  }
  if(But_DOWN.fell() and SCREEN[now_scr] == "MENU"){
    sel_item++;
    if(sel_item > 1){
      sel_item = 2;
    }
    tft.fillScreen(TFT_BLACK);
  }
  if(But_RIGHT.fell() and sel_item == 0){
    tft.fillScreen(TFT_BLACK);
   
    star_game();
    //if(){

    //}
    
  }
  

}

