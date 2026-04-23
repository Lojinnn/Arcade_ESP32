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
#define But_Rd 12
#define Sv 4

////////////// очки игр
unsigned int star_score;


//////////////////

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite ship_sprite = TFT_eSprite(&tft);
TFT_eSprite aim_sprite = TFT_eSprite(&tft);
TFT_eSprite aim_sprite_fire = TFT_eSprite(&tft);
//для выстрела
bool aim_fire = false;
unsigned long aim_tau = 0;
uint8_t aim_fire_time = 150;     //в миллисекундах
//
int x_s = random(2, 238-32);
int y_s = random(2, 278-14);
int x_s_old = 0;
int y_s_old = 0;
int xvs = random(-10, 10);
int yvs = random(-10, 10);
unsigned int s_hit = 0;
unsigned long s_tau_hit = 0;
unsigned long s_tau = 0;
uint8_t fps = 29; //в миллисекундах

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




void setup() {
  Serial.begin(115200);
  tft.init();

  But_UP.attach(But_Wh, INPUT);    
  But_UP.interval(25);
  But_DOWN.attach(But_Bl, INPUT);    
  But_DOWN.interval(25);
  But_RIGHT.attach(But_Gr, INPUT);
  But_RIGHT.interval(25);
  But_LEFT.attach(But_Rd, INPUT);
  But_LEFT.interval(25);

  digitalWrite(Sv, HIGH);

  ship_sprite.createSprite(32, 14);
  ship_sprite.setSwapBytes(true);
  ship_sprite.drawBitmap(0, 0, im_ship, 32, 14, TFT_GREEN);

  aim_sprite.createSprite(32, 32);
  aim_sprite.setSwapBytes(true);
  //aim_sprite.fillSprite(TFT_TRANSPARENT);
  aim_sprite.drawCircle(15, 15, 15, TFT_RED);
  aim_sprite.drawCircle(15, 15, 14, TFT_RED);  
  aim_sprite.drawRect(14, 14, 3, 3, TFT_RED);
  
  aim_sprite_fire.createSprite(32, 32);
  aim_sprite_fire.setSwapBytes(true);
  //aim_sprite_fire.fillSprite(TFT_TRANSPARENT);
  aim_sprite_fire.fillCircle(15, 15, 15, TFT_YELLOW);  
  aim_sprite_fire.drawCircle(15, 15, 15, TFT_RED);
  aim_sprite_fire.drawCircle(15, 15, 14, TFT_RED);  
  aim_sprite_fire.drawRect(14, 14, 3, 3, TFT_RED);


  tft.setRotation(0);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
}
/////////////////////////////////////////////// может сделать более одного кораблика?
void star_shooter_oneship(){
  //tft.setFreeFont(&FreeSans9pt7b);
  while(1){
    star_game();
    But_LEFT.update();
    But_UP.update();
 
    if(But_UP.fell()){break;}
  }
}

void star_start(){

}

int x_a = 120;
int y_a = 140;
int x_a_old = 0;
int y_a_old = 0;

void star_aim_update(){
  int JVX = map(analogRead(JX), 4040, 100, -28, 30);
  int JVY = map(analogRead(JY), 4040, 0, -30, 30);

  x_a = x_a + JVX;
  y_a = y_a + JVY;
  if(2 >= x_a){
    x_a = 2;
  }
  if(238 <= x_a+32){
    x_a = 206;
  }
  if(2 >= y_a){
    y_a = 2;
  }
  if(278 <= y_a+32){
    y_a = 246;
  }
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

void star_aim_prerender(){
  tft.fillRect(x_a_old, y_a_old, 32, 32, TFT_BLACK);
  
  x_a_old = x_a;
  y_a_old = y_a;
  
  TFT_eSprite* active_aim = aim_fire ? &aim_sprite_fire : &aim_sprite;
  active_aim->pushSprite(x_a, y_a);
}

void star_render(){
  tft.startWrite();
  tft.fillRect(x_s_old, y_s_old, 32, 14, TFT_BLACK);

  //tft.drawRect(x_a_old+14, y_a_old+14, 3, 3, TFT_BLACK);
  ship_sprite.pushSprite(x_s, y_s);

  star_aim_prerender();

  tft.endWrite();
}

void star_ship_update(){ 
  x_s_old = x_s;
  y_s_old = y_s;
  x_s = x_s + xvs;
  y_s = y_s + yvs;
  
  if(x_s <=2){
    xvs = random(1, 10);
  }
  if(x_s >=238-32){
    xvs = random(-10, -1);
  }
  if(y_s <=2){
    yvs = random(1, 10);
  }
  if(y_s >=278-14){
    yvs = random(-10, -1);
  }
}

void star_game(){
  if(millis() - s_tau < fps) return;
  s_tau = millis();
  star_aim_update();
  star_ship_update();
    
  if(But_LEFT.fell()){
    aim_fire = true;
    aim_tau = millis();
    Serial.println("FIRE");
  }

  if(aim_fire and millis() - aim_tau >= aim_fire_time){
    aim_fire = false;
  }

 // tft.setCursor(30, 30);
 // tft.print(star_score);

  //if((aim_fire and x_s+16 >= x_a and x_s+16 <= x_a+16 and y_s+7 >= y_a and y_s+7 <= y_a+16) or
  //   (aim_fire and x_s+16 >= x_a+16 and x_s+16 <= x_a+32 and y_s+7 >= y_a and y_s+7 <= y_a+16) or
  //   (aim_fire and x_s+16 >= x_a and x_s+16 <= x_a+16 and y_s+7 >= y_a+16 and y_s+7 <= y_a+32) or
 //    (aim_fire and x_s+16 >= x_a+16 and x_s+16 <= x_a+32 and y_s+7 >= y_a+16 and y_s+7 <= y_a+32)){
 //   star_score++;

 //   s_hit = true;
 //   s_tau_hit = millis();
 // }

  //if(s_hit){
 //   tft.fillScreen(TFT_BLACK);
  //}

 // if(s_hit and millis() - s_tau_hit >= 300){
  //  s_hit = false;
  //}
  ////////рендер
  star_render();  
  
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
  But_RIGHT.update();
  But_LEFT.update();

  //star_aim(150, 150);

  //tft.drawBitmap(150, 150, im_ship, 32, 14, TFT_GREEN);

  if(SCREEN[now_scr] == "MENU"){
    menu();
  }
  if(But_UP.fell() and SCREEN[now_scr] == "MENU"){
    sel_item--;
    if(sel_item <= 0){
      sel_item = 0;

    }
    Serial.println(11);
    tft.fillScreen(TFT_BLACK);
  }
  if(But_DOWN.fell() and SCREEN[now_scr] == "MENU"){
    sel_item++;
    if(sel_item > 1){
      sel_item = 2;
    }
    Serial.println(1);
    tft.fillScreen(TFT_BLACK);
  }
  if(But_RIGHT.fell()){ Serial.println(11111111);}
  
  if(But_RIGHT.fell() and sel_item == 0 and SCREEN[now_scr] == "MENU"){
    now_scr = 1;
    tft.fillScreen(TFT_BLACK);
    
    star_shooter_oneship();
    //if(){

    //}
    
  }
  

}

