#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU		14745600
#define BAUD		38400
#define MYUBRR		(((((F_CPU * 10) / (16L * BAUD)) + 5) / 10) - 1)

#define DATA_DDR	DDRA
#define DATA_PORT	PORTA

#define TRAN_DDR	DDRB
#define TRAN_PORT	PORTB

#define CLK_DDR		DDRC
#define CLK_PORT	PORTC

#define OTH_DDR		DDRD
#define OTH_PORT	PORTD
#define SW_PIN		(1 << PORTD2)
#define LED_PIN		(1 << PORTD6)
#define OC_PIN		(1 << PORTD7)

uint8_t crntLayer;
uint8_t crntMode;

void Init();

#endif