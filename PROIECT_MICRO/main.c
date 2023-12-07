#include "Gpio.h"
#include "Uart.h"
#include "Adc.h"
#include "Pit.h"

int main()
{
	UART0_Init(38400);
 //	Switch_Init();
	RGBLed_Init();
//	for(;;) {}
	
	
  ADC0_Init();
	PIT_Init();
	
	for(;;){}


}
