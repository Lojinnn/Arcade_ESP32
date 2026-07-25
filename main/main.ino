/*
  заметки:
    freeFonts: &FreeSans9pt7b; &FreeSans12pt7b; &FreeSans18pt7b; &FreeSans24pt7b;
    
    снизь нагрузку на проц в меню



*/

#include <Bounce2.h>

//////////////// память
#include <Preferences.h>
Preferences prefs;



///////////////////////////////////

#include <SPI.h>
#include <TFT_eSPI.h>
//////////для меню
uint16_t text_h = 25;
uint16_t start_text_y = 100;
int sel_item = 0;
///////////////////////////////////


////////////////////////////////для статы
uint8_t st_text_h = 30;
uint16_t st_start_text_y = 120;
short st_sel_item = -1;
////////////////////////////////

int now_scr = 0;
const char* SCREEN[] = {"MENU", "STAR_SHOOTER", "STATISTICS", "OPTIONS"};
const char* Menu_str[] = {"Star shooter", "Game statistics", "Options"};
const char* Stat_str[] = {"Star shooter", "Arkanoid", "Ping pong", "Pacman", "Reset"};
Bounce But_UP, But_DOWN, But_RIGHT, But_LEFT = Bounce();

#define JX 35
#define JY 34
#define But_Wh 26
#define But_Bl 25
#define But_Gr 27
#define But_Rd 12
#define Sv 4

////////////// очки игр
unsigned int star_score;
unsigned int star_all_score;
unsigned int star_high_score;

//////////////////

/////////////ачивки
//стар шутер
bool star_first_hit = false; // первое попадание
bool star_hun_hits = false;  // сто очков суммарно
bool star_hun_hgsc = false;  // сто очков за сессию



/////////////


TFT_eSPI tft = TFT_eSPI();
TFT_eSprite ship_sprite = TFT_eSprite(&tft);
TFT_eSprite aim_sprite = TFT_eSprite(&tft);
TFT_eSprite aim_sprite_fire = TFT_eSprite(&tft);
//для выстрела
bool aim_fire = false;
bool star_aim_fire = false;
unsigned long aim_tau = 0;
uint8_t aim_fire_time = 150;     //в миллисекундах
//
const short rand_s[] = {-2, -1, 1, 2}; //трогать только края (кроме хард мода)

short star_menu = 0;
bool flag_hold = false;
int x_s = random(2, 238-32);
int y_s = random(2, 278-14);
int x_s_old = 0;
int y_s_old = 0;
int xvs = random(-10, 10);
int yvs = random(-10, 10);
unsigned int s_hit = 0;
unsigned long s_tau_hit = 0;
unsigned long s_tau = 0;
unsigned long start_timer = 0;
unsigned long star_time = 0;

uint8_t fps = 30; //в миллисекундах

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
  aim_sprite.drawCircle(15, 15, 15, TFT_RED);
  aim_sprite.drawCircle(15, 15, 14, TFT_RED);  
  aim_sprite.drawRect(14, 14, 3, 3, TFT_RED);
  
  aim_sprite_fire.createSprite(32, 32);
  Serial.printf("Sprite created: %d bytes\n", 32*32*2); 
  aim_sprite_fire.setSwapBytes(true);
  aim_sprite_fire.fillCircle(15, 15, 15, TFT_YELLOW);  


  tft.setRotation(0);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  load_data();
  
}

void call(){
  Serial.print  ("X:");
  Serial.print  (analogRead(JX));
  Serial.print  ("Y:");
  Serial.println(analogRead(JY));
}

void load_data(){
  prefs.begin("conf1", false);
  star_high_score = prefs.getUInt("st_hs", 0);
  star_all_score = prefs.getUInt("st_all_sc", 0);
  star_time = prefs.getUInt("st_time", 0);
  star_first_hit = prefs.getBool("st_ach_fh", 0);
  star_hun_hits = prefs.getBool("st_ach_hh", 0);
  star_hun_hgsc = prefs.getBool("st_ach_h_hgsc", 0);

  prefs.end();
  //Serial.println("Load!");  
}

void star_save(unsigned int new_score){
  unsigned long sesion_time = millis() - start_timer;
  star_time += sesion_time;
  
  
  prefs.begin("conf1", false);
  star_all_score = new_score + star_all_score;
  prefs.putUInt("st_all_sc", star_all_score);
  prefs.putUInt("st_time", star_time);
  if(star_high_score < new_score){
    star_high_score = new_score;
    
    prefs.putUInt("st_hs", star_high_score);
    
    //Serial.println("Save!");   ////для отладки
  }
  prefs.end();
}

/////////////////////////////////////////////// может сделать более одного кораблика?

/////////////////////////////////////время
void star_timer_start(){
  start_timer = millis();

}

const char* star_conv_time(unsigned long ms) {
  static char buf[12];
  unsigned long sec = ms / 1000;
  unsigned int h = sec / 3600;
  unsigned int m = (sec % 3600) / 60;
  unsigned int s = sec % 60;
  sprintf(buf, "%02u:%02u:%02u", h, m, s);
  return buf;
}
/////////////////////////////////////////////////////

////////////////////ачивки
void achievement(){


}
////////////////////


void star_shooter_oneship(){
  star_score = 0;
  star_timer_start();
  while(1){
  
    But_UP.update();    
    But_RIGHT.update();

    if(But_RIGHT.read() == HIGH and But_RIGHT.currentDuration() > 1500){
      flag_hold = true;
    }
    if(flag_hold and But_RIGHT.read() == LOW){
      star_menu = star_menu + 1;
      tft.fillScreen(TFT_BLACK);  
      flag_hold = false;
    }
    if(star_menu == 0){
      star_start();
    }  

    if(star_menu == 1){   
      tft.setFreeFont(&FreeSans9pt7b);
      star_game();
      if(But_LEFT.fell()){
        aim_fire = true;
        aim_tau = millis();
        Serial.println("FIRE");
      }
      if(aim_fire and millis() - aim_tau >= aim_fire_time){
        aim_fire = false;
      }
    }   

    if(star_menu == 2){
      star_end();
    }
    if(star_menu >=3){star_menu = 0;}

    if(But_UP.fell()){now_scr = 0; tft.fillScreen(TFT_BLACK); star_menu = 0; break;}
  }
  star_save(star_score);
}

void star_start(){
  tft.setFreeFont(&FreeSans24pt7b);
  tft.setCursor(20, 120);
  tft.print("!START!");
  tft.setFreeFont(&FreeSans9pt7b);
  tft.setCursor(20, 170);
  tft.print("Star shooter 2026");
  call();             //опрос джостика для диагностики
}

int x_a = 120;
int y_a = 140;
int x_a_old = 0;
int y_a_old = 0;

void star_aim_update(){
  int JVX = map(analogRead(JX), 4095, 0, -30, 30);
  int JVY = map(analogRead(JY), 4095, 0, -30, 30);

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

  tft.fillRect(x_a_old-1, y_a_old-1, 3, 3, TFT_BLACK);

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
  
  if(s_hit){
    x_s = random(2, 238-32);
    y_s = random(2, 278-14);
    xvs = random(rand_s[0], rand_s[3]);
    yvs = random(rand_s[0], rand_s[3]);
  }

  if(x_s <=2){
    xvs = random(rand_s[2], rand_s[3]);     ///////1, 10
  }
  if(x_s >=238-32){
    xvs = random(rand_s[0], rand_s[1]);
  }
  if(y_s <=2){
    yvs = random(rand_s[2], rand_s[3]);
  }
  if(y_s >=278-14){
    yvs = random(rand_s[0], rand_s[1]);
  }
}

void star_game(){
  if(millis() - s_tau < fps) return;
  Serial.println(star_time);

  But_LEFT.update();
  s_tau = millis();
  star_aim_update();
  star_ship_update();
    
  tft.setCursor(30, 30);
  tft.print(star_score);

  if((But_LEFT.fell() and x_s+16 >= x_a and x_s+16 <= x_a+16 and y_s+7 >= y_a and y_s+7 <= y_a+16) or
     (But_LEFT.fell() and x_s+16 >= x_a+16 and x_s+16 <= x_a+32 and y_s+7 >= y_a and y_s+7 <= y_a+16) or
     (But_LEFT.fell() and x_s+16 >= x_a and x_s+16 <= x_a+16 and y_s+7 >= y_a+16 and y_s+7 <= y_a+32) or
     (But_LEFT.fell() and x_s+16 >= x_a+16 and x_s+16 <= x_a+32 and y_s+7 >= y_a+16 and y_s+7 <= y_a+32)) {  
    star_score++;
    s_hit = true;
    s_tau_hit = millis();
  }

  if(s_hit){
    tft.fillScreen(TFT_BLACK);
  }

  if(s_hit and millis() - s_tau_hit >= 300){
    s_hit = false;
  }
  
  ////////рендер
  star_render();  
  ////////
}

void star_end(){
  tft.setFreeFont(&FreeSans24pt7b);
  tft.setCursor(50, 80);
  tft.print("!END!");
  
  tft.setFreeFont(&FreeSans9pt7b);
  tft.setCursor(20, 160);
  tft.print("Your score = " + String(star_score));
}



///////////////////////////////////////////////

void menu(){
  tft.setFreeFont(&FreeSans12pt7b);
  tft.setCursor(20, 30);
  tft.print("ARCADE_ESP32");
  tft.setFreeFont(&FreeSans9pt7b);
 for(int i = 0; i < 3; i++){
    uint16_t yPos = start_text_y + (i * text_h);
    
    tft.setCursor(20, yPos);
    tft.print(Menu_str[i]);


    if(i == sel_item){
      tft.drawRect(10, yPos-15, 185, 20, TFT_YELLOW);
    }

  }


}

void statistics(){
  tft.setFreeFont(&FreeSans18pt7b);
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(2, 55);
  tft.print("Your Statistics");

  tft.setFreeFont(&FreeSans12pt7b);
  for(int i = 0; i < 5; i++){
    uint16_t yPos = st_start_text_y + (i * st_text_h);

    if(i == 4){
      tft.setTextColor(TFT_RED);
    }
    else{
      tft.setTextColor(TFT_WHITE);
    }
    
    tft.setCursor(20, yPos);
    tft.print(Stat_str[i]);

    if(i == st_sel_item){
      tft.drawRect(10, yPos-20, 185, 25, TFT_YELLOW);
    }

  }
}

void loop() {
  But_UP.update();
  But_DOWN.update();
  But_RIGHT.update();
  But_LEFT.update();

//////////////////////экраны
  if(SCREEN[now_scr] == "MENU"){
    menu();
  }

  if(SCREEN[now_scr] == "STAR_SHOOTER"){
    star_shooter_oneship();
  }

  if(SCREEN[now_scr] == "STATISTICS"){
    statistics();
  }
//////////////////////

/////////////////////для меню  
  if(SCREEN[now_scr] == "MENU"){
    if(But_UP.fell()){
      sel_item--;
      if(sel_item <= 0){
        sel_item = 0;

      }
      tft.fillScreen(TFT_BLACK);
    }
    if(But_DOWN.fell()){
      sel_item++;
      if(sel_item > 1){
        sel_item = 2;
      }
      tft.fillScreen(TFT_BLACK);
    }
    ///////////////статистика
    if(But_RIGHT.fell() and sel_item == 1){
      now_scr = 2;
      tft.fillScreen(TFT_BLACK);
    }    
    ///////////////

    //////////стар шутер
    if(But_RIGHT.fell() and sel_item == 0){
      now_scr = 1;
      tft.fillScreen(TFT_BLACK);
    }
    //////////

  }
///////////////////

////////////////////////////для статы
  if(SCREEN[now_scr] == "STATISTICS"){
    if(But_UP.fell()){
      st_sel_item--;
      if(st_sel_item <= 0){
        st_sel_item = 0; 
      }
      tft.fillScreen(TFT_BLACK);
    }

    if(But_DOWN.fell()){
      st_sel_item++;
      if(st_sel_item >= 4){
        st_sel_item = 4; 
      }
      tft.fillScreen(TFT_BLACK);
    }

    if(But_RIGHT.fell() and st_sel_item == 4){
      now_scr = -1;
      tft.fillScreen(TFT_BLACK);
      tft.setFreeFont(&FreeSans12pt7b);
      tft.setTextColor(TFT_RED);
      tft.setCursor(20, 100);
      tft.print("If you want to reset");
      tft.setCursor(20, 125);
      tft.print("click red button");
      while(1){
        But_UP.update();
        But_DOWN.update();
        But_LEFT.update();
        But_RIGHT.update();
        
        if(But_LEFT.fell()){
          prefs.begin("conf1", false);
          /////////подтираем только данные игр
          prefs.remove("st_hs");
          prefs.remove("st_all_sc");

          prefs.end();
          now_scr = 0;
          st_sel_item = -1;
          tft.fillScreen(TFT_BLACK);
          break;
        }
        if(But_UP.fell() or But_DOWN.fell() or But_RIGHT.fell()){
          now_scr = 2; 
          st_sel_item = -1; 
          tft.fillScreen(TFT_BLACK);
          break;
        }
      }
    }

    if(But_RIGHT.fell() and st_sel_item == 0){
      now_scr = -1;
      load_data();
      tft.fillScreen(TFT_BLACK);
      while(1){
        But_UP.update();
        
        tft.setFreeFont(&FreeSans18pt7b);
        tft.setTextColor(TFT_WHITE);       
        tft.setCursor(20, 40);
        tft.print("Star shooter");
        
        tft.setFreeFont(&FreeSans9pt7b);
        tft.setCursor(10, 70);
        tft.print("Record score ");
        tft.setCursor(140, 70);
        tft.print(star_high_score);

        tft.setCursor(10, 90);
        tft.print("All score ");
        tft.setCursor(140, 90);
        tft.print(star_all_score);

        tft.setCursor(10, 110);
        tft.print("time played ");
        tft.setCursor(140, 110);
        tft.print(star_conv_time(star_time));


        if(But_UP.fell()){tft.fillScreen(TFT_BLACK); st_sel_item = -1; now_scr = 2; break;}
      }
    }

    if(But_LEFT.fell()){
      tft.setTextColor(TFT_WHITE);
      tft.fillScreen(TFT_BLACK);
      now_scr = 0;
    }


  }

////////////////////////////


}