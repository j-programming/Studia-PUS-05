#include <avr/io.h>
#include <avr/interrupt.h>
#include "TaskManagerLib.h"

int current=0, i=0;
void enable_clock();
int NUMBERS[]= {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
void foo(void * params){
	PORTA = ~NUMBERS[i%10];
	i++;
	return;
}
void fooo(void * params){
	
	if(current==0){
		PORTB = 0b11111110;
		
	}
	if(current==1){ 
		PORTB = 0b11111101;
		
	}
	if(current==2){
		PORTB = 0b11111011;
		
	}
	if(current==3){
		PORTB = 0b11110111;
		
	}
	current = (current + 1) % 4;
}

int main() {
	enable_clock();
	DDRB=0xFF;
	DDRA=0xFF;
	
	PORTA = ~NUMBERS[8];
	AddTask(0,1,fooo,0);
	AddTask(0,1000,foo,0);
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