
#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>

volatile unsigned char cdata='z';
volatile int sendstat[50];
volatile int i=-1;

volatile unsigned char strDouble[20];
volatile float total_time;
volatile unsigned int t,overflowCount;

void sendVal(uint8_t c){
	while (! (UCSRA & (1 << UDRE)) );
	
	UDR = c;//once transmitter is ready sent eight bit data
}

unsigned char receiveChar(){
	while ( (UCSRA & (1 << RXC)) == 0x00 );
	
	return UDR ;
}


ISR(TIMER1_CAPT_vect)
{
	if(sendstat[i]==0)
	{
		if(cdata == 's')
		{
			t = ICR1;
			PORTA=2;
			_delay_ms(200);
			PORTA=0;
			sendVal('2');
			_delay_us(10);
			sendVal('!');
			_delay_us(10);
			total_time = overflowCount * 0.065536 + t * (1e-6);
			dtostrf(total_time,5,6,strDouble);
			for(volatile int k = 0 ; strDouble[k] != '\0' ; k++){
				sendVal(strDouble[k]);
				_delay_us(10);
			}
			sendVal('$');
			_delay_us(10);
			TCNT1 = 0;
			overflowCount = 0;
			cdata = 'z';
			sendstat[i]=1;
		}
	}
}

ISR(TIMER1_OVF_vect){
	overflowCount++;
}

int main(void)

{
	DDRA |= (1 << DDA1);
	int UBBRValue = 12; //AS described before setting baud rate 9600BPS, double transmission
	UBRRH = (unsigned char) (UBBRValue >> 8);
	UBRRL = (unsigned char) UBBRValue;
	UCSRA = 2;
	UCSRB = (1 << RXEN) | (1 << TXEN);    //Enable the receiver and transmitter
	UCSRC = 0b10000110;
	
	DDRD &= ~(1 << DDD6);
	TCCR1A = 0x00;
	TCCR1B = 0x41;
	TIMSK = (1 << TICIE1) | (1 << TOIE1);
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
			overflowCount = 0;
			TCNT1 = 0;
		}
	}

}

