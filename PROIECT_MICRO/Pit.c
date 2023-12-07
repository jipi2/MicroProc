#include "Pit.h"
#include "Uart.h"
#include "Adc.h"
#include "Gpio.h"

#define LED_PIN (12) // PORT A
uint32_t timer_value;
uint8_t led_state;

void PIT_Init(void) {
	
	// Activarea semnalului de ceas pentru perifericul PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	// Utilizarea semnalului de ceas pentru tabloul de timere
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	// Oprirea decrementarii valorilor numaratoarelor in modul debug
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	// Setarea valoarea numaratorului de pe canalul 0 la o perioada de 1 secunda
	PIT->CHANNEL[0].LDVAL = 0x3F3332;
	
  // Activarea întreruperilor pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	// Activarea timerului de pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;

	// Activarea întreruperii mascabile si setarea prioritatiis
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}
uint8_t UTILS_PrintFlameValue(){
	
	uint16_t analog_input = (uint16_t) ADC0->R[0];

	float measured_voltage = (analog_input * 6.0f) / 65535;
	
	uint8_t parte_zecimala = (uint8_t) measured_voltage;
	uint8_t parte_fractionara1 = ((uint8_t)(measured_voltage * 10)) % 10;
	
	//turn on lights
	ChangeColorFromFlame(parte_zecimala+(parte_fractionara1)/10);
	
	uint8_t parte_fractionara2 = ((uint8_t)(measured_voltage * 100)) % 10;
	uint8_t parte_fractionara3 = ((uint8_t)(measured_voltage * 1000)) % 10;
	UART0_Transmit('V');
	UART0_Transmit('o');
	UART0_Transmit('l');
	UART0_Transmit('t');
	UART0_Transmit('a');
	UART0_Transmit('g');
	UART0_Transmit('e');
	UART0_Transmit(' ');
	UART0_Transmit('=');
	UART0_Transmit(' ');
	UART0_Transmit(parte_zecimala + 0x30);
	UART0_Transmit('.');
	UART0_Transmit(parte_fractionara1 + 0x30);
	UART0_Transmit(parte_fractionara2 + 0x30);
	UART0_Transmit(parte_fractionara3 + 0x30);
	UART0_Transmit('V');
	UART0_Transmit(0x0A);
	UART0_Transmit(0x0D);
	
}
void PIT_IRQHandler(void) {
	
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) 
	{
		UTILS_PrintFlameValue();
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		if(led_state) {
			GPIOA->PCOR |= (1<<LED_PIN);
			led_state = 0;
		}
	}
}