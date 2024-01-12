#include "Gpio.h"
#include "Uart.h"

#define RED_LED_PIN (3) /* PORT B */

#define GREEN_LED_PIN (1) /* PORT B */

#define YELLOW_LED_PIN (12) /* port A */

/*LED-urile de pe placuta*/

#define PLACA_RED_LED_PIN (18) /* PORT B */
#define PLACA_GREEN_LED_PIN (19) /* PORT B */
#define PLACA_BLUE_LED_PIN (1) /* PORT D */

static uint8_t flag_green = 0;
static uint8_t flag_yellow = 0;
static uint8_t flag_red = 0;
volatile uint8_t ledDirection = 0;
static uint8_t state = 0;
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
	
	/* Stingerea LED-ului (punerea pe 1 logic) */
	GPIOB_PSOR |= (1<<PLACA_RED_LED_PIN);
	
	/* ----- GREEN --- */
	PORTB->PCR[PLACA_GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PLACA_GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
		/* Configurare pin pe post de output */
	GPIOB_PDDR |= (1<<PLACA_GREEN_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 1 logic) */ 
	GPIOB_PSOR |= (1<<PLACA_GREEN_LED_PIN);
	
	/* ---- BLUE ---- */
	PORTD->PCR[PLACA_BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PLACA_BLUE_LED_PIN] |= PORT_PCR_MUX(1);
	
		/* Configurare pin pe post de output */
	GPIOD_PDDR |= (1<<PLACA_BLUE_LED_PIN);
	
	/* Stingerea LED-ului (punerea pe 1 logic) */
	GPIOD_PSOR |= (1<<PLACA_BLUE_LED_PIN);
}

void black(void)
{
	GPIOB_PSOR |= (1<<PLACA_GREEN_LED_PIN);
	GPIOB_PSOR |= (1<<PLACA_RED_LED_PIN);
	GPIOD_PSOR |= (1<<PLACA_BLUE_LED_PIN);
}

void ChangePlacaColor(void)
{
		if(state == 0)
		{
			black();
			GPIOB_PTOR |= (1<<PLACA_GREEN_LED_PIN);
			GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
			GPIOD_PTOR |= (1<<PLACA_BLUE_LED_PIN);
			
			if(ledDirection == 0)
				state = 1;	
			else
				state = 3;
			
		}
		else if(state == 1)
		{
			black();
			GPIOB_PTOR |= (1<<PLACA_GREEN_LED_PIN);
			if(ledDirection == 0)
				state = 2;
		else
					
				state = 0;
		}
		else if (state == 2)
		{
			black();
			GPIOB_PTOR |= (1<<PLACA_RED_LED_PIN);
			GPIOB_PTOR |= (1<<PLACA_GREEN_LED_PIN);
			if(ledDirection == 0)
				state = 3;
			else		
				state = 1;
		}
		else
		{
			black();
			if(ledDirection == 0)
				state = 0;
			else
				state = 2;
		}
}

void ChangeColorFromFlame(float measured_voltage)
{	
	if(measured_voltage < 1.66f)
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
		if(measured_voltage < 3.32f)
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
