/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


char text[30];
uint8_t data;
uint8_t strIndex = 0;
RF24 radio(7, 8); // CE, CSN
//const byte address[6] = "00001";
const byte addresses[][6] = {"00001", "00002"};
void setup() {
  Serial.begin(9600);
  pinMode(0,INPUT);//RXD pin is set for INPUT
  pinMode(1,OUTPUT);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  //radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MIN);
  //radio.startListening();
}
void loop() {
  
     radio.startListening();
     if (radio.available()) {
         char tex[32] = "";
         radio.read(&tex, sizeof(tex));
         Serial.println(tex);
         if(strcmp(tex,"send")==0)
         {
          Serial.write('s');
         }
       }
         //radio.stopListening();
         if(Serial.available() > 0)
         {
            data = Serial.read();
            //Serial.println((char)data);
            radio.stopListening();
            //radio.flush_tx();
            text[strIndex] = data;
            if(text[strIndex] == '$') {
              text[strIndex + 1] = '\0';
              radio.write(&text, sizeof(text));
              strIndex =0;
            }
            else strIndex++;
         }
}
