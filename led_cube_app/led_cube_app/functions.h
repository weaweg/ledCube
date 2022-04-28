#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define AXIS_X	0
#define AXIS_Y	1
#define AXIS_Z	2

#define CB_SIZE		8
#define CUBE_BYTES	64

#define EFFECTS_NUMBER	7

uint8_t cube[CB_SIZE][CB_SIZE];

void fillCube();
void clearCube();
void effectWrite(const char *str);
void effectRain(uint16_t iter);
void effectPlane(uint16_t iter, uint8_t plane);
void effectBox(uint16_t iter);
void effectWave(uint16_t iter);
void effectFiller();
void effectLoadbar();
void launchEffect(uint8_t effect);

#endif