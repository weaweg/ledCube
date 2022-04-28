#include <xc.h>
#include <stdlib.h>
#include <time.h>
#include <avr/interrupt.h>
#include "main.h"
#include "functions.h"
#include <util/delay.h>

int main(void) {
	Init();
    while(1) {
		for(int i = 0; i < EFFECTS_NUMBER; ++i) {
			launchEffect(i);
		}
    }
}

void Init() {
	//ustawienie liczb pseudolosowych
	srand(time(NULL));

	//ustawienia zmiennych
	crntMode = 1;
	crntLayer = 0;
	
	//ustawienia portów
	DATA_DDR = 0xFF;
	DATA_PORT = 0x00;
	
	TRAN_DDR = 0xFF;
	TRAN_PORT = 0x00;
	
	CLK_DDR = 0xFF;
	CLK_PORT = 0x00;
	
	OTH_DDR = 0xFB;
	OTH_PORT= 0x04;
	
	//ustawienie timera
	OCR2 = 10;
	TCCR2 |= (1<<CS20) | (1<<CS22) | (1<<WGM21);
	TIMSK |= (1<<OCIE2);
	
	//ustawienie przerwania przycisku
	MCUCR |= (1<<ISC01);
	GICR |= (1<<INT0);
	
	//w³¹czenie przerwañ
	sei();
}

ISR(TIMER2_COMP_vect) {
	TRAN_PORT = 0x00;
	OTH_PORT |= OC_PIN;
	
	for (int i = 0; i < CB_SIZE; ++i) {
		DATA_PORT = cube[crntLayer][i];
		CLK_PORT = (1<<i);
	}

	TRAN_PORT = (1<<crntLayer);
	OTH_PORT &= ~OC_PIN;
	
	++crntLayer;
	if (crntLayer == 8) crntLayer = 0;
}

ISR(INT0_vect) {
	cli();
	OTH_PORT |= LED_PIN;
	if(crntMode == 0) {
		TCNT2 = 0x00;
		TIMSK |= (1<<OCIE2);
		OTH_PORT &= ~OC_PIN;
		crntMode = 1;
	}
	else {
		TIMSK &= ~(1<<OCIE2);
		TRAN_PORT = 0x00;
		OTH_PORT |= OC_PIN;
		DATA_PORT = 0x00;
		CLK_PORT = 0x00;
		crntMode = 0;
	}
	_delay_ms(2000);
	OTH_PORT &= ~LED_PIN;
	sei();
}