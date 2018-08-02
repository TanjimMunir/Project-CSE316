#define F_CPU 1000000UL

#include<avr/io.h>
#include<avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

volatile int breakloop=0;

char text[30];
uint8_t strIndex = 0;

uint8_t buzzerIndex = 0;

struct buzzer{
	int id;
	float timeStamp;
};

struct buzzer buzzers[6];

int comparator(const void *p, const void *q)
{
	float l = ((struct buzzer *)p)->timeStamp;
	float r = ((struct buzzer *)q)->timeStamp;
	return l - r;
}

void sendChar(char c){
	while (! (UCSRA & (1 << UDRE)) );
	
	UDR = c;//once transmitter is ready sent eight bit data
}

char receiveChar(){
	while ( (UCSRA & (1 << RXC)) == 0x00 );
	
	return UDR ;
}

ISR(INT1_vect)
{
	PORTA = 7;
	_delay_ms(100);
	PORTA = 0;
	_delay_ms(100);
	sendChar('b');
	breakloop = 1;
	GIFR = 0xFF;
}

int main(void)

{
	//DDRB = 0xFF;
	DDRA &= ~(1 << PINA0);//pin0 of portD as INPUT
	DDRA |= (1 << PINA1) | (1 << PINA2) | (1 << PINA3) | (1 << PINA7);
	int UBBRValue = 12;//AS described before setting baud rate 9600BPS, double transmission
	UBRRH = (unsigned char) (UBBRValue >> 8);
	UBRRL = (unsigned char) UBBRValue;
	UCSRA = 0;
	UCSRB = (1 << RXEN) | (1 << TXEN);    //Enable the receiver and transmitter
	UCSRC = 0b10000110;
	char k;
	int buttonPressed = 0;
	
	MCUCR = MCUCR & 0b11110011;
	MCUCR = MCUCR | 0b00001000;
	GICR = (1<<INT1);
	sei();

	while (1)

	{
		//sendChar('s');
		k='0';
		if(PINA & (1 << DDA0))
		{
			buttonPressed = 1;
			
		}
		
		if(buttonPressed == 1)
		{
			PORTA=2;
			_delay_ms(200);
			PORTA=0;
			sendChar('s');
			
			_delay_ms(10);
			
			while(1)
			{
				k=receiveChar();
				
				_delay_ms(700);
				
				if(k < 'a' || k > 'z') { text[strIndex] = k;} 
				
				if(text[strIndex] == '$'){
					PORTA=4;
					_delay_ms(200);
					PORTA=0;
					text[strIndex + 1] = '\0';
					
					
					buzzers[buzzerIndex].id = text[0]-48;
					char ff[20];
					uint8_t i = 2;
					for( ;  text[i] != '$' ; i++){
						ff[i - 2] = text[i];
					}
					ff[i - 2] = '\0';
					float tt = atof(ff);
					buzzers[buzzerIndex].timeStamp = tt;
					buzzerIndex++;
					
					
					for(int q = 0 ; ff[q] != '\0' ; q++){
						sendChar(ff[q]);
						_delay_us(10);
					}
					
					
					
					
					strIndex = 0;
					text[0] = '\0';
					
				} else strIndex++;
				
				/*
				if(k == '1')
				{
					PORTA=4;
					_delay_ms(200);
					PORTA=0;
				}
				
				if(k == '2')
				{
					PORTA=8;
					_delay_ms(100);
					PORTA=0;
					_delay_ms(100);
				}
				*/
				
				
				
				if(breakloop == 1)
				{
					breakloop = 0;
					break;
				}

			}
			buttonPressed = 0;
		}
		
		int size = sizeof(buzzers) / sizeof(buzzers[0]);
		qsort((void*)buzzers, size, sizeof(buzzers[0]), comparator);
		
		if(buzzerIndex > 0)
		{
			int lc=0;
			while(lc < buzzerIndex)
			{
				PORTA = 4;
				_delay_ms(900);
				if(buzzers[lc].id==1)
				{
					PORTA = 2;
					_delay_ms(900);
				}
				else if(buzzers[lc].id==2)
				{
					PORTA = 4;
					_delay_ms(900);
					sendChar('2');
				}
				PORTA = 0x00;
				_delay_ms(900);
				lc++;
			}
			
			buzzerIndex = 0;
		}
		
		
		PORTA = 0b10000000;
		_delay_ms(100);
		PORTA = 0;
		

	}

}