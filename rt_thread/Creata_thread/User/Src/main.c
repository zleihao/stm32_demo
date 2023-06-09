/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/
#include "board.h"
#include "rtthread.h"


/*
*************************************************************************
*                               变量
*************************************************************************
*/
/* 定义线程控栈时要求RT_ALIGN_SIZE个字节对齐 */
/* 定义线程栈 */
ALIGN(RT_ALIGN_SIZE) static uint8_t led_thread_stack[1024];

/* 定义线程控制块 */
static struct rt_thread led_thread;

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void led_thread_entry(void *parameter);

/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
/**
* @brief  主函数
* @param  无
* @retval 无
*/
int main(void)
{
    /*
    * 开发板硬件初始化，RTT系统初始化已经在main函数之前完成，
    * 即在component.c文件中的rtthread_startup()函数中完成了。
    * 所以在main函数中，只需要创建线程和启动线程即可。
    */
    rt_thread_init(&led_thread,
                   "led_thread",
                   led_thread_entry,
                   RT_NULL,
                   &led_thread_stack,
                   1024,
                   3,
                   20);

    /* 启动线程，开启调度 */
    rt_thread_startup(&led_thread);
}

/*
*************************************************************************
*                             线程定义
*************************************************************************
*/
static void led_thread_entry(void *parameter)
{
    /* 线程是一个死循环 */
    while (1) {
        LED_G(ON);
        rt_thread_delay(500);

        LED_G(OFF);
        rt_thread_delay(500);
    }
}

/*******************************END OF FILE****************************/