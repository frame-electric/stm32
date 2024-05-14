/*
* Copyright (c) 2006-2021, RT-Thread Development Team
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date           Author       Notes
* 2018-11-06     SummerGift   first version
* 2018-11-19     flybreak     add stm32f407-atk-explorer bsp
*/

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h> 
#include "oled.h"
#include "bmp.h"
//#include "touch.h" 
#include "lcd.h"
/* defined the LED pin*/
#define LED0_PIN    GET_PIN(F, 9)
#define LED1_PIN    GET_PIN(F,10)

/* define the BEEP pin*/
#define BEEP_PIN    GET_PIN(F, 8)

/*define the key pin*/
#define KEY_0_PIN   GET_PIN(E, 4)
#define KEY_1_PIN   GET_PIN(E, 3)
#define KEY_2_PIN   GET_PIN(E, 2)
#define KEY_UP_PIN  GET_PIN(A, 0)

/* 定义操作 */
#define BEEP_ON()  rt_pin_write(BEEP_PIN, PIN_HIGH)
#define BEEP_OFF() rt_pin_write(BEEP_PIN, PIN_LOW)

#define LED0_ON()  rt_pin_write(LED0_PIN, PIN_HIGH)
#define LED0_OFF() rt_pin_write(LED0_PIN, PIN_LOW)
#define LED1_ON()  rt_pin_write(LED1_PIN, PIN_HIGH)
#define LED1_OFF() rt_pin_write(LED1_PIN, PIN_LOW)

/* 临时定义OLED显示 */
/* 定义IIC引脚*/



void hardware_init(void)
{
    /* led 初始化*/
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);

    /* 按键初始化*/
    rt_pin_mode(KEY_0_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_1_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_2_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_UP_PIN, PIN_MODE_INPUT_PULLDOWN);

    /* 蜂鸣器初始化*/
    rt_pin_mode(BEEP_PIN, PIN_MODE_OUTPUT);

    /* LED 初始化*/
    OLED_Init(); 

}

rt_thread_t OLED_thread;
void OLED_entry(void *p)
{
    u32 sel = 0;
    u8 refresh = 0;
    while(1) 
    {		
        if(PIN_LOW == rt_pin_read(KEY_0_PIN))
        {
            rt_thread_mdelay(40);//延时去抖
            if(PIN_LOW == rt_pin_read(KEY_0_PIN))
            {
                sel++;
                refresh = 1;
            }
        }
        if(sel & 0x01 && refresh)
        {
            OLED_Clear();
            refresh = 0;
            OLED_ShowCHinese(18,0,0);//电
            OLED_ShowCHinese(36,0,1);//子
            OLED_ShowCHinese(72,0,6);//科
            OLED_ShowCHinese(90,0,7);//技
        }
        else if(refresh)
        {
            OLED_Clear();
            refresh = 0;
            OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
        }
    }	
}

int main(void)
{
    hardware_init();
    OLED_Clear();
    OLED_thread = rt_thread_create("OLED_work", OLED_entry, RT_NULL, 512, 15, 5);
    if(OLED_thread != RT_NULL)
    {
        rt_thread_startup(OLED_thread);
    }
    else
    {
        rt_kprintf("create OLED_work thread failed\n");
    }
    rt_kprintf("start_running\r\n");
}
