#include "Gpio.h"
#include "Uart.h"

int main()
{
	UART0_Init(38400);
	Switch_Init();
	RGBLed_Init();
	for(;;) {}
}
