#include "MKL25Z4.h"
void RGBLed_Init(void);
void PORTD_IRQHandler(void);
void ChangeColorFromFlame(float);
void ChangePlacaColor(void);

volatile extern uint8_t ledDirection;