#include "Gpio.h"
#include "Uart.h"
#include "Adc.h"
#include "Pit.h"

static uint8_t flag = 0;

char changeLedSeqDir(void);

char changeLedSeqDir(void)
{
	if(UART0->S1 & UART0_S1_RDRF_MASK) 
	{
		char c;
		c = UART0->D;
		if(flag == 0)
			flag = 1;
		else
			flag = 0;
		ledDirection = flag;
		return c;
	}
	else
		return 'c';
}

int main(void)
{
	float measured_voltage;
	uint8_t parte_zecimala;
	uint8_t parte_fractionara1;
	
	UART0_Init(38400);
  RGBLed_Init();
	ADC0_Init();
  PIT_Init();
	
	for(;;)
	{		
		changeLedSeqDir();		
		if(readyToTransmit == 1)
		{
				measured_voltage = (analog_input * 5.0f) / 65535;
				
				parte_zecimala = (uint8_t) measured_voltage;
				parte_fractionara1 = ((uint8_t)(measured_voltage * 10)) % 10;
		
				ChangeColorFromFlame(parte_zecimala+(parte_fractionara1)/10);
	
				UART0_Transmit('V');
				UART0_Transmit('a');
				UART0_Transmit('l');
				UART0_Transmit('u');
				UART0_Transmit('e');
				UART0_Transmit(' ');
				UART0_Transmit('=');
				UART0_Transmit(' ');
				UART0_Transmit(parte_zecimala + 0x30);
				UART0_Transmit('.');
				UART0_Transmit(parte_fractionara1 + 0x30);
				UART0_Transmit('V');
				UART0_Transmit(0x0A);
				UART0_Transmit(0x0D);
				
				readyToTransmit = 0;
			}
		}
}

