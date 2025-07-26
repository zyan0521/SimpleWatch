#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Menu.h"
#include "Key.h"
#include "Timer.h"

//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//write in Timer.c

//the number of current page
uint8_t PageNum;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	OLED_Init();
	OLED_Clear();
	Timer_Init();
	MenuAPB_Init();

	while (1)
	{
//		Menu_ShowUI();
		PageNum = Get_MenuPage();
		if(PageNum == 1)
		{
		
		}
		else if(PageNum == 2)
		{
			SettingPage();
		}
	}
}

void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
		Key_Tick();	
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
