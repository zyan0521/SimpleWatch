#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "OLED.h"
#include "Key.h"

uint8_t MenuPageSele = 0;	//select menu or setting


void MenuAPB_Init(void)
{
	MyRTC_Init();
	Key_Init();
}

/*-----------show the memu UI------------*/

void Menu_ShowUI(void)
{
	MyRTC_ReadTime();
	
	OLED_Printf(0,0,OLED_6X8,"%4d-%02d-%02d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
	OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
	OLED_ShowString(0,48,"菜单",OLED_8X16);
	OLED_ShowString(96,48,"设置",OLED_8X16);
	//OLED_Update();
}

/*----------Use Key to control mune--------*/

// slecect page
uint8_t Get_MenuPage(void) {
	uint8_t KeyNum;
	while(1) {
        KeyNum = Key_GetNum();
        if(KeyNum == 1) {
            MenuPageSele = 1;
        }
        else if(KeyNum == 2) {
            MenuPageSele = 2;
        }
        else if(KeyNum == 3) {
            OLED_Clear();
            OLED_Update();
            break;
        }
		
		switch(MenuPageSele) {
			case 1:
				Menu_ShowUI();
				OLED_ReverseArea(0,48,32,16);
				OLED_Update();
				break;
			
			case 2:
				Menu_ShowUI();
				OLED_ReverseArea(96,48,32,16);
				OLED_Update();
				break;
		}
    }
    return MenuPageSele;
}

