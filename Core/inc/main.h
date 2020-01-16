#pragma once

/*********************************************************************/

/**********************	DEFINE	**************************************/
#define MATRIX_POLL_PERIOD 50				//	polling period value in milliseconds
#define LONG_PRESS_TIMER_MAX 20				//	set long press delay time to 1 second (20 if polling period 50 ms)

#define KEY_PRESS_COUNT_SINGLE 1
#define KEY_PRESS_COUNT_DOUBLE 2

#define HANDLE_DELAY 50

#define ROW 4
#define COLUMN 3



typedef enum {
	LED_TURN_ON,
	LED_TURN_OFF,
} LED_TURN_typedef;

typedef enum {
	LED_COLOR_RED,
	LED_COLOR_GREEN,
	LED_COLOR_BLUE,
	LED_COLOR_ORANGE,
} LED_COLOR_typedef;

typedef struct {
	char key;
	FlagStatus prevState;
	FlagStatus curState;
	FlagStatus pressStart;
	FlagStatus shortPressFinish;
	FlagStatus longPressFinish;
	FlagStatus longPressHold;
	uint32_t longPressTimer;
} KEY_typedef;



/*************************	FUNCTIONS PROTOTYPE	******************************/
void initGPIO(void);
void switchLED(LED_COLOR_typedef color, LED_TURN_typedef turn);
void taskPollMatrix(void *pvParameters);
void taskKeyHandle(void *pvParameters);

void single_key_pressed(char key);
void double_key_pressed(char key1, char key2);
void single_key_long_pressed(char key);
void double_key_long_pressed(char key1, char key2);

int main(void);
