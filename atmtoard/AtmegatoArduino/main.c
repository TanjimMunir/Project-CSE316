
#define F_CPU 1000000UL

#include<avr/io.h>
#include<avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

volatile int breakloop=0;



void sendChar(unsigned char c){
	while (! (UCSRA & (1 << UDRE)) );
	
	UDR = c;//once transmitter is ready sent eight bit data

	_delay_ms(20);
}

unsigned char receiveChar(){
	while ( (UCSRA & (1 << RXC)) == 0x00 );
	
	return UDR ;
	_delay_ms(20);
}

ISR(INT1_vect)
{
	PORTA = 7;
	_delay_ms(100);
	PORTA = 0;
	sendChar('b');
	breakloop = 1;
	_delay_ms(200);
	GIFR = 0xFF;
}

int main(void)

{
	DDRA &= ~(1 << PINA0);//pin0 of portD as INPUT
	DDRA |= (1 << PINA1) | (1 << PINA2) | (1 << PINA3) | (1 << PINA7);
	int UBBRValue = 12;//AS described before setting baud rate 9600BPS, double transmission
	UBRRH = (unsigned char) (UBBRValue >> 8);
	UBRRL = (unsigned char) UBBRValue;
	UCSRA = 2;
	UCSRB = (1 << RXEN) | (1 << TXEN);    //Enable the receiver and transmitter
	UCSRC = 0b10000110;
	unsigned char k;
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
				UDR =0;
				if(k == '1')
				{
					PORTA=4;
					_delay_ms(200);
					PORTA=0;
				}
				
				k=receiveChar();
				UDR =0;
				if(k == '2')
				{
					PORTA=8;
					_delay_ms(200);
					PORTA=0;
				}
				
				if(breakloop == 1)
				{
					breakloop = 0;
					break;
				}
				
				
			}
			buttonPressed = 0;
		}
		
		PORTA = 0b10000000;
		_delay_ms(100);
		PORTA = 0;
		

	}

}

