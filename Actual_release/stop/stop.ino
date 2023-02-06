#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

int isPassed = 1;

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  pinMode(2, OUTPUT);//define arduino pin
  pinMode(4, INPUT);//define arduino pin
  Serial.begin(9600);//enable serial monitor

}

void loop() {

  digitalWrite(2, LOW);
  delayMicroseconds(4);
  digitalWrite(2, HIGH);
  delayMicroseconds(10);
  digitalWrite(2, LOW);
  
  long t = pulseIn(4, HIGH);

  long cm = t / 29 / 2; //time convert distance

  if(cm <= 100){
    
    isPassed = 2;
    radio.write(&isPassed, sizeof(isPassed));
    delay(1000);
     
  }
  
}
