#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define KEY_PORT	GPIOA
#define KEY_1		GPIO_Pin_4
#define KEY_2		GPIO_Pin_2
#define KEY_3		GPIO_Pin_0

uint8_t KeyNum = 0;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = KEY_1 | KEY_2 | KEY_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(KEY_PORT,&GPIO_InitStruct);
}

static uint8_t Key_GetState(void)
{
	uint8_t keystate = 0;
	if(	GPIO_ReadInputDataBit(KEY_PORT,KEY_1) == 0){	
		keystate = 1;
	}
	if(GPIO_ReadInputDataBit(KEY_PORT,KEY_2) == 0){	
		keystate = 2;
	}	
	if(GPIO_ReadInputDataBit(KEY_PORT,KEY_3) == 0){	
		keystate = 3;
	}
	return keystate;
}

void Key_Tick(void){
	static uint8_t PreState = 0,CurState = 0;
	PreState = CurState;
	CurState = Key_GetState();
	if (PreState != 0 && CurState == 0){
		KeyNum = PreState;
	}
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if(KeyNum)
	{
		Temp=KeyNum;
		KeyNum=0;
		return Temp;
	}
	else
	{
		return 0;
	}
}
