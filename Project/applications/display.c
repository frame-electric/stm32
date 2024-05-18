#include "display.h"
#include "oled.h"
#include "bmp.h"
#include "multi_button.h"

static rt_thread_t OLED_thread;
static rt_thread_t KEY_thread;
static int8_t index = 0;
static int8_t str_index = 0;

uint8_t string_list[][30] =
{
    "hello world",
    "what about going to walk?",
    "hello python"
};

/*define the key pin*/
#define KEY_RIGHT_PIN   GET_PIN(E, 4)
#define KEY_DOWN_PIN    GET_PIN(E, 3)
#define KEY_LEFT_PIN    GET_PIN(E, 2)
#define KEY_UP_PIN      GET_PIN(A, 0)

uint8_t Hzk_num = 14;

static struct button btn_left;
static struct button btn_right;
static struct button btn_up;
static struct button btn_down;

static uint8_t button_read_up_pin(void)
{
    return rt_pin_read(KEY_UP_PIN);

}
static uint8_t button_read_left_pin(void)
{
    return rt_pin_read(KEY_LEFT_PIN);
}

static uint8_t button_read_down_pin(void)
{
    return rt_pin_read(KEY_DOWN_PIN);
}

static uint8_t button_read_right_pin(void)
{
    return rt_pin_read(KEY_RIGHT_PIN);
}

void button_left_callback(void *btn)
{
    uint32_t btn_event_val;

    btn_event_val = get_button_event((struct button *)btn);

    switch(btn_event_val)
    {
        case SINGLE_CLICK:
            index -= 1;
            str_index -= 1;
        break;

        case DOUBLE_CLICK:
            rt_kprintf("button left double click\n");
        break;

        case LONG_PRESS_START:
            rt_kprintf("button left long press start\n");
        break;
    }
}

void button_right_callback(void *btn)
{
    uint32_t btn_event_val;

    btn_event_val = get_button_event((struct button *)btn);

    switch(btn_event_val)
    {
        case SINGLE_CLICK:
            index += 1;
            str_index += 1;
        break;

        case DOUBLE_CLICK:
            rt_kprintf("button right double click\n");
        break;

        case LONG_PRESS_START:
            rt_kprintf("button right long press start\n");
        break;
    }
}

void button_down_callback(void *btn)
{
    uint32_t btn_event_val;

    btn_event_val = get_button_event((struct button *)btn);

    switch(btn_event_val)
    {
        case SINGLE_CLICK:
            index += 1;
            str_index += 1;
        break;

        case DOUBLE_CLICK:
            rt_kprintf("button down double click\n");
        break;

        case LONG_PRESS_START:
            rt_kprintf("button down long press start\n");
        break;
    }
}

void button_up_callback(void *btn)
{
    uint32_t btn_event_val;

    btn_event_val = get_button_event((struct button *)btn);

    switch(btn_event_val)
    {
        case SINGLE_CLICK:
            index -= 1;
            str_index -= 1;
        break;

        case DOUBLE_CLICK:
            rt_kprintf("button up double click\n");
        break;

        case LONG_PRESS_START:
            rt_kprintf("button up long press start\n");
        break;
    }
}

void btn_thread_entry(void* p)
{
    while(1)
    {
        /* 5ms */
        rt_thread_delay(RT_TICK_PER_SECOND/200);
        button_ticks();
    }
}

void OLED_entry(void *p)
{
    rt_kprintf("oled_entry running\n");
    while(1) 
    {
        if(index < 0) index = Hzk_num - 1;
        if(index >= Hzk_num) index = 0;

        if(str_index < 0) str_index = 2;
        if(str_index >= 3) str_index = 0;

        OLED_ShowCHinese(0,0,index);
        OLED_ShowString(17, 0, string_list[str_index], strlen((char *)string_list[str_index])); 
        // if(PIN_LOW == rt_pin_read(KEY_0_PIN))
        // {
        //     rt_thread_delay(30);
        //     if(PIN_LOW == rt_pin_read(KEY_0_PIN))
        //     {
        //         sel++;
        //         refresh = 1;
        //     }
        // }
        // if(sel & 0x01 && refresh)
        // {
        //     OLED_Clear();
        //     refresh = 0;
        //     OLED_ShowCHinese(18*0,0,0);//电
        //     OLED_ShowCHinese(18*1,0,1);//子
        //     OLED_ShowCHinese(18*2,0,6);//技
        //     OLED_ShowCHinese(18*3,0,7);//术
        //     OLED_ShowCHinese(18*4,0,11);//马
        //     OLED_ShowCHinese(18*5,0,12);//执
        //     OLED_ShowCHinese(18*6,0,13);//鹏
        // }
        // else if(refresh)
        // {
        //     OLED_Clear();
        //     refresh = 0;
        //     OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
        // }
    }	
}

void display_init(void)
{
    // OLED初始化
	OLED_Init();
	OLED_Clear();

    /* 按键初始化*/
    rt_pin_mode(KEY_RIGHT_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_DOWN_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_LEFT_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_UP_PIN, PIN_MODE_INPUT_PULLDOWN);

    // 创建按键线程
    KEY_thread = rt_thread_create("KEY_work", btn_thread_entry, RT_NULL, 1024, 10, 10);
    if(KEY_thread != RT_NULL)
    {
        rt_thread_startup(KEY_thread);
    }
    else
    {
        rt_kprintf("create KEY_work thread failed\n");
    }

    button_init(&btn_left , button_read_left_pin , PIN_LOW);
    button_init(&btn_right, button_read_right_pin, PIN_LOW);
    button_init(&btn_down , button_read_down_pin , PIN_LOW);
    button_init(&btn_up   , button_read_up_pin   , PIN_HIGH);

    button_attach(&btn_left, SINGLE_CLICK,     button_left_callback);
    button_attach(&btn_left, DOUBLE_CLICK,     button_left_callback);
    button_attach(&btn_left, LONG_PRESS_START, button_left_callback);
    button_start (&btn_left);

    button_attach(&btn_right, SINGLE_CLICK,     button_right_callback);
    button_attach(&btn_right, DOUBLE_CLICK,     button_right_callback);
    button_attach(&btn_right, LONG_PRESS_START, button_right_callback);
    button_start (&btn_right);

    button_attach(&btn_down, SINGLE_CLICK,     button_down_callback);
    button_attach(&btn_down, DOUBLE_CLICK,     button_down_callback);
    button_attach(&btn_down, LONG_PRESS_START, button_down_callback);
    button_start (&btn_down);

    button_attach(&btn_up, SINGLE_CLICK,     button_up_callback);
    button_attach(&btn_up, DOUBLE_CLICK,     button_up_callback);
    button_attach(&btn_up, LONG_PRESS_START, button_up_callback);
    button_start (&btn_up);

    // 创建显示线程
    OLED_thread = rt_thread_create("OLED_work", OLED_entry, RT_NULL, 512, 15, 5);
    if(OLED_thread != RT_NULL)
    {
        rt_thread_startup(OLED_thread);
    }
    else
    {
        rt_kprintf("create OLED_work thread failed\n");
    }

}

const struct display_task_t display_task = 
{
    .init = display_init,
};
