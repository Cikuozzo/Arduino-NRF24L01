#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

int isPassed = 0;
double tempoI = 0;
double cronos = 0;
double tempoF = 0;
int state = 0;

void setup() {
  
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
    
  }

  if(state == 0){
    
      tempoI = millis();
   
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

}
