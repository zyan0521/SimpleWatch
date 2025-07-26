#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "OLED.h"
#include "Key.h"

uint8_t SetTimeSele = 1;
extern uint8_t KeyNum;

/*show the settimeUi of first page*/
static void SetTimeFirst_ShowUI(void){
	
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(0,16,OLED_8X16,"年:%4d",MyRTC_Time[0]);
	OLED_Printf(0,32,OLED_8X16,"月:%02d",MyRTC_Time[1]);
	OLED_Printf(0,48,OLED_8X16,"日:%02d",MyRTC_Time[2]);
}

/*show the settimeUi of second page*/
static void SetTimeSec_ShowUI(void){

	OLED_Printf(0,0,OLED_8X16,"时:%02d",MyRTC_Time[3]);
	OLED_Printf(0,16,OLED_8X16,"分:%02d",MyRTC_Time[4]);
	OLED_Printf(0,32,OLED_8X16,"秒:%02d",MyRTC_Time[5]);
}

uint8_t SetTimePage(void) {
	while(1) {
        KeyNum = Key_GetNum();
        if(KeyNum == 1) {
            SetTimeSele--;
			if(SetTimeSele < 1){
				SetTimeSele = 7;
			}
        }
        else if(KeyNum == 2) {
            SetTimeSele++;
			if(SetTimeSele > 7){
				SetTimeSele = 1;
			}
        }
        else if(KeyNum == 3) {
            OLED_Clear();
            OLED_Update();
            return SetTimeSele;
        }
		
		switch(SetTimeSele) {
			case 1:
				OLED_Clear();
				SetTimeFirst_ShowUI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;		
			case 2:
				OLED_Clear();	
				SetTimeFirst_ShowUI();
				OLED_ReverseArea(0,16,16,16);
				OLED_Update();
				break;
			case 3:
				OLED_Clear();
				SetTimeFirst_ShowUI();
				OLED_ReverseArea(0,32,16,16);
				OLED_Update();
				break;
			case 4:
				OLED_Clear();
				SetTimeFirst_ShowUI();
				OLED_ReverseArea(0,48,16,16);
				OLED_Update();
				break;
			
			//start sceond page
			case 5:
				OLED_Clear();
				SetTimeSec_ShowUI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			case 6:
				OLED_Clear();
				SetTimeSec_ShowUI();
				OLED_ReverseArea(0,16,16,16);
				OLED_Update();
				break;
			case 7:
				OLED_Clear();
				SetTimeSec_ShowUI();
				OLED_ReverseArea(0,32,16,16);
				OLED_Update();
				break;		
		}
    }
}
