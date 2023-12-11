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
	
  // Activarea Óntreruperilor pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	// Activarea timerului de pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;

	// Activarea Óntreruperii mascabile si setarea prioritatiis
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}

void PIT_IRQHandler(void) {
	
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) 
	{
	  //UTILS_PrintFlameValue();
		ChangePlacaColor();
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		if(led_state) {
			GPIOA->PCOR |= (1<<LED_PIN);
			led_state = 0;
		}
	}
}