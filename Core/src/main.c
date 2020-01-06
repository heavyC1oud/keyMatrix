
#include <stddef.h>
#include "stm32f0xx.h"
#include "interrupt.h"
#include "main.h"

/*********************************************************************/

/**********************	VARIABLE	**********************/
const char key[ROW][COLUMN] = {{'1', '2', '3'},
							   {'4', '5', '6'},
							   {'7', '8', '9'},
							   {'*', '0', '#'}};

KEY_typedef matrix[ROW][COLUMN];


/*************************	FUNCTION	******************************/

/**********************************************************************
*	function name	:	initGPIO
*	Description		:	GPIO initialization
*	Arguments		:	none
*	Return value	:	none
**********************************************************************/
void initGPIO(void)
{
	//	enable PORTA/PORTB bus
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;

	//	scan GPIO (columns) set as output open-drain default HI-Z
	//	CLM_1 / PORTA[0]
	//	CLM_2 / PORTA[1]
	//	CLM_3 / PORTA[2]
	GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2);
	GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0;

	GPIOA->OTYPER |= GPIO_OTYPER_OT_0 | GPIO_OTYPER_OT_1 | GPIO_OTYPER_OT_2;

	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR0 | GPIO_OSPEEDR_OSPEEDR1 | GPIO_OSPEEDR_OSPEEDR2;

	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2);

	GPIOA->BSRR = GPIO_BSRR_BS_0 | GPIO_BSRR_BS_1 | GPIO_BSRR_BS_2;

	//	poll GPIO (rows) set as input pull-up
	//	ROW_1 / PORTB[0]
	//	ROW_2 / PORTB[1]
	//	ROW_3 / PORTB[2]
	//	ROW_4 / PORTB[3]
	GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3);

	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3);
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR0_0 | GPIO_PUPDR_PUPDR1_0 | GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR3_0;
}
/*********************************************************************/


/**********************************************************************
*	function name	:	pollMatrix
*	Description		:	polling matrix keyboard
*	Arguments		:	none
*	Return value	:	none
**********************************************************************/
void pollMatrix(void)
{
	uint8_t state = 0;

	for(uint8_t j = 0; j < ROW; j++) {
		for(uint8_t i = 0; i < COLUMN; i++) {
			matrix[j][i].curState = RESET;
		}
	}

	//	scan column 1
	GPIOA->BSRR = GPIO_BSRR_BR_0 | GPIO_BSRR_BS_1 | GPIO_BSRR_BS_2;
	//	'1' - key
	//	polling row 1
	state = GPIOB->IDR & GPIO_IDR_0;
	if(state) {
		matrix[0][0].curState = SET;
		matrix[0][0].delayCount++;
	}

	//	'4' - key
	//	polling row 2
	state = GPIOB->IDR & GPIO_IDR_1;
	if(state) {
		matrix[1][0].curState = SET;
		matrix[1][0].delayCount++;
	}

	//	'7' - key
	//	polling row 3
	state = GPIOB->IDR & GPIO_IDR_2;
	if(state) {
		matrix[2][0].curState = SET;
		matrix[2][0].delayCount++;
	}

	//	'*' - key
	//	polling row 4
	state = GPIOB->IDR & GPIO_IDR_3;
	if(state) {
		matrix[3][0].curState = SET;
		matrix[3][0].delayCount++;
	}

	//	scan column 2
	GPIOA->BSRR = GPIO_BSRR_BS_0 | GPIO_BSRR_BR_1 | GPIO_BSRR_BS_2;
	//	'2' - key
	//	polling row 1
	state = GPIOB->IDR & GPIO_IDR_0;
	if(state) {
		matrix[0][1].curState = SET;
		matrix[0][1].delayCount++;
	}

	//	'5' - key
	//	polling row 2
	state = GPIOB->IDR & GPIO_IDR_1;
	if(state) {
		matrix[1][1].curState = SET;
		matrix[1][1].delayCount++;
	}

	//	'8' - key
	//	polling row 3
	state = GPIOB->IDR & GPIO_IDR_2;
	if(state) {
		matrix[2][1].curState = SET;
		matrix[2][1].delayCount++;
	}

	//	'0' - key
	//	polling row 4
	state = GPIOB->IDR & GPIO_IDR_3;
	if(state) {
		matrix[3][1].curState = SET;
		matrix[3][1].delayCount++;
	}

	//	scan column 3
	GPIOA->BSRR = GPIO_BSRR_BS_0 | GPIO_BSRR_BS_1 | GPIO_BSRR_BR_2;
	//	'3' - key
	//	polling row 1
	state = GPIOB->IDR & GPIO_IDR_0;
	if(state) {
		matrix[0][2].curState = SET;
		matrix[0][2].delayCount++;
	}

	//	'6' - key
	//	polling row 2
	state = GPIOB->IDR & GPIO_IDR_1;
	if(state) {
		matrix[1][2].curState = SET;
		matrix[1][2].delayCount++;
	}

	//	'9' - key
	//	polling row 3
	state = GPIOB->IDR & GPIO_IDR_2;
	if(state) {
		matrix[2][2].curState = SET;
		matrix[2][2].delayCount++;
	}

	//	'#' - key
	//	polling row 4
	state = GPIOB->IDR & GPIO_IDR_3;
	if(state) {
		matrix[3][2].curState = SET;
		matrix[3][2].delayCount++;
	}

	//	set scan pins to hi-z
	GPIOA->BSRR = GPIO_BSRR_BS_0 | GPIO_BSRR_BS_1 | GPIO_BSRR_BS_2;

	//	handling collected data
	for(uint8_t j = 0; j < ROW; j++) {
		for(uint8_t i = 0; i < COLUMN; i++) {
			//	check press
			if((matrix[j][i].prevState == SET) && (matrix[j][i].curState == RESET) && (matrix[j][i].delayCount <= LONG_PRESS_DELAY_COUNT)) {
				matrix[j][i].delayCount = 0;
				matrix[j][i].press = SET;
			}
			//	check long press
			else if((matrix[j][i].delayCount > LONG_PRESS_DELAY_COUNT) && (matrix[j][i].longPressHold == RESET)) {
				matrix[j][i].longPress = SET;
				matrix[j][i].longPressHold = SET;
			}
			//	check long press release
			else if((matrix[j][i].prevState == SET) && (matrix[j][i].curState == RESET) && (matrix[j][i].delayCount > LONG_PRESS_DELAY_COUNT)) {
				matrix[j][i].longPressHold = RESET;
				matrix[j][i].delayCount = 0;
			}

			//	no event
			matrix[j][i].prevState = matrix[j][i].curState;
		}
	}
}
/*********************************************************************/


/**********************************************************************
*	function name	:	keyHandle
*	Description		:	handling matrix keyboard
*	Arguments		:	none
*	Return value	:	none
**********************************************************************/
void keyHandle(void)
{
	uint8_t shortKeyPressed = 0;
	uint8_t longKeyPressed = 0;
	uint8_t keySet = 0;
	char key1 = 0;
	char key2 = 0;

	//	calculate pressed keys
	for(uint8_t j = 0; j < ROW; j++) {
		for(uint8_t i = 0; i < COLUMN; i++) {
			if(matrix[j][i].press == SET) {
				shortKeyPressed++;
			}
			else if(matrix[j][i].longPress == SET) {
				longKeyPressed++;
			}
		}
	}

	//	short single pressed context
	if(shortKeyPressed == 1) {
		for(uint8_t j = 0; j < ROW; j++) {
			for(uint8_t i = 0; i < COLUMN; i++) {
				if(matrix[j][i].press == SET) {
					matrix[j][i].press = RESET;

					single_key_pressed(matrix[j][i].key);

					return;
				}
			}
		}
	}
	//	short double pressed context
	else if(shortKeyPressed > 1) {
		for(uint8_t j = 0; j < ROW; j++) {
			for(uint8_t i = 0; i < COLUMN; i++) {
				if(matrix[j][i].press == SET) {
					matrix[j][i].press = RESET;

					if(keySet == 0) {
						key1 = matrix[j][i].key;
						keySet++;
					}
					else {
						key2 = matrix[j][i].key;

						double_key_pressed(key1, key2);
					}

					return;
				}
			}
		}
	}
	//	long single pressed context
	else if(longKeyPressed == 1) {
		for(uint8_t j = 0; j < ROW; j++) {
			for(uint8_t i = 0; i < COLUMN; i++) {
				if(matrix[j][i].longPress == SET) {
					matrix[j][i].longPress = RESET;

					single_key_long_pressed(matrix[j][i].key);

					return;
				}
			}
		}
	}
	//	long double pressed context
	else if(longKeyPressed > 1) {
		for(uint8_t j = 0; j < ROW; j++) {
			for(uint8_t i = 0; i < COLUMN; i++) {
				if(matrix[j][i].longPress == SET) {
					matrix[j][i].longPress = RESET;

					if(keySet == 0) {
						key1 = matrix[j][i].key;
						keySet++;
					}
					else {
						key2 = matrix[j][i].key;

						double_key_long_pressed(key1, key2);
					}

					return;
				}
			}
		}
	}
}
/*********************************************************************/



void single_key_pressed(char key)
{

}

void double_key_pressed(char key1, char key2)
{

}

void single_key_long_pressed(char key)
{

}

void double_key_long_pressed(char key1, char key2)
{

}


/**********************************************************************
*	function name	:	main
*	Description		:	main function
*	Arguments		:	none
*	Return value	:	none
**********************************************************************/
int main(void)
{
	//	clear key matrix flags
	for(uint8_t j = 0; j < ROW; j++) {
		for(uint8_t i = 0; i < COLUMN; i++) {
			matrix[j][i].key = key[j][i];
			matrix[j][i].prevState = RESET;
			matrix[j][i].curState = RESET;
			matrix[j][i].press = RESET;
			matrix[j][i].longPress = RESET;
			matrix[j][i].longPressHold = RESET;
			matrix[j][i].delayCount = 0;
		}
	}

	initGPIO();

	while(1) {
		keyHandle();
	}
}
