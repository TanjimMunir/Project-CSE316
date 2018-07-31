
#define F_CPU 1000000UL

#include<avr/io.h>
#include<avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

volatile unsigned char cdata='z';
volatile int sendstat[50];
volatile int i=-1;


void sendVal(uint8_t c){
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
	if(sendstat[i]==0)
	{
		
		if(cdata == 's')
		{
			PORTA=2;
			_delay_ms(200);
			PORTA=0;
			sendVal(2);
			cdata = 'z';
			sendstat[i]=1;
		}
	}
	_delay_ms(200);
	GIFR = 0xFF;
}

int main(void)
{
	DDRA |= (1 << PINA1);
	int UBBRValue = 12;//AS described before setting baud rate 9600BPS, double transmission
	UBRRH = (unsigned char) (UBBRValue >> 8);
	UBRRL = (unsigned char) UBBRValue;
	UCSRA = 2;
	UCSRB = (1 << RXEN) | (1 << TXEN);    //Enable the receiver and transmitter
	UCSRC = 0b10000110;
	
	MCUCR = MCUCR & 0b11110011;
	MCUCR = MCUCR | 0b00001000;
	GICR = (1<<INT1);
	sei();

	while (1)

	{
		unsigned char rdata = receiveChar();
		if(rdata == 's')
		{
			i++;
			sendstat[i]=0;
			cdata = rdata;
			PORTA = 2;
			_delay_ms(200);
			PORTA = 0;
		}
		
	}

}

