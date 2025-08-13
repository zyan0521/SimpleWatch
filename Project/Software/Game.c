#include "OLED.h"
#include <stdlib.h>
#include "Delay.h"
#include <math.h>
#include "key.h"
#include "Menu.h"

/******物体位置结构体 ********/
typedef struct {
    uint8_t minX, minY, maxX, maxY;
} ObjectPosition_t;

/*********游戏分数**********/
uint32_t score;
void Show_Score(void) {
	OLED_ShowNum(98, 0, score, 5, OLED_6X8);
}

/*********地面位置position **********/
uint16_t Ground_Pos;
void Show_Ground(void)
{
	if (Ground_Pos < 128)
	{
		for (uint8_t i = 0; i < 128; i++)
		{
			OLED_DisplayBuf[7][i] = Ground[i + Ground_Pos];
		}
	}
	else
	{
		for (uint8_t i = 0; i < 255 - Ground_Pos; i++)
		{
			OLED_DisplayBuf[7][i] = Ground[i + Ground_Pos];
		}
		for (uint8_t i = 255 - Ground_Pos; i < 128; i++)
		{
			OLED_DisplayBuf[7][i] = Ground[i - (255 - Ground_Pos)];
		}
	}
}

/*********障碍物位置position **********/
ObjectPosition_t Barrier_t;
uint8_t Barrier_Flag;
uint8_t Barrier_Pos;
void Show_Barrier(void){
    if(Barrier_Pos >= 143){
        Barrier_Flag = rand() % 3;
    } 
    OLED_ShowImage(127 - Barrier_Pos, 44, 16, 18, Barrier[Barrier_Flag]);

    Barrier_t.minX = 127 - Barrier_Pos;
    Barrier_t.maxX = 139 - Barrier_Pos;
    Barrier_t.minY = 48;
    Barrier_t.maxY = 60;
}

/**********云朵位置position**********/

uint8_t Cloud_Pos1;
uint8_t Cloud_Pos2;
void Show_Cloud(void){
    OLED_ShowImage(127 - Cloud_Pos1, 8, 16, 8, Cloud);
    OLED_ShowImage(127 - Cloud_Pos2, 14, 16, 8, Cloud);
}

/**************小恐龙*****************/
#define  PI             3.1415927f
uint8_t dino_jump_flag = 0; // 0:奔跑 1:跳跃
uint8_t dino_pos = 0;
extern uint8_t KeyNum;
uint8_t jump_t,jump_pos;
ObjectPosition_t dino_t;
void Show_Dino(void){
    KeyNum = Key_GetNum();
    if(KeyNum == 1) dino_jump_flag = 1;
    jump_pos = 28 * sin((float)(PI * jump_t / 100));
    if(dino_jump_flag == 0){
        if(dino_pos % 2 == 0)
        OLED_ShowImage(10, 44, 16, 18, Dino[0]);
        else
        OLED_ShowImage(10, 44, 16, 18, Dino[1]);
    } else {
        OLED_ShowImage(10, 44 - jump_pos, 16, 18, Dino[2]);
    }
    dino_t.minX = 12;
    dino_t.maxX = 24;
    dino_t.minY = 42 - jump_pos;
    dino_t.maxY = 64 - jump_pos;
}


/*****************碰撞检测***************/
extern uint8_t gameTime_Flag;
uint8_t IS_Collision(ObjectPosition_t *Barrier, ObjectPosition_t *dino) {
    if((dino->maxX > Barrier->minX) && (dino->minX < Barrier->maxX) && (dino->maxY > Barrier->minY) && (dino->minY < Barrier->maxY)){
         OLED_Clear();
        OLED_ShowString(28, 24, "Game Over", OLED_8X16);
        OLED_Update();
        Delay_s(1);
        OLED_Clear();
        OLED_Update();
        gameTime_Flag = 0;
        score = 0;
        //游戏结束
        return 1;
    }
    return 0;
}


/*********游戏定时器中断函数 *************/

void GameTick(void) {
    static uint8_t score_cnt, ground_cnt, cloud_cnt, dino_cnt;
    score_cnt++;
    ground_cnt++;
    cloud_cnt++;
    dino_cnt++;

    if (score_cnt >= 10) {
        score++;
        score_cnt = 0;
    }
    if(ground_cnt >= 2){
        Ground_Pos++;
        if(Ground_Pos >= 256)
            Ground_Pos = 0;
        if (gameTime_Flag){
            Barrier_Pos++;
            if (Barrier_Pos >= 144)
                Barrier_Pos = 0;      
        }
        ground_cnt = 0;
    }
    if(cloud_cnt >= 5){
        Cloud_Pos1++;
        if(Cloud_Pos1 >= 200)
            Cloud_Pos1 = 0;
        cloud_cnt = 0;
    }
    if(cloud_cnt >= 4){
        Cloud_Pos2++;
        if(Cloud_Pos2 >= 180)
            Cloud_Pos2 = 0;
    }
    if(dino_cnt >= 8){
        dino_pos++;
        dino_cnt = 0;
    }
    if (dino_jump_flag == 1) {
		jump_t++;
		if (jump_t >= 100) {
			jump_t = 0;
			dino_jump_flag = 0;
		}
	}
}

/*********游戏页面函数 **********/

uint8_t DinoGamePage(void) {
	while(1){
        OLED_Clear();
        Show_Score();
        Show_Ground();
        Show_Barrier();
        Show_Cloud();
        Show_Dino();
        OLED_Update();

        if (IS_Collision(&Barrier_t, &dino_t) == 1) {
            return 0;
            // 游戏结束处理
        }
    }
}
