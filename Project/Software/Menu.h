#ifndef __MENU_H
#define __MENU_H

void MenuAPB_Init(void);
uint8_t Get_MenuPage(void);
uint8_t SettingPage(void);
uint8_t SlideMenuPage(void);
uint8_t StopWatchPage(void);
void StopWatch_Tick(void);
uint8_t FlashlightPage(void);
uint8_t IMUPage(void);
void MPU6050_Calculat(void);
uint8_t GamePage(void) ;

#endif
