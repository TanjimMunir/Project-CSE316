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

char text[30];
char receiveText[30];
uint8_t indx = 0;
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
unsigned char data;
unsigned char bdata;
uint8_t av=1;

void setup() {
  Serial.begin(4800);
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
    }
    
      if(data == 's')
      {
        Serial.println("here2");
        radio.stopListening();
        strcpy(text,"send");
        radio.write(&text, sizeof(text));
        while(1)
        {
          
          radio.startListening();

          if (radio.available(&av)) {
            char tex[32] = "";
            radio.read(&tex, sizeof(tex));
            int n = sizeof(tex);
            for(int l=0;l < n;l++)
            { 
                if(tex[l]=='h')
                {
                  break;
                }
                Serial.write(tex[l]);
                delay(700);
                if(tex[l]=='$'){break;}
            }
            tex[0]='\0';
          }
        

        if(Serial.available() > 0)
        {
          bdata = Serial.read();
          
              int asciiVal = (bdata)%125;
              char asciiChar = asciiVal;
              Serial.print(asciiChar);
          
          if(bdata == 'b')
          {
            data = 'z';
            break;
          }
        }
      }
        Serial.println("Came out");
      }
     
    
     
    

  
}


