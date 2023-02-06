#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "LedControl.h"


#define pinCLK 4
#define pinCS 3
#define pinDIN 2

#define ELEMENTS 4
LedControl lc = LedControl(pinDIN, pinCLK, pinCS, ELEMENTS);  

#define MAXBARX (ELEMENTS*8)

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

int pista = 0;
int isPassed = 0;
double tempoI = 0;
double cronos = 0;
double tempoF = 0;
int state = 0;
int i = 0;
int orario[] = {1, 3, 0, 0};
int id = 0;

byte alpha[31][5] = {
  {
    B00011100,
    B00100010,
    B00100010,
    B00100010,
    B00011100
  },
  {
    B00001100,
    B00000100,
    B00000100,
    B00000100,
    B00000100
  },{
    B00011100,
    B00000010,
    B00011100,
    B00100000,
    B00111110
  },{
    B00011100,
    B00000010,
    B00001100,
    B00000010,
    B00011100
  },{
    B00010000,
    B00010000,
    B00010100,
    B00011110,
    B00000100
  },{
    B00111110,
    B00100000,
    B00011100,
    B00000010,
    B00111100
  },{
    B00011100,
    B00100000,
    B00111100,
    B00100010,
    B00011100
  },{
    B00111110,
    B00000010,
    B00000100,
    B00000100,
    B00001000
  },{
    B00011100,
    B00100010,
    B00011100,
    B00100010,
    B00011100
  },{
    B00011100,
    B00100010,
    B00011110,
    B00000010,
    B00011100
  },{  
    B00111000,
    B01000101,
    B01000100,
    B01000101,
    B00111000  
  },{
    B00011000,
    B00001001,
    B00001000,
    B00001001,
    B00001000
  },{
    B00111000,
    B00000101,
    B00111000,
    B01000001,
    B01111100
  },{
    B00111000,
    B00000101,
    B00011000,
    B00000101,
    B00111000
  },{
    B00100000,
    B00100001,
    B00101000,
    B00111101,
    B00001000
  },{
    B01111100,
    B01000001,
    B00111000,
    B00000101,
    B01111100
  },{
    B00111000,
    B01000001,
    B01111000,
    B01000101,
    B00111000
  },{
    B01111100,
    B00000101,
    B00001000,
    B00001001,
    B00010000
  },{
    B00111000,
    B01000101,
    B00111000,
    B01000101,
    B00111000 
  },{
    B00111000,
    B01000101,
    B00111000,
    B00000101,
    B00111000  
  },{
    B00011100,
    B10100010,
    B00100010,
    B10100010,
    B00011100
  },{
    B00001100,
    B10000100,
    B00000100,
    B10000100,
    B00000100
  },{
    B00011100,
    B10000010,
    B00011100,
    B10100000,
    B00111110
  },{
    B00011100,
    B10000010,
    B00001100,
    B10000010,
    B00011100
  },{
    B00010000,
    B10010000,
    B00010100,
    B10011110,
    B00000100
  },{
    B00111110,
    B10100000,
    B00011100,
    B10000010,
    B00111100
  },{
    B00011100,
    B10100000,
    B00111100,
    B10100010,
    B00011100
  },{
    B00111110,
    B10000010,
    B00000100,
    B10000100,
    B00001000
  },{
    B00011100,
    B10100010,
    B00011100,
    B10100010,
    B00011100
  },{
    B00011100,
    B10100010,
    B00011110,
    B10000010,
    B00011100
  },{
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  }
};

void setup() {

  for (int i = 0; i < ELEMENTS ; i++) {
    lc.shutdown(i,false);  // Wake up displays  
    lc.setIntensity(i,1);  // 0-15
    lc.clearDisplay(i);  // Clear Displays

  }
  while(!Serial);
  Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();  
}

void loop() {

  if (radio.available()){
    
    radio.read(&isPassed, sizeof(isPassed));
    Serial.println(isPassed);
    Serial.println(state);
    
  }

  if(state == 0){
    
      tempoI = millis();
      Serial.println(isPassed);
    Serial.println(state);
   
  }

  if(isPassed == 1){

      state = 1;
      isPassed = 0; 
    
  }

  if(state == 1){
    
      cronos = millis() - tempoI ;
      Serial.println(cronos / 1000);

    if(isPassed == 2){
    
      state = 0;

  }

}

    for (int i = 0; i < sizeof(orario); i++) {
    int c = orario[i];

    if(i == 0 || i == 3){
      
      id = c; 

    }
    if(i == 1){
      
      id = c + 10;

    }
    if(i == 2){

      id = c + 20;

    }

    int xlettera = i*8;  
    drawSprite(xlettera,1, alpha[id]);

  }
  
  delay(500);
  
}

void pixel(int x, int y, bool stato){  
  int el = (int)(x / 8);
  int xloc = 7 -(x - (8*el));
  y = 7 - y;
  lc.setLed(el,y,xloc, stato);
}

void drawSprite(int xs, int ys, byte data[]){  
  for (int r = 0; r < 5; r++) {
    for (int i = 0; i < 8; i++) {
      bool pix = data[r] & (B10000000 >> i);
      if ((xs+i) <= MAXBARX) { 
        pixel(xs+i, ys+r, pix);
      }
    }
  }
}                                                         
