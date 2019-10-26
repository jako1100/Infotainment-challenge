/*
 * 		Challenge: Infotainment ( LCD & Keypad using small OS)
 *
 *      Author: MAhmoud Yaakoub
 *      		Mostafa Ashry
 *
 *      Date : 26 - 10 - 2019
 */

#include "LCD.h"
#include "OS.h"
#include "keypad.h"

#define NOT_INIT          0
#define INIT              1

#define CHOISE1           1
#define CHOISE2           2
#define CHOISE3           3
#define CHOISE4           4

#define NEXT_KEY          7

#define RIGHT     100
#define WRONG       101
#define INVALID_KEY     102
#define SKIP       103

#define INTRO_SCREEN     22
#define QUES_DISP        33
#define QUES_RESULT_DISP 44
#define FINAL_SCORE_DISP 55


/*******************************************************
 * 	     Global variables Shared between Tasks
 *******************************************************/

uint8 g_Init = NOT_INIT;
uint8 g_keyPadPressed = NO_KEY_PRESSED;
uint8 g_LCD_dispChar=0;
uint8 *g_LCD_dispString=NULL;
uint8 g_LCD_DispCharRequest = 0;
uint8 g_LCD_dispStringRequest = 0;
uint8 g_LCD_clearRequest = 0;


/*******************************************************
 * 		Questions, 	Answers,  Score
 *******************************************************/

uint8 introScreen[32]= "   Welcome to     Final 5 Game  ";

uint8 aQuestions[5][32] =
{{"Man city win UCL  1   0   2   3 "},
{"Max speed cheeta80 140 120 90 Km"},\
{" Bulgaria is in N S E W of UK "},\
{"First car was in 19(02,05,08,09)"},\
{"Strongest Muscle HEA THI FAK BAC"}};

uint8 Answers[5] =
{CHOISE1,\
		CHOISE1,\
		CHOISE2,
		CHOISE1,\
		CHOISE2};

uint8 aResults[4][32] =
{{"  Ed8t sa7       ya 3aaaaam !"},\
{"   Wla Yhemk      blashoh 5ales "},\
{"   Ya waaaaad     ya  Gamed "},\
{"Ya 3m el game de  m4 ll a8bia ?!"}};

uint8 yourFinalScore[6][32] =
{{"lazm teheen         nfsk kda ?"},\
{" Gamal abdel         Nasser gdn"},\
{"y3ny ygy mnk        bs m4 awy "},\
{"48aaal ya m3lm"},\
{"ba2y 3el 7lw         takkaaaaa"},\
{"Mbora7a 3leena Ya    3aam El4ba7"}};


/*******************************************************
 * 	@ Task Name        :    gameStates
 * 	@ Brief            :    This task is to handle all states of this game
 * 	                        like question display and your answer and final score
 * 	@ Task Periodicity :  	100ms
 *******************************************************/

void gameStates(void)
{
	static volatile uint8 gameState = INTRO_SCREEN;
	static volatile uint8 firstEntry = 0;
	static volatile uint8 questionNumber=0;
	static volatile uint8 result=0;
	static volatile uint8 delayTime = 0;
	static volatile uint8 score=0;

	if(g_Init == INIT)
	{
		if(delayTime == 0)
		{
			switch(gameState)
			{
			case INTRO_SCREEN:
				if(firstEntry == 0)
				{
					firstEntry=1;
					g_LCD_dispStringRequest = 1;
					g_LCD_dispString = introScreen;
				}
				if(g_keyPadPressed == NEXT_KEY)
				{
					gameState = QUES_DISP;
					firstEntry =0;
					g_keyPadPressed = NO_KEY_PRESSED;
				}
				break;

			case QUES_DISP:
				if(firstEntry == 0)
				{
					firstEntry=1;
					g_LCD_clearRequest = 1;
				}
				if(g_LCD_clearRequest ==0)
				{
					g_LCD_clearRequest =3;
					g_LCD_dispStringRequest = 1;
					g_LCD_dispString = aQuestions[questionNumber];

				}
				if(g_keyPadPressed == NEXT_KEY)
				{
					result = SKIP;
					gameState = QUES_RESULT_DISP;
					firstEntry =0;
					g_keyPadPressed = NO_KEY_PRESSED;
				}
				else if((g_keyPadPressed == CHOISE1)\
						||(g_keyPadPressed == CHOISE2)\
						||(g_keyPadPressed ==CHOISE3)\
						||(g_keyPadPressed ==CHOISE4))
				{
					if(g_keyPadPressed == Answers[questionNumber])
						result = RIGHT;
					else
						result = WRONG;

					gameState = QUES_RESULT_DISP;
					firstEntry =0;
					g_keyPadPressed = NO_KEY_PRESSED;
				}
				else if(g_keyPadPressed != NO_KEY_PRESSED)
				{
					result = INVALID_KEY;
					gameState = QUES_RESULT_DISP;
					firstEntry =0;
					g_keyPadPressed = NO_KEY_PRESSED;
				}


				break;

			case QUES_RESULT_DISP:

				if(firstEntry == 0)
				{
					firstEntry=1;
					g_LCD_clearRequest = 1;
				}


				switch(result)
				{
				case INVALID_KEY:

					if(g_LCD_clearRequest ==0)
					{
						g_LCD_clearRequest =3;
						g_LCD_dispStringRequest = 1;
						g_LCD_dispString = aResults[0];
						firstEntry=0;
						delayTime = 10;
						gameState = QUES_DISP;
					}

					break;
				case SKIP:

					if(g_LCD_clearRequest ==0)
					{
						g_LCD_clearRequest =3;
						g_LCD_dispStringRequest = 1;
						g_LCD_dispString = aResults[1];
						firstEntry=0;
						delayTime = 10;
						questionNumber++;
						if(questionNumber > 4)
						{
							gameState = FINAL_SCORE_DISP;
						}
						else
						{
							gameState = QUES_DISP;
						}
					}

					break;
				case RIGHT:

					if(g_LCD_clearRequest ==0)
					{
						g_LCD_clearRequest =3;
						score++;
						g_LCD_dispStringRequest = 1;
						g_LCD_dispString = aResults[2];
						firstEntry=0;
						delayTime = 10;
						questionNumber++;
						if(questionNumber > 4)
						{
							gameState = FINAL_SCORE_DISP;
						}
						else
						{
							gameState = QUES_DISP;
						}
					}

					break;
				case WRONG:

					if(g_LCD_clearRequest ==0)
					{
						g_LCD_clearRequest =3;
						g_LCD_dispStringRequest = 1;
						g_LCD_dispString = aResults[3];
						firstEntry=0;
						delayTime = 10;
						questionNumber++;
						if(questionNumber > 4)
						{
							gameState = FINAL_SCORE_DISP;
						}
						else
						{
							gameState = QUES_DISP;
						}
					}

					break;

				}

				break;
				case FINAL_SCORE_DISP:
					if(firstEntry == 0)
					{
						firstEntry=1;
						g_LCD_clearRequest = 1;
					}

					if(g_LCD_clearRequest ==0)
					{
						g_LCD_clearRequest=3;
						g_LCD_dispString = yourFinalScore[score];
						g_LCD_dispStringRequest = 1;
					}

					break;
			}
		}

		else
		{
			delayTime--;
		}
	}
}
/*
 * @ Task name : initSystem
 * @ Brief     : this task to handle the system initialization. No other
 *               task can run unless init is finished.
 * */

void initSystem(void)
{
	static uint8 DIO_init_status = NOT_INIT;
	static uint8 LCD_init_status = NOT_INIT;

	if(g_Init == NOT_INIT)
	{
		if(DIO_init_status != INIT)
		{
			DIO_init();
			DIO_init_status = INIT;
		}

		if(LCD_init_status != INIT)
		{
			LCD_init_status = LCD_init(LCD_ptrConfig);
			if(LCD_init_status == LCD_DONE)
			{
				LCD_init_status = INIT;
				g_Init = INIT;
			}
		}

	}
}

void LCD_dispChar (void)
{
	uint8 LCD_dispFuncStatus;
	if(g_Init == INIT)
	{
		if(g_LCD_DispCharRequest ==1)
		{
			LCD_dispFuncStatus = LCD_displayChar(g_LCD_dispChar);
			if(LCD_dispFuncStatus == LCD_DONE)
			{
				g_LCD_DispCharRequest = 0;
			}
		}
	}
}

void Keypad_getKey(void)
{
	uint8 LOC_pressedKey;
	uint8 keyPad_Status;
	static prevKey = NO_KEY_PRESSED;
	if(g_Init == INIT)
	{
		keyPad_Status = KeyPad_getPressedKey(&LOC_pressedKey);
		if(keyPad_Status == KEYPAD_DONE)
		{
			if(prevKey != LOC_pressedKey)
			{
				prevKey = LOC_pressedKey;
				g_keyPadPressed = LOC_pressedKey;
			}


		}
		else if(keyPad_Status == NO_KEY_PRESSED)
		{
			prevKey = NO_KEY_PRESSED;
		}
	}
}

void LCD_dispStr (void)
{
	uint8 LCD_dispFuncStatus = 0;
	uint8 LCD_clearFuncStatus = 0;

	if(g_Init == INIT)
	{
		if(g_LCD_clearRequest == 1)
		{
			LCD_clearFuncStatus = LCD_clear();
			if(LCD_clearFuncStatus == LCD_DONE)
			{
				g_LCD_clearRequest = 0;
			}
		}

		if(g_LCD_dispStringRequest ==1)
		{
			LCD_dispFuncStatus = LCD_displayString(g_LCD_dispString);
			if(LCD_dispFuncStatus == LCD_STR_COMPELET)
			{
				g_LCD_dispStringRequest = 0;
			}
		}
	}
}

void main(void)
{
	REG_DDRB = 0xFF;
	REG_DDRD = 0xFF;
	OS_Init(&OS_Config_STR_t);
	sei();

	Create_Task(1,initSystem,0);
	Create_Task(1,LCD_dispStr,0);
	Create_Task(30,Keypad_getKey,0);
	Create_Task(100,gameStates,0);

	SOS_Run();

}

