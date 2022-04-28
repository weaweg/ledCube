#include <xc.h>
#include <stdlib.h>
#include "main.h"
#include "functions.h"
#include "font.h"
#include <util/delay.h>

uint8_t isInRange(uint8_t x, uint8_t y, uint8_t z) {
	if (x > CB_SIZE || y > CB_SIZE || z > CB_SIZE)
	return 0;
	return 1;
}

void setPoint(uint8_t x, uint8_t y, uint8_t z) {
	if(isInRange(x, y, z))
	cube[z][y] |= (1<<x);
}

void clrPoint(uint8_t x, uint8_t y, uint8_t z) {
	if(isInRange(x, y, z))
	cube[z][y] &= ~(1<<x);
}

uint8_t getPoint(uint8_t x, uint8_t y, uint8_t z) {
	if (isInRange(x, y, z))
	if (cube[z][y] & (1 << x))
	return 1;
	return 0;
}

void setPlaneX (uint8_t x) {
	if (x < CB_SIZE)
	for (uint8_t z = 0; z < CB_SIZE; ++z)
	for (uint8_t y = 0; y < CB_SIZE; ++y)
	cube[z][y] |= (1 << x);
}

void clrPlaneX (uint8_t x) {
	if (x < CB_SIZE)
	for (uint8_t z = 0; z < CB_SIZE; ++z)
	for (uint8_t y = 0; y < CB_SIZE; ++y)
	cube[z][y] &= ~(1 << x);
}

void setPlaneY (uint8_t y) {
	if (y < CB_SIZE)
	for (uint8_t z = 0; z < CB_SIZE; ++z)
	cube[z][y] = 0xff;
}

void clrPlaneY (uint8_t y) {
	if (y < CB_SIZE)
	for (uint8_t z = 0; z < CB_SIZE; ++z)
	cube[z][y] = 0x00;
}

void setPlaneZ (uint8_t z) {
	if (z < CB_SIZE)
	for (uint8_t i = 0; i < CB_SIZE; ++i)
	cube[z][i] = 0xFF;
}

void clrPlaneZ (uint8_t z) {
	if (z < CB_SIZE)
	for (uint8_t i = 0; i < CB_SIZE; ++i)
	cube[z][i] = 0x00;
}

void setPlane (uint8_t axis, uint8_t i) {
	switch (axis) {
		case AXIS_X:
		setPlaneX(i);
		break;
		case AXIS_Y:
		setPlaneY(i);
		break;
		case AXIS_Z:
		setPlaneZ(i);
		break;
	}
}

void clrPlane (uint8_t axis, uint8_t i) {
	switch (axis) {
		case AXIS_X:
		clrPlaneX(i);
		break;
		case AXIS_Y:
		clrPlaneY(i);
		break;
		case AXIS_Z:
		clrPlaneZ(i);
		break;
	}
}


void fillCube() {
	for (uint8_t z = 0; z < CB_SIZE; ++z) {
		for (uint8_t y = 0; y < CB_SIZE; ++y) {
			cube[z][y] = 0xFF;
		}
	}
}

void clearCube() {
	for (uint8_t z = 0; z < CB_SIZE; ++z) {
		for (uint8_t y = 0; y < CB_SIZE; ++y) {
			cube[z][y] = 0x00;
		}
	}
}

void shift (uint8_t axis, uint8_t dir) {
	uint8_t i, j, k;
	for (i = 0; i < CB_SIZE-1; ++i) {
		if (dir == 0) j = i;
		else j = (7-i);
		
		for (uint8_t x = 0; x < CB_SIZE; ++x) {
			for (uint8_t y = 0; y < CB_SIZE; ++y) {
				if (dir == 0) k = j+1;
				else k = j-1;
				
				if (axis == AXIS_X) {
					if(getPoint(k, y, x))
					setPoint(j, y, x);
					else
					clrPoint(j, y, x);
				}
				
				else if (axis == AXIS_Y) {
					if(getPoint(x, k, y))
					setPoint(x, j, y);
					else
					clrPoint(x, j, y);
				}
				
				else if (axis == AXIS_Z) {
					if(getPoint(x, y, k))
					setPoint(x, y, j);
					else
					clrPoint(x, y, j);
				}
			}
		}
	}
	
	if (dir == 0) i = 7;
	else i = 0;
	clrPlane(axis, i);
}

void boxWireframe(uint8_t a, uint8_t b) {
	for (uint8_t i = a; i <= b; ++i) {
		setPoint(i, a, a);
		setPoint(i, a, b);
		setPoint(i, b, a);
		setPoint(i, b, b);
		setPoint(a, i, a);
		setPoint(a, i, b);
		setPoint(b, i, a);
		setPoint(b, i, b);
		setPoint(a, a, i);
		setPoint(a, b, i);
		setPoint(b, a, i);
		setPoint(b, b, i);
	}
}

#define WRITEDELAY 50
void effectWrite(const char *str) {
	clearCube();
	while (*str) {
		uint8_t chr[5];
		fontGetChar(*str++, chr);
		for (uint8_t y = 0; y < 5; ++y)
		for (uint8_t z = 0; z < 8; ++z)
		if ((chr[y] & (0x80>>z)))
		setPoint(0, y+1, z);
		
		for (uint8_t i = 0; i < 6; ++i) {
			_delay_ms(WRITEDELAY);
			shift(AXIS_X, 1);
		}
	}

	for (uint8_t i = 0; i < 8; ++i) {
		_delay_ms(WRITEDELAY);
		shift(AXIS_X, 1);
	}
}

#define RAINDELAY 70
void effectRain(uint16_t iter) {
	clearCube();
	uint8_t rnd_num;
	for (; iter > 0; --iter) {
		rnd_num = rand()%4 + 2;
		uint8_t rnd_x, rnd_y;
		for (uint8_t j = 0; j < rnd_num; ++j) {
			rnd_x = rand()%8;
			rnd_y = rand()%8;
			setPoint(rnd_x, rnd_y, 7);
		}
		_delay_ms(RAINDELAY);
		shift(AXIS_Z, 0);
	}
	for (uint8_t i = 0; i < CB_SIZE; ++i) {
		_delay_ms(RAINDELAY);
		shift(AXIS_Z, 0);
	}
}

#define PLANEDELAY 40
void effectPlane(uint16_t iter, uint8_t plane) {
	clearCube();
	for(; iter > 0; --iter) {
		for (uint8_t i = 0; i < 7; ++i) {
			clearCube();
			setPlane(plane, i);
			_delay_ms(PLANEDELAY);
		}
		for (uint8_t i = 7; i > 0; --i) {
			clearCube();
			setPlane(plane, i);
			_delay_ms(PLANEDELAY);
		}
	}
}

#define BOXDELAY 60
void effectBox(uint16_t iter) {
	clearCube();
	for(; iter > 0; --iter) {
		uint8_t j;
		for (uint8_t i = 0; i < 4; ++i) {
			j = i;
			clearCube();
			boxWireframe(3-j, 4+j);
			_delay_ms(BOXDELAY);
		}
		for (uint8_t i = 0; i < 4; ++i) {
			j = 3 - i;
			clearCube();
			boxWireframe(3-j, 4+j);
			_delay_ms(BOXDELAY);
		}
	}
}

#define WAVEDELAY 40
void effectWave(uint16_t iter) {
	clearCube();
	for(; iter > 0; --iter) {
		uint8_t i;
		for(i = 0; i < CB_SIZE-1; ++i) {
			cube[i][0] = 0xFF;
			cube[i+1][0] = 0xFF;
			_delay_ms(WAVEDELAY);
			shift(AXIS_Y, 1);
		}
		for(i = CB_SIZE-1; i > 0; --i) {
			cube[i][0] = 0xFF;
			cube[i-1][0] = 0xFF;
			_delay_ms(WAVEDELAY);
			shift(AXIS_Y, 1);
		}
	}
	for (uint8_t i = 0; i < CB_SIZE; ++i) {
		_delay_ms(WAVEDELAY);
		shift(AXIS_Y, 1);
	}
}

#define FILLERDELAY 10
void effectFiller() {
	clearCube();
	uint8_t x, y, z;
	for (uint16_t i = 0; i < 511; ++i) {
		x = rand()%8;
		y = rand()%8;
		z = rand()%8;

		if(!getPoint(x, y, z)) {
			setPoint(x, y, z);
			_delay_ms(FILLERDELAY);
		} else --i;
	}
}

#define LOADBARDELAY 30
void effectLoadbar() {
	clearCube();
	for (uint8_t z = 0; z < 8; ++z)
	for (uint8_t y = 0; y < 8; ++y) {
		cube[z][y] = 0xFF;
		_delay_ms(LOADBARDELAY);
	}
	_delay_ms(105);
	for (uint8_t z = 0; z < 8; ++z)
	for (uint8_t y = 0; y < 8; ++y) {
		cube[z][y] = 0x00;
		_delay_ms(LOADBARDELAY);
	}
}

void launchEffect(uint8_t effect) {
	switch(effect) {
		case 0:
			effectWrite("START");
			break;
		case 1:
			effectLoadbar();
			break;
		case 2:
			effectFiller();
			break;
		case 3:
			effectPlane(3, AXIS_X);
			effectPlane(3, AXIS_Y);
			effectPlane(3, AXIS_Z);
			break;
		case 4:
			effectWave(10);
			break;
		case 5:
			effectBox(10);
			break;
		case 6:
			effectRain(100);
			break;
	}
}