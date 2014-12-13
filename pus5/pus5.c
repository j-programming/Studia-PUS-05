/*
 * Klawiatura
 *  Author: Jacek Doniec, Michal Daniel
 */

/* 
	CONFIG:
	PB[0-4]->kolumna C[4-1] (na odwrot)
	PD[0-7]->W1-K4
	PA[0-7]->Cyfra[a-dp]
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "TaskManagerLib.h" //biblioteka Task-managera
#define F_CPU 16000000ul
#include <util/delay.h>


int current=0;
int ButtonValue=0;
void enable_clock();
int NUMBERS[]= {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

//2 cyfry na wyswietlacz - przycisk
void fooo(void * params)
{
	int tmp=0;
	if(current==0){
		PORTB = 0b11111111;
		PORTA = ~NUMBERS[ButtonValue%10];
		PORTB = 0b11111110;
	}
	if(current==1){
		PORTB = 0b11111111;
		tmp = ButtonValue/10;
		PORTA = ~NUMBERS[tmp%10];
		PORTB = 0b11111101;
	}
	current = (current + 1) % 2;
}


void keyListen(void * params)
{
	//podlaczenie do zlacz portD
	//odczyt wierszy - ustawienie
	PORTD = 0;
	DDRD = 0xF0;
	PORTD = 0x0F;
	_delay_us(5);
	

	int row = ~PIND & 0b00001111;
	int row_count = 0;
	
	//odczyt wierszy - odczyt
	for(int i = 0; i < 4; i++)
	{
		if(row & (1 << i))
		{
			row_count = i + 1;
		}
	}

	//odczyt kolumn - ustawienie
	PORTD = 0x00;
	DDRD = 0x0F;
	PORTD = 0xF0;
	_delay_us(5);

	int column = ((~PIND) >> 4) & 0b00001111 ;
	//odczyt kolumn - odczyt
	int column_count = 0;
	for(int i = 0; i < 4; i++)
	{
		if(column & (1 << i))
		{
			column_count = i + 1;
		}
	}
	
	
	//podliczanie
	int TempButtonValue = row_count * 4 + column_count;
	
	if(ButtonValue > 0)
		ButtonValue = TempButtonValue- 4;
	else ButtonValue = TempButtonValue;
}
		


int main() {
	//display
	enable_clock();
	DDRB=0xFF;
	DDRA=0xFF;
	PORTA = ~NUMBERS[8];
	
	//klawiatura
	PORTD = 0x00;
	DDRD = 0x0F;
	PORTD = 0xF0;
	

	//taski
	AddTask(0,4,fooo,0);
	AddTask(1,50,keyListen,0);
	execute();
	return 0;
}


void enable_clock() {
  /*
   * Configure the timer0 to interrupt on OCR0 match and then clear counter
   * 16MHz / 64 = 250kHz  <-- Prescaler
   * 250kHz / 250 = 1kHz  <-- OCR0 matcher
   * So timer0 interrupt will run at 1kHz/1ms interval
   */
  
  TIMSK |= (1 << OCIE0);		//turn on timer 0 cmp match ISR 
  OCR0 = 250;  	        //set the compare reg to 250 time ticks
  TCCR0 |= (1 << WGM01) | (1<<CS00) | (1 << CS01);   // turn on clear-on-match and CLK/64 prescaler
  sei();
}


// This function runs at 1000Hz (1ms)
ISR (TIMER0_COMP_vect) {
	schedule();
}