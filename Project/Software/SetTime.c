#include "stm32f10x.h" // Device header
#include "MyRTC.h"
#include "OLED.h"
#include "Key.h"

uint8_t SetTimeSele = 1;
uint8_t SetTimeSeleTemp;
extern uint8_t KeyNum;

/*show the settimeUi of first page*/
static void SetTimeFirst_ShowUI(void)
{

	OLED_ShowImage(0, 0, 16, 16, Return);
	OLED_Printf(0, 16, OLED_8X16, "年:%4d", MyRTC_Time[0]);
	OLED_Printf(0, 32, OLED_8X16, "月:%02d", MyRTC_Time[1]);
	OLED_Printf(0, 48, OLED_8X16, "日:%02d", MyRTC_Time[2]);
}

/*show the settimeUi of second page*/
static void SetTimeSec_ShowUI(void)
{

	OLED_Printf(0, 0, OLED_8X16, "时:%02d", MyRTC_Time[3]);
	OLED_Printf(0, 16, OLED_8X16, "分:%02d", MyRTC_Time[4]);
	OLED_Printf(0, 32, OLED_8X16, "秒:%02d", MyRTC_Time[5]);
}

static void Change_RTCTime(uint8_t RTC_Index,uint8_t flag)
{
	if(flag == 1)
	{
		MyRTC_Time[RTC_Index]++;
	}
	else if(flag == 0)
	{
		MyRTC_Time[RTC_Index]--;
	}
	MyRTC_SetTime();
}

static uint8_t SetYear(void){
	while(1){
		KeyNum = Key_GetNum();
		if(KeyNum == 1){
			Change_RTCTime(0,1);
		}
		else if(KeyNum == 2){
			Change_RTCTime(0,0);
		}
		else if(KeyNum == 3){
			return 0;
		}
		SetTimeFirst_ShowUI();
		OLED_ReverseArea(24,16,32,16);
		OLED_Update();
	}
}

static uint8_t SetMonth(void){
	while(1){
		KeyNum = Key_GetNum();
		if(KeyNum == 1){
			Change_RTCTime(1,1);
			if(MyRTC_Time[1] > 12) MyRTC_Time[1] = 1; // wrap around
			MyRTC_SetTime();
		}
		else if(KeyNum == 2){
			Change_RTCTime(1,0);
			if(MyRTC_Time[1] < 1) MyRTC_Time[1] = 12; // wrap around
			MyRTC_SetTime();
		}
		else if(KeyNum == 3){
			return 0;
		}
		SetTimeFirst_ShowUI();
		OLED_ReverseArea(24,32,16,16);
		OLED_Update();
	}
}

static uint8_t SetDay(void){
	while(1){
		KeyNum = Key_GetNum();
		if(KeyNum == 1){
			Change_RTCTime(2,1);
			if(MyRTC_Time[2] > 31) MyRTC_Time[2] = 1; // wrap around
			MyRTC_SetTime();
		}
		else if(KeyNum == 2){
			Change_RTCTime(2,0);
			if(MyRTC_Time[2] < 1) MyRTC_Time[2] = 31; // wrap around
			MyRTC_SetTime();
		}
		else if(KeyNum == 3){
			return 0;
		}
		SetTimeFirst_ShowUI();
		OLED_ReverseArea(24,48,16,16);
		OLED_Update();
	}
}

static uint8_t SetHour(void){
	while(1){
		KeyNum = Key_GetNum();
		if(KeyNum == 1){
			Change_RTCTime(3,1);
			if(MyRTC_Time[3] > 23) MyRTC_Time[3] = 0; // wrap around
			MyRTC_SetTime();
		}
		else if(KeyNum == 2){
			Change_RTCTime(3,0);
			if(MyRTC_Time[3] < 0) MyRTC_Time[3] = 23; // wrap around
			MyRTC_SetTime();
		}
		else if(KeyNum == 3){
			return 0;
		}
		SetTimeSec_ShowUI();
		OLED_ReverseArea(24,0,16,16);
		OLED_Update();
	}
}

static uint8_t SetMinute(void){
	while(1){
		KeyNum = Key_GetNum();
		if(KeyNum == 1){
			Change_RTCTime(4,1);
			if(MyRTC_Time[4] > 59) MyRTC_Time[4] = 0; // wrap around
			MyRTC_SetTime();
		}
		else if(KeyNum == 2){
			Change_RTCTime(4,0);
			if(MyRTC_Time[4] < 0) MyRTC_Time[4] = 59; // wrap around
			MyRTC_SetTime();
		}
		else if(KeyNum == 3){
			return 0;
		}
		SetTimeSec_ShowUI();
		OLED_ReverseArea(24,16,16,16);
		OLED_Update();
	}
}

static uint8_t SetSecond(void){
	while(1){
		KeyNum = Key_GetNum();
		if(KeyNum == 1){
			Change_RTCTime(5,1);
			if(MyRTC_Time[5] > 59) MyRTC_Time[5] = 0; // wrap around
			MyRTC_SetTime();
		}
		else if(KeyNum == 2){
			Change_RTCTime(5,0);
			if(MyRTC_Time[5] < 0) MyRTC_Time[5] = 59; // wrap around
			MyRTC_SetTime();
		}
		else if(KeyNum == 3){
			return 0;
		}
		SetTimeSec_ShowUI();
		OLED_ReverseArea(24,32,16,16);
		OLED_Update();
	}
}

uint8_t SetTimePage(void)
{
	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			SetTimeSele--;
			if (SetTimeSele < 1)
			{
				SetTimeSele = 7;
			}
		}
		else if (KeyNum == 2)
		{
			SetTimeSele++;
			if (SetTimeSele > 7)
			{
				SetTimeSele = 1;
			}
		}
		else if (KeyNum == 3)
		{
			OLED_Clear();
			OLED_Update();
			SetTimeSeleTemp = SetTimeSele;
			if (SetTimeSeleTemp == 1)
				return 0;
			else if (SetTimeSeleTemp == 2){SetYear();}// years
			else if (SetTimeSeleTemp == 3){SetMonth();}// month
			else if (SetTimeSeleTemp == 4){SetDay();}// day
			else if (SetTimeSeleTemp == 5){SetHour();}// hour
			else if (SetTimeSeleTemp == 6){SetMinute();}// minute
			else if (SetTimeSeleTemp == 7){SetSecond();}// second
		}

		switch (SetTimeSele)
		{
		case 1:
			OLED_Clear();
			SetTimeFirst_ShowUI();
			OLED_ReverseArea(0, 0, 16, 16);
			OLED_Update();
			break;
		case 2:
			OLED_Clear();
			SetTimeFirst_ShowUI();
			OLED_ReverseArea(0, 16, 16, 16);
			OLED_Update();
			break;
		case 3:
			OLED_Clear();
			SetTimeFirst_ShowUI();
			OLED_ReverseArea(0, 32, 16, 16);
			OLED_Update();
			break;
		case 4:
			OLED_Clear();
			SetTimeFirst_ShowUI();
			OLED_ReverseArea(0, 48, 16, 16);
			OLED_Update();
			break;

		// start sceond page
		case 5:
			OLED_Clear();
			SetTimeSec_ShowUI();
			OLED_ReverseArea(0, 0, 16, 16);
			OLED_Update();
			break;
		case 6:
			OLED_Clear();
			SetTimeSec_ShowUI();
			OLED_ReverseArea(0, 16, 16, 16);
			OLED_Update();
			break;
		case 7:
			OLED_Clear();
			SetTimeSec_ShowUI();
			OLED_ReverseArea(0, 32, 16, 16);
			OLED_Update();
			break;
		}
	}
}
