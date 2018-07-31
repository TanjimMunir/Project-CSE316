/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/


#include <SPI.h>
#include <stdlib.h>
#include <string.h>

#include "nRF24L01.h"
#include "RF24.h"

char text[20];
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
unsigned char data;
unsigned char bdata;

void setup() {
  Serial.begin(9600);
  pinMode(0,INPUT);//RXD pin is set for INPUT
  pinMode(1,OUTPUT);
  radio.begin();
  //radio.openWritingPipe(address);
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MIN);
}
void loop() {
    if(Serial.available() > 0)
    {
      //Serial.println("here1");
      data= Serial.read();
      if(data == 's')
      {
        //Serial.println("here2");
        radio.stopListening();
        strcpy(text,"send");
        radio.write(&text, sizeof(text));
        while(1)
        {
          delay(5);
          radio.startListening();

          if (radio.available()) {
            Serial.println("here");
            char tex[32] = "";
            radio.read(&tex, sizeof(tex));
            Serial.println(tex);
             if(strcmp(tex,"slave1 has sent")== 0)
             {
               Serial.write('1');
             }
             
             if(strcmp(tex,"slave2 has sent")== 0)
             {
              Serial.write('2');
             }
          }
        

        if(Serial.available() > 0)
        {
          bdata = Serial.read();
          if(bdata == 'b')
          {
            Serial.println(bdata);
            break;
          }
        }
      }
      Serial.println("Came out");
      }
     
    }
     
    

  
}


