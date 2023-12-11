#include "Gpio.h"
#include "Uart.h"

#define SWITCH_PIN (4) //PORT D

#define RED_LED_PIN (3) // PORT B

#define GREEN_LED_PIN (1) // PORT B

#define YELLOW_LED_PIN (12) //port A

/*LED-urile de pe placuta*/

#define PLACA_RED_LED_PIN (18) //PORT B
#define PLACA_GREEN_LED_PIN (19) //PORT B
#define PLACA_BLUE_LED_PIN (1) //PORT D

uint8_t state;
uint8_t flag_green = 0;
uint8_t flag_yellow = 0;
uint8_t flag_red = 0;



void Switch_Init(void) {
	
	// Activarea semnalului de ceas pentru a putea folosi GPIO cu ajutorul pinului 1 de pe portul C
//	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
		SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	// Utilizarea GPIO impune selectarea variantei de multiplexare cu valorea 1
	//PORTA->PCR[SWITCH_PIN] &= ~PORT_PCR_MUX_MASK;
	//PORTA->PCR[SWITCH_PIN] |= PORT_PCR_MUX(1);
	
	// Activare întreruperi pe rising edge
	//PORTA->PCR[SWITCH_PIN] |= PORT_PCR_IRQC(0x09) | PORT_PCR_PE_MASK;
	
		PORTD->PCR[SWITCH_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[SWITCH_PIN] |= PORT_PCR_MUX(1);
	
	// Activare întreruperi pe rising edge
	PORTD->PCR[SWITCH_PIN] |= PORT_PCR_IRQC(0x09) | PORT_PCR_PE_MASK;
	
	
	state = 0;
	
	// Activare întrerupere pentru a folosi switch-u
	//NVIC_ClearPendingIRQ(PORTA_IRQn);
	//NVIC_SetPriority(PORTA_IRQn, 128);
	//NVIC_EnableIRQ(PORTA_IRQn);
	
	
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_SetPriority(PORTD_IRQn, 128);
	NVIC_EnableIRQ(PORTD_IRQn);
}
//void PORTD_IRQHandler() {
//	
//	// Prin utilizarea variabilei state, putem realiza un FSM
//	// si sa configuram fiecare tranzitie in parte prin 
//	// stingerea ledului anterior si aprinderea ledului curent
//	
//	if(state == 0) {
//			//turn on red led
//		GPIOB_PTOR |= (1<<RED_LED_PIN);
//		UART0_Transmit(0x52);
//		state = 1;
//		

//	} else if (state == 1) {
//		//turn off red led
//		GPIOB_PTOR |= (1<<RED_LED_PIN);

//		//turn on green led
//		GPIOB_PTOR |= (1<<GREEN_LED_PIN);	
//		UART0_Transmit(0x47);
//		state = 2;
//	}
//	
//	 else if (state == 2) {
//		//turn off green led
//		GPIOB_PTOR |= (1<<GREEN_LED_PIN);
//		 
//		//turn on yellow led
//		GPIOA_PTOR |= (1<<YELLOW_LED_PIN);
//		UART0_Transmit(0x59);

//		state = 3;
//	} else if (state == 3) {
//				//turn off yellow leds
//				GPIOA_PTOR |= (1<<YELLOW_LED_PIN);

//		state = 0;
//	}
//	
//	//PORTA_ISFR = (1<<SWITCH_PIN);
//	
//	PORTD_ISFR = (1<<SWITCH_PIN);

//}

void ChangePlacaColor()
{
	if(state == 0)
	{
		//turn on all leds
		GPIOB_PTOR |= (1<<PLACA_GREEN_LED_PIN);
		GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
		GPIOD_PTOR |= (1<<PLACA_BLUE_LED_PIN);
		
		state = 1;
	}
	else if(state == 1)
	{
		//turn off red and blue
		GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
		GPIOD_PTOR |= (1<<PLACA_BLUE_LED_PIN);
		
		state = 2;
	}
	else if (state == 2)
	{
		//turn on red
		GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
		
		state = 3;
	}
	else
	{
		//turn off red and green
		GPIOB_PTOR |= (1<<PLACA_GREEN_LED_PIN);
		GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
		
		state = 0;
	}
}

void ChangeColorFromFlame(float measured_voltage)
{
//	GPIOB_PSOR |= (1<<RED_LED_PIN);
//	GPIOB_PSOR |= (1<<GREEN_LED_PIN);
		//GPIOB_PSOR |= (1<<YELLOW_LED_PIN);
	
	if(measured_voltage < 2.0)
	{
		if(flag_green == 0)
		{	
			GPIOB_PTOR |= (1<<GREEN_LED_PIN);
			flag_green = 1;
		}
		if(flag_red == 1)
		{
			GPIOB_PTOR |= (1<<RED_LED_PIN);
		}
		if(flag_yellow == 1)
		{
			GPIOA_PTOR |= (1<<YELLOW_LED_PIN);
		}
		flag_red = 0;
		flag_yellow = 0;
		
		UART0_Transmit(0x47);
	}
	else
		if(measured_voltage < 4.0)
		{
			if(flag_yellow == 0)
			{	
				GPIOA_PTOR |= (1<<YELLOW_LED_PIN);
				flag_yellow = 1;
			}
			if(flag_red == 1)
			{
				GPIOB_PTOR |= (1<<RED_LED_PIN);
			}
			if(flag_green == 0)
			{
				GPIOB_PTOR |= (1<<GREEN_LED_PIN);
			}
			flag_red = 0;
			flag_green = 1;
			
				UART0_Transmit(0x59);
		}
		else
		{
			if(flag_red == 0)
			{	
				GPIOB_PTOR |= (1<<RED_LED_PIN);
				flag_red = 1;
			}
			if(flag_green == 0)
			{
				GPIOB_PTOR |= (1<<GREEN_LED_PIN);
			}
			if(flag_yellow == 0)
			{
				GPIOA_PTOR |= (1<<YELLOW_LED_PIN);
			}
			flag_green = 1;
			flag_yellow = 1;
			
			UART0_Transmit(0x52);
		}
}

void RGBLed_Init(void){
	
	// Activarea semnalului de ceas pentru pinii folositi în cadrul led-ului RGB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTA_MASK;
	
	// --- RED LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<RED_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
  GPIOB_PCOR |= (1<<RED_LED_PIN);
	
	
	// --- GREEN LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<GREEN_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOB_PCOR |= (1<<GREEN_LED_PIN);
	
	//----YELLOW LED ----
	
	PORTA->PCR[YELLOW_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[YELLOW_LED_PIN] |= PORT_PCR_MUX(1);
	
		// Configurare pin pe post de output
	GPIOA_PDDR |= (1<<YELLOW_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOA_PCOR |= (1<<YELLOW_LED_PIN);
	
	
	//LED-ul de pe placuta:
	//----- RED ----
	PORTB->PCR[PLACA_RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PLACA_RED_LED_PIN] |= PORT_PCR_MUX(1);
	
		// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<PLACA_RED_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOB_PSOR |= (1<<PLACA_RED_LED_PIN);
	
	//----- GREEN ---
	PORTB->PCR[PLACA_GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PLACA_GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
		// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<PLACA_GREEN_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOB_PSOR |= (1<<PLACA_GREEN_LED_PIN);
	
	// ---- BLUE ----
	PORTD->PCR[PLACA_BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PLACA_BLUE_LED_PIN] |= PORT_PCR_MUX(1);
	
		// Configurare pin pe post de output
	GPIOD_PDDR |= (1<<PLACA_BLUE_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOD_PSOR |= (1<<PLACA_BLUE_LED_PIN);
	state = 0;
}
