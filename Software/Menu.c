#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "OLED.h"
#include "Key.h"
#include "SetTime.h"

#define SettingPageItemNum		2


uint8_t MenuPageSele = 0;	//select menu or setting
uint8_t SettingPageSele = 1;
uint8_t SettingPageSele_Temp;
uint8_t KeyNum;	//it is different form Key_Num in Key.c


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
            return MenuPageSele;
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
}

/*-----------show the setting UI------------*/

void Setting_ShowUI(void){
	OLED_ShowString(48,0,"设置",OLED_8X16);
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,32,"日期时间        ",OLED_8X16);
	//OLED_Update();
}

uint8_t SettingPage(void) {
	while(1) {
        KeyNum = Key_GetNum();
        if(KeyNum == 1) {
            SettingPageSele--;
			if(SettingPageSele < 1){
				SettingPageSele = SettingPageItemNum;
			}
        }
        else if(KeyNum == 2) {
            SettingPageSele++;
			if(SettingPageSele > SettingPageItemNum){
				SettingPageSele = 1;
			}
        }
        else if(KeyNum == 3) {
            OLED_Clear();
            OLED_Update();
            SettingPageSele_Temp = SettingPageSele;
			if(SettingPageSele_Temp == 1)	return 0;
			else if(SettingPageSele_Temp == 2){SetTimePage();}	//enter the time setup page	
        }
		
		switch(SettingPageSele) {
			case 1:
				Setting_ShowUI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			
			case 2:
				Setting_ShowUI();
				OLED_ReverseArea(0,32,128,16);
				OLED_Update();
				break;
			//...
			//case SettingPageItemNum:
		}
    }
}
