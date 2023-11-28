#include "Gpio.h"
#include "Uart.h"

//#define SWITCH_PIN (12) // PORT A

#define SWITCH_PIN (4) //PORT D

#define RED_LED_PIN (3) // PORT B

#define GREEN_LED_PIN (1) // PORT B


//#define BLUE_LED_PIN (1) // PORT D

#define YELLOW_LED_PIN (12) //port A

uint8_t state;


void Switch_Init(void) {
	
	// Activarea semnalului de ceas pentru a putea folosi GPIO cu ajutorul pinului 1 de pe portul C
//	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
		SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	// Utilizarea GPIO impune selectarea variantei de multiplexare cu valorea 1
	//PORTA->PCR[SWITCH_PIN] &= ~PORT_PCR_MUX_MASK;
	//PORTA->PCR[SWITCH_PIN] |= PORT_PCR_MUX(1);
	
	// Activare �ntreruperi pe rising edge
	//PORTA->PCR[SWITCH_PIN] |= PORT_PCR_IRQC(0x09) | PORT_PCR_PE_MASK;
	
		PORTD->PCR[SWITCH_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[SWITCH_PIN] |= PORT_PCR_MUX(1);
	
	// Activare �ntreruperi pe rising edge
	PORTD->PCR[SWITCH_PIN] |= PORT_PCR_IRQC(0x09) | PORT_PCR_PE_MASK;
	
	
	state = 0;
	
	// Activare �ntrerupere pentru a folosi switch-u
	//NVIC_ClearPendingIRQ(PORTA_IRQn);
	//NVIC_SetPriority(PORTA_IRQn, 128);
	//NVIC_EnableIRQ(PORTA_IRQn);
	
	
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_SetPriority(PORTD_IRQn, 128);
	NVIC_EnableIRQ(PORTD_IRQn);
}
void PORTD_IRQHandler() {
	
	
	// Prin utilizarea variabilei state, putem realiza un FSM
	// si sa configuram fiecare tranzitie in parte prin 
	// stingerea ledului anterior si aprinderea ledului curent
	
	if(state == 0) {
			//turn on red led
		GPIOB_PTOR |= (1<<RED_LED_PIN);
		UART0_Transmit(0x52);
		state = 1;
		

	} else if (state == 1) {
		//turn off red led
		GPIOB_PTOR |= (1<<RED_LED_PIN);

		//turn on green led
		GPIOB_PTOR |= (1<<GREEN_LED_PIN);	
		UART0_Transmit(0x47);
		state = 2;
	}
	
	 else if (state == 2) {
		//turn off green led
		GPIOB_PTOR |= (1<<GREEN_LED_PIN);
		 
		//turn on yellow led
		GPIOA_PTOR |= (1<<YELLOW_LED_PIN);
		UART0_Transmit(0x59);

		state = 3;
	} else if (state == 3) {
				//turn off yellow leds
				GPIOA_PTOR |= (1<<YELLOW_LED_PIN);

		state = 0;
	}
		
	
	
	
	//PORTA_ISFR = (1<<SWITCH_PIN);
	
	PORTD_ISFR = (1<<SWITCH_PIN);

}
void RGBLed_Init(void){
	
	// Activarea semnalului de ceas pentru pinii folositi �n cadrul led-ului RGB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTA_MASK;
	
	// --- RED LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<RED_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOB_PSOR |= (1<<RED_LED_PIN);
	
	
	// --- GREEN LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<GREEN_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOB_PSOR |= (1<<GREEN_LED_PIN);
	
	
	
	PORTA->PCR[YELLOW_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[YELLOW_LED_PIN] |= PORT_PCR_MUX(1);
	
		// Configurare pin pe post de output
	GPIOA_PDDR |= (1<<YELLOW_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOA_PSOR |= (1<<YELLOW_LED_PIN);
}