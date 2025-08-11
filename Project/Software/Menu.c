#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "OLED.h"
#include "Key.h"
#include "SetTime.h"
#include "Menu.h"


uint8_t KeyNum;	//it is different form Key_Num in Key.c


void MenuAPB_Init(void)
{
	MyRTC_Init();
	Key_Init();
}

/*-----------show the memu UI------------*/
uint8_t MenuPageSele = 0;	//select menu or setting

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

#define		SettingPageItemNum						2

uint8_t SettingPageSele = 1;
uint8_t SettingPageSele_Temp;

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

		if(SettingPageSele_Temp == 1)	return 0;	//return last level page
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

/*---------------------------------show the slide menu UI---------------------------------------*/

/*设置滑动菜单界面选择项*/
#define		SlideMenuPageItemNum						7

/*滑动菜单界面选择项变量*/
uint8_t SlideMenuPageSele = 1;
uint8_t SlideMenuPageSele_Temp;

uint8_t PreSelection;
uint8_t TargetSelection;
uint8_t x_Pre =48;
uint8_t speed = 4;
uint8_t MoveFlag;	//开始移动标志位，0:停止，1:开始

/*滑动菜单界面显示UI*/
static void SlideMenu_ShowUI(void) {
	OLED_Clear();
	OLED_ShowImage(42,10,44,44,Frame);
	if (PreSelection < TargetSelection){
		x_Pre -= speed;
		if(x_Pre == 0){
			PreSelection++;
			MoveFlag = 0;
			x_Pre = 48;
		}
	}
	if (PreSelection > TargetSelection){
		x_Pre += speed;
		if(x_Pre == 96){
			PreSelection--;
			MoveFlag = 0;
			x_Pre = 48;
		}
	}
	if (PreSelection > 1){
		OLED_ShowImage(x_Pre - 96,16,32,32,Menu_Graph[PreSelection - 2]);
	}
	if (PreSelection > 0){
		OLED_ShowImage(x_Pre - 48,16,32,32,Menu_Graph[PreSelection - 1]);
	}	
	OLED_ShowImage(x_Pre,16,32,32, Menu_Graph[PreSelection]);
	OLED_ShowImage(x_Pre + 48,16,32,32,Menu_Graph[PreSelection + 1]);
	OLED_ShowImage(x_Pre + 96,16,32,32,Menu_Graph[PreSelection + 2]);
	OLED_Update();
}

/*设定当前选中项和目标选中项,当前选中值大于目标选中值时左移，当前选中值小于目标选中值时右移*/
static void Set_SlideMenuSelection(uint8_t MoveFlag, uint8_t preSelection,uint8_t targetSelection) {
	if (MoveFlag == 1){
		PreSelection = preSelection;
		TargetSelection = targetSelection;
	}
	SlideMenu_ShowUI();
}

/*滑动菜单界面选择项*/
uint8_t SlideMenuPage(void) {
	MoveFlag = 1;
	uint8_t MoveDerection = 1; // 0 左移，1 右移
	while(1) {		
        KeyNum = Key_GetNum();
        if(KeyNum == 1) {
			MoveDerection = 0 ;
			MoveFlag = 1;
            SlideMenuPageSele--;
			if(SlideMenuPageSele < 1){
				SlideMenuPageSele = SlideMenuPageItemNum;
			}
        }
        else if(KeyNum == 2) {
			MoveDerection = 1;
			MoveFlag = 1;
            SlideMenuPageSele++;
			if(SlideMenuPageSele > SlideMenuPageItemNum){
				SlideMenuPageSele = 1;
			}
        }
        else if(KeyNum == 3) {
            OLED_Clear();
            OLED_Update();
            SlideMenuPageSele_Temp = SlideMenuPageSele;

			if(SlideMenuPageSele_Temp == 1)	return 0;	//return last level page
			else if(SlideMenuPageSele_Temp == 2){StopWatchPage();}	//enter the time setup page	
			else if(SlideMenuPageSele_Temp == 3){}	//enter the time setup page
			else if(SlideMenuPageSele_Temp == 4){}	//enter the	 time setup page
			else if(SlideMenuPageSele_Temp == 5){}	
			else if(SlideMenuPageSele_Temp == 6){}	
			else if(SlideMenuPageSele_Temp == SlideMenuPageItemNum){}	
        }
		
		if (SlideMenuPageSele == 1) {
			if (MoveDerection == 0){
				Set_SlideMenuSelection(MoveFlag, 1, 0);
			}
			else {
				Set_SlideMenuSelection(MoveFlag, 0, 0);
			}
		}
		else if (SlideMenuPageSele == SlideMenuPageItemNum) {
			if (MoveDerection == 0){
				Set_SlideMenuSelection(MoveFlag, SlideMenuPageItemNum - 1, SlideMenuPageItemNum - 1);
			}
			else {
				Set_SlideMenuSelection(MoveFlag, SlideMenuPageItemNum - 2, SlideMenuPageItemNum - 1);
			}
		}
		else {
			if (MoveDerection == 0){
				Set_SlideMenuSelection(MoveFlag, SlideMenuPageSele , SlideMenuPageSele - 1);
			}
			else {
				Set_SlideMenuSelection(MoveFlag, SlideMenuPageSele - 2 , SlideMenuPageSele - 1);
			}
		}	
    }
}


/*---------------------------------StopWatch Page---------------------------------------*/

/*展示秒表界面*/
int min,sec,tick;
static void StopWatch_ShowUI(void){
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(32, 20, OLED_8X16, "%02d:%02d:%02d", min, sec, tick);
	OLED_ShowString(8,44,"开始",OLED_8X16);
	OLED_ShowString(48,44,"停止",OLED_8X16);
	OLED_ShowString(88,44,"清除",OLED_8X16);
}

/*秒表计时函数*/
uint8_t StopWatchRunning = 0; //0:停止，1:开始计时
void StopWatch_Tick(void){
	if (StopWatchRunning == 1){
		tick++;
		if(tick >= 100) {
			tick = 0;
			sec++;
			if(sec >= 60) {
				sec = 0;
				min++;
				if(min >= 60) {
					min = 0;
				}
			}
		}
	}
}

#define		StopWatchPageItemNum						4
uint8_t StopWatchPageSele = 1;
uint8_t StopWatchPageSele_Temp;
uint8_t StopWatchPage(void){
	while(1) {
		
        KeyNum = Key_GetNum();
        if(KeyNum == 1) {
            StopWatchPageSele--;
			if(StopWatchPageSele < 1){
				StopWatchPageSele = StopWatchPageItemNum;
			}
        }
        else if(KeyNum == 2) {
            StopWatchPageSele++;
			if(StopWatchPageSele > StopWatchPageItemNum){
				StopWatchPageSele = 1;
			}
        }
        else if(KeyNum == 3) {
            OLED_Clear();
            OLED_Update();
            StopWatchPageSele_Temp = StopWatchPageSele;

		if(StopWatchPageSele_Temp == 1)	return 0;	//return last level page
			//dont have next level;
        }

		switch(StopWatchPageSele) {
			case 1:
				StopWatch_ShowUI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			
			case 2:
				StopWatch_ShowUI();
				StopWatchRunning = 1;
				OLED_ReverseArea(8,44,32,16);
				OLED_Update();
				break;
			case 3:
				StopWatch_ShowUI();
				StopWatchRunning = 0;
				OLED_ReverseArea(48,44,32,16);
				OLED_Update();
				break;
			case 4:
				StopWatch_ShowUI();
				StopWatchRunning = 0;
				min = 0;
				sec = 0;
				tick = 0;
				OLED_ReverseArea(88,44,32,16);
				OLED_Update();
				break;

			//...
			//case StopWatchPageItemNum:
		}
    }
}
