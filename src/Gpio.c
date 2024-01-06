#include "Gpio.h"
#include "Uart.h"

#define SWITCH_PIN (4) /* PORT D */

#define RED_LED_PIN (3) /* PORT B */

#define GREEN_LED_PIN (1) /* PORT B */

#define YELLOW_LED_PIN (12) /* port A */

/*LED-urile de pe placuta*/

#define PLACA_RED_LED_PIN (18) /* PORT B */
#define PLACA_GREEN_LED_PIN (19) /* PORT B */
#define PLACA_BLUE_LED_PIN (1) /* PORT D */

uint8_t stateR = 0;
uint8_t stateL = 0; 
uint8_t flag_green = 0;
uint8_t flag_yellow = 0;
uint8_t flag_red = 0;
volatile uint8_t ledDirection = 0;

void RGBLed_Init(void){
	
/* Activarea semnalului de ceas pentru pinii folositi in cadrul led-ului RGB */
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTA_MASK;
	
/*	 --- RED LED --- */
	
	 /* Utilizare GPIO ca varianta de multiplexare */ 
	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
	
	/*  Configurare pin pe post de output */
	GPIOB_PDDR |= (1<<RED_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */
  GPIOB_PCOR |= (1<<RED_LED_PIN);
	
	
	/* --- GREEN LED --- */
	
	/* Utilizare GPIO ca varianta de multiplexare */
	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
	/* Configurare pin pe post de output */
	GPIOB_PDDR |= (1<<GREEN_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */
	GPIOB_PCOR |= (1<<GREEN_LED_PIN);
	
	/* ----YELLOW LED ---- */
	
	PORTA->PCR[YELLOW_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[YELLOW_LED_PIN] |= PORT_PCR_MUX(1);
	
		/* Configurare pin pe post de output */
	GPIOA_PDDR |= (1<<YELLOW_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */
	GPIOA_PCOR |= (1<<YELLOW_LED_PIN);
	
	
	/* LED-ul de pe placuta: */
	/* ----- RED ----  */
	PORTB->PCR[PLACA_RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PLACA_RED_LED_PIN] |= PORT_PCR_MUX(1);
	
		/* Configurare pin pe post de output */
	GPIOB_PDDR |= (1<<PLACA_RED_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */
	GPIOB_PSOR |= (1<<PLACA_RED_LED_PIN);
	
	/* ----- GREEN --- */
	PORTB->PCR[PLACA_GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PLACA_GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
		/* Configurare pin pe post de output */
	GPIOB_PDDR |= (1<<PLACA_GREEN_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */ 
	GPIOB_PSOR |= (1<<PLACA_GREEN_LED_PIN);
	
	/* ---- BLUE ---- */
	PORTD->PCR[PLACA_BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PLACA_BLUE_LED_PIN] |= PORT_PCR_MUX(1);
	
		/* Configurare pin pe post de output */
	GPIOD_PDDR |= (1<<PLACA_BLUE_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 0 logic) */
	GPIOD_PSOR |= (1<<PLACA_BLUE_LED_PIN);
	stateR = 0;
	stateL = 5;
}


void ChangePlacaColor(void)
{
	if (ledDirection == 0)
	{
		if(stateL != 5)
		{
			GPIOB_PSOR |= (1<<PLACA_GREEN_LED_PIN);
			GPIOB_PSOR |= (1<<PLACA_RED_LED_PIN);
			GPIOD_PSOR |= (1<<PLACA_BLUE_LED_PIN);
			stateR = 0;
			stateL = 5;
		}
		if(stateR == 0)
		{
			/*turn on all leds */
			GPIOB_PTOR |= (1<<PLACA_GREEN_LED_PIN);
			GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
			GPIOD_PTOR |= (1<<PLACA_BLUE_LED_PIN);
			
			stateR = 1;
		}
		else if(stateR == 1)
		{
			/* turn off red and blue*/
			GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
			GPIOD_PTOR |= (1<<PLACA_BLUE_LED_PIN);
			
			stateR = 2;
		}
		else if (stateR == 2)
		{
			/* turn on red*/
			GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
		
			stateR = 3;
		}
		else
		{
			/* turn off red and green*/
			GPIOB_PTOR |= (1<<PLACA_GREEN_LED_PIN);
			GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
			
			stateR = 0;

		}
	}
	else
	{
		if(stateR != 5)
		{
			GPIOB_PSOR |= (1<<PLACA_GREEN_LED_PIN);
			GPIOB_PSOR |= (1<<PLACA_RED_LED_PIN);
			GPIOD_PSOR |= (1<<PLACA_BLUE_LED_PIN);
			
			stateL = 0;
			stateR = 5;
		}
		if(stateL == 0)
		{
			GPIOB_PSOR |= (1<<PLACA_GREEN_LED_PIN);
			GPIOB_PSOR |= (1<<PLACA_RED_LED_PIN);
			GPIOD_PSOR |= (1<<PLACA_BLUE_LED_PIN);
			
			stateL = 1;
		}
		else if (stateL == 1)
		{
			/* turn on*/
			GPIOB_PTOR |= (1<<PLACA_GREEN_LED_PIN);
			GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
			
			stateL=2;
		}
		else if (stateL == 2)
		{
			/* turn off*/
			GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
			
			stateL =3; 
		}
		else 
		{
			/* turn on*/
			GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
			GPIOD_PTOR |= (1<<PLACA_BLUE_LED_PIN);
			
			stateL = 0;
		}
	}
}

void ChangeColorFromFlame(float measured_voltage)
{	
	if(measured_voltage < 2.0f)
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
		
	}
	else
		if(measured_voltage < 4.0f)
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
			
		}
}
