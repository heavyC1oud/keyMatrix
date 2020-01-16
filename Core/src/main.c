
#include <stddef.h>
#include <string.h>
#include "stm32f0xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "main.h"

/*********************************************************************/

/**********************	VARIABLE	**********************/
const char key[ROW][COLUMN] = {{'1', '2', '3'},
							   {'4', '5', '6'},
							   {'7', '8', '9'},
							   {'*', '0', '#'}};

KEY_typedef matrix[ROW][COLUMN] = {0};


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

	//	LED
		//	enable PORTC bus
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	//	PORTC6 - RED
	//	PORTC7 - GREEN
	//	PORTC8 - BLUE
	//	PORTC9 - YELLOW

	//	MODERx[1:0] = 01 - General purpose output mode
	GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;

	//	OTYPERx = 0 - Output push-pull
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);

	//	OSPEEDRx[1:0] = 00 - Low speed
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR6 | GPIO_OSPEEDR_OSPEEDR7 | GPIO_OSPEEDR_OSPEEDR8 | GPIO_OSPEEDR_OSPEEDR9);

	//	PUPDRx[1:0] = 00 - No pull-up, pull-down
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);

	//	BSRRx = 1 - reset bit
	GPIOC->BSRR = GPIO_BSRR_BR_6 | GPIO_BSRR_BR_7 | GPIO_BSRR_BR_8 | GPIO_BSRR_BR_9;
}
/*********************************************************************/


/**********************************************************************
*	function name	:	switchLED
*	Description		:	turn on/off LED
*	Arguments		:	pvParameters - freeRTOS argument (unused)
*	Return value	:	none
**********************************************************************/
void switchLED(LED_COLOR_typedef color, LED_TURN_typedef turn)
{
	switch(color) {
	case LED_COLOR_RED:
		if(turn == LED_TURN_ON) {
			GPIOC->BSRR = GPIO_BSRR_BS_6;
		}
		else {
			GPIOC->BSRR = GPIO_BSRR_BR_6;
		}

		break;
	case LED_COLOR_GREEN:
		if(turn == LED_TURN_ON) {
			GPIOC->BSRR = GPIO_BSRR_BS_7;
		}
		else {
			GPIOC->BSRR = GPIO_BSRR_BR_7;
		}

		break;
	case LED_COLOR_BLUE:
		if(turn == LED_TURN_ON) {
			GPIOC->BSRR = GPIO_BSRR_BS_8;
		}
		else {
			GPIOC->BSRR = GPIO_BSRR_BR_8;
		}

		break;
	case LED_COLOR_YELLOW:
		if(turn == LED_TURN_ON) {
			GPIOC->BSRR = GPIO_BSRR_BS_9;
		}
		else {
			GPIOC->BSRR = GPIO_BSRR_BR_9;
		}

		break;
	default:
		break;
	}
}
/*********************************************************************/


/**********************************************************************
*	function name	:	taskPollMatrix
*	Description		:	polling matrix keyboard
*	Arguments		:	pvParameters - freeRTOS argument (unused)
*	Return value	:	none
**********************************************************************/
void taskPollMatrix(void *pvParameters)
{
	uint8_t state = 0;

	while(1) {
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
		if(state) matrix[0][0].curState = SET;

		//	'4' - key
		//	polling row 2
		state = GPIOB->IDR & GPIO_IDR_1;
		if(state) matrix[1][0].curState = SET;

		//	'7' - key
		//	polling row 3
		state = GPIOB->IDR & GPIO_IDR_2;
		if(state) matrix[2][0].curState = SET;

		//	'*' - key
		//	polling row 4
		state = GPIOB->IDR & GPIO_IDR_3;
		if(state) matrix[3][0].curState = SET;

		//	scan column 2
		GPIOA->BSRR = GPIO_BSRR_BS_0 | GPIO_BSRR_BR_1 | GPIO_BSRR_BS_2;
		//	'2' - key
		//	polling row 1
		state = GPIOB->IDR & GPIO_IDR_0;
		if(state) matrix[0][1].curState = SET;

		//	'5' - key
		//	polling row 2
		state = GPIOB->IDR & GPIO_IDR_1;
		if(state) matrix[1][1].curState = SET;

		//	'8' - key
		//	polling row 3
		state = GPIOB->IDR & GPIO_IDR_2;
		if(state) matrix[2][1].curState = SET;

		//	'0' - key
		//	polling row 4
		state = GPIOB->IDR & GPIO_IDR_3;
		if(state) matrix[3][1].curState = SET;

		//	scan column 3
		GPIOA->BSRR = GPIO_BSRR_BS_0 | GPIO_BSRR_BS_1 | GPIO_BSRR_BR_2;
		//	'3' - key
		//	polling row 1
		state = GPIOB->IDR & GPIO_IDR_0;
		if(state) matrix[0][2].curState = SET;

		//	'6' - key
		//	polling row 2
		state = GPIOB->IDR & GPIO_IDR_1;
		if(state) matrix[1][2].curState = SET;

		//	'9' - key
		//	polling row 3
		state = GPIOB->IDR & GPIO_IDR_2;
		if(state) matrix[2][2].curState = SET;

		//	'#' - key
		//	polling row 4
		state = GPIOB->IDR & GPIO_IDR_3;
		if(state) matrix[3][2].curState = SET;

		//	set scan pins to hi-z
		GPIOA->BSRR = GPIO_BSRR_BS_0 | GPIO_BSRR_BS_1 | GPIO_BSRR_BS_2;

		//	handling collected data
		for(uint8_t j = 0; j < ROW; j++) {
			for(uint8_t i = 0; i < COLUMN; i++) {
				//	push key detect
				if((matrix[j][i].curState == SET) && (matrix[j][i].prevState == RESET)) {
					matrix[j][i].longPressTimer++;
				}
				//	release key detect
				else if((matrix[j][i].curState == RESET) && (matrix[j][i].prevState == SET)) {
					//	short press detect
					if(matrix[j][i].longPressTimer <= LONG_PRESS_TIMER_MAX) {
						matrix[j][i].shortPress = SET;
					}

					matrix[j][i].longPressHold = RESET;			//	re-triggering avoid
					matrix[j][i].longPressTimer = 0;
				}
				//	hold press detect
				else if((matrix[j][i].curState == SET) && (matrix[j][i].prevState == SET)) {
					matrix[j][i].longPressTimer++;

					//	long press detect
					if((matrix[j][i].longPressTimer > LONG_PRESS_TIMER_MAX) && (matrix[j][i].longPressHold == RESET)) {
						matrix[j][i].longPressHold = SET;			//	re-triggering avoid
						matrix[j][i].longPress = SET;
					}
				}

				matrix[j][i].prevState = matrix[j][i].curState;

				vTaskDelay(MATRIX_POLL_PERIOD);
			}
		}
	}
}
/*********************************************************************/


/**********************************************************************
*	function name	:	taskKeyHandle
*	Description		:	handling matrix keyboard
*	Arguments		:	pvParameters - freeRTOS argument (unused)
*	Return value	:	none
**********************************************************************/
void taskKeyHandle(void *pvParameters)
{
	uint8_t keyShortPressed = 0;
	uint8_t keyLongPressed = 0;
	uint8_t keySet = 0;
	char key1 = 0;
	char key2 = 0;

//	uint32_t matrixSize = 0;
//	KEY_typedef localMatrix[ROW][COLUMN] = {0};
//
//	matrixSize = sizeof(matrix);
//
//	memcpy(&localMatrix, &matrix, sizeof();


	while(1) {
		//	calculate pressed keys
		for(uint8_t j = 0; j < ROW; j++) {
			for(uint8_t i = 0; i < COLUMN; i++) {
				if(matrix[j][i].shortPress == SET) keyShortPressed++;

				if(matrix[j][i].longPress == SET) keyLongPressed++;
			}
		}

		//	short single pressed context
		if(keyShortPressed == KEY_PRESS_COUNT_SINGLE) {
			for(uint8_t j = 0; j < ROW; j++) {
				for(uint8_t i = 0; i < COLUMN; i++) {
					if(matrix[j][i].shortPress == SET) {
						matrix[j][i].shortPress = RESET;

						single_key_pressed(matrix[j][i].key);

						return;
					}
				}
			}
		}

		//	short double pressed context
		else if(keyShortPressed == KEY_PRESS_COUNT_DOUBLE) {
			for(uint8_t j = 0; j < ROW; j++) {
				for(uint8_t i = 0; i < COLUMN; i++) {
					if(matrix[j][i].shortPress == SET) {
						matrix[j][i].shortPress = RESET;

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

		//	short multi pressed context
		else if(keyShortPressed > KEY_PRESS_COUNT_DOUBLE) {
			for(uint8_t j = 0; j < ROW; j++) {
				for(uint8_t i = 0; i < COLUMN; i++) {
					if(matrix[j][i].shortPress == SET) {
						matrix[j][i].shortPress = RESET;
					}
				}
			}

			return;
		}

		//	long single pressed context
		else if(keyLongPressed == KEY_PRESS_COUNT_SINGLE) {
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
		else if(keyLongPressed == KEY_PRESS_COUNT_DOUBLE) {
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

		//	long multi pressed context
		else if(keyLongPressed > KEY_PRESS_COUNT_DOUBLE) {
			for(uint8_t j = 0; j < ROW; j++) {
				for(uint8_t i = 0; i < COLUMN; i++) {
					if(matrix[j][i].longPress == SET) {
						matrix[j][i].longPress = RESET;
					}
				}
			}
		}
	}
}
/*********************************************************************/



void single_key_pressed(char key)
{
	switchLED(LED_COLOR_RED, LED_TURN_ON);

	vTaskDelay(HANDLE_DELAY);

	switchLED(LED_COLOR_RED, LED_TURN_OFF);
}

void double_key_pressed(char key1, char key2)
{
	switchLED(LED_COLOR_GREEN, LED_TURN_ON);

	vTaskDelay(HANDLE_DELAY);

	switchLED(LED_COLOR_GREEN, LED_TURN_OFF);
}

void single_key_long_pressed(char key)
{
	switchLED(LED_COLOR_BLUE, LED_TURN_ON);

	vTaskDelay(HANDLE_DELAY);

	switchLED(LED_COLOR_BLUE, LED_TURN_OFF);
}

void double_key_long_pressed(char key1, char key2)
{
	switchLED(LED_COLOR_YELLOW, LED_TURN_ON);

	vTaskDelay(HANDLE_DELAY);

	switchLED(LED_COLOR_YELLOW, LED_TURN_OFF);
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
			matrix[j][i].shortPress = RESET;
			matrix[j][i].longPress = RESET;
			matrix[j][i].longPressHold = RESET;
			matrix[j][i].longPressTimer = 0;
		}
	}

	initGPIO();

	xTaskCreate(taskPollMatrix, "POLL MATRIX", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate(taskKeyHandle, "KEY HANDLE", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	vTaskStartScheduler();

	//	program never run here
	while(1);
}
