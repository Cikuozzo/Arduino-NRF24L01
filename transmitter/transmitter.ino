#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";
int valore;  //dichiaro la variabile valore che memorizzerà il valore dato dal convertitore analogico-digitale
int portarossa = 5; 
int portaverde = 6; 
int portablu = 7;

void colore (unsigned char rosso, unsigned char verde, unsigned char blu)
{
 analogWrite(portarossa, rosso); //attiva il led rosso con l’intensita’ definita nella variabile rosso
 analogWrite(portablu, blu); //attiva il led blu con l’intensita’ definita nella variabile blu
 analogWrite(portaverde, verde); //attiva il led verde con l’intensita’ definita nella variabile verde
}

void setup() {

  pinMode(portarossa, OUTPUT); // dichiara la porta 11 come porta di output
  pinMode(portaverde, OUTPUT); // dichiara la porta 10 come porta di output
  pinMode(portablu, OUTPUT); // dichiara la porta 9 come porta di output
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  pinMode(2, OUTPUT);//define arduino pin Trigger
  pinMode(4, INPUT);//define arduino pin Echo
  Serial.begin(9600);//enable serial monitor

}

void loop() {

  valore=analogRead(0); //Leggo il valore analogico alla porta A0 e lo metto dentro "valore"
  if (valore>=750) //se valore maggiore di 573 allora....
   {
   colore(0, 255, 0);
   }
   if ((valore<749)&&(valore >=250)) //se valore maggiore di 450 e valore minore di 573 allora....
   {
   colore(255,215,0); // accende il giallo (255 di rosso e 215 di verde)
   }
   if (valore<250)//se valore minore di 450 allora....
   {
     colore(255, 0, 0);
   }

  int isPassed = 0;
  digitalWrite(2, LOW);
  delayMicroseconds(4);
  digitalWrite(2, HIGH);
  delayMicroseconds(10);
  digitalWrite(2, LOW);
  
  long t = pulseIn(4, HIGH);

  long cm = t / 29 / 2; //time convert distance

  if(cm <= 100){
    
    isPassed = 1;
    radio.write(&isPassed, sizeof(isPassed));
    delay(1000);
     
  }
  
  
}
