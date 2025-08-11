/*
 * @Author: Jack 202205710405@smail.xtu.edu.cn
 * @Date: 2025-08-11 18:24:55
 * @LastEditors: Jack 202205710405@smail.xtu.edu.cn
 * @LastEditTime: 2025-08-11 18:37:55
 * @FilePath: \Project\Hardware\LED.c
 * @Description: 
 * 
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved. 
 */
#define FlashlightPort		GPIOC
#define FlashlightPin		GPIO_Pin_13

#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = FlashlightPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FlashlightPort, &GPIO_InitStructure);
	
	GPIO_SetBits(FlashlightPort, FlashlightPin);
}

void LED_ON(void)
{
	GPIO_ResetBits(FlashlightPort, FlashlightPin);
}

void LED_OFF(void)
{
	GPIO_SetBits(FlashlightPort, FlashlightPin);
}

