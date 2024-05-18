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
#include "display.h"
#include "oled.h"

/* defined the LED pin*/
#define LED0_PIN    GET_PIN(F, 9)
#define LED1_PIN    GET_PIN(F,10)

/* define the BEEP pin*/
#define BEEP_PIN    GET_PIN(F, 8)

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

    /* 蜂鸣器初始化*/
    rt_pin_mode(BEEP_PIN, PIN_MODE_OUTPUT);

    /* LED 初始化*/
    OLED_Init(); 

}



int main(void)
{
    hardware_init();
    display_task.init();
    rt_kprintf("start_running\r\n");
    while (1)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
