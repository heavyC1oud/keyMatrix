#pragma once

/*********************************************************************/

/**********************	DEFINE	**************************************/
#define LONG_PRESS_DELAY_COUNT 50			//	set long press delay time to 1 second (polling period 20 ms)

#define ROW 4
#define COLUMN 3


typedef struct {
	char key;
	FlagStatus prevState;
	FlagStatus curState;
	FlagStatus press;
	FlagStatus longPress;
	FlagStatus longPressHold;
	uint16_t delayCount;
} KEY_typedef;



/*************************	FUNCTIONS PROTOTYPE	******************************/
void initGPIO(void);
void pollMatrix(void);
void keyHandle(void);

void single_key_pressed(char key);
void double_key_pressed(char key1, char key2);
void single_key_long_pressed(char key);
void double_key_long_pressed(char key1, char key2);

int main(void);
