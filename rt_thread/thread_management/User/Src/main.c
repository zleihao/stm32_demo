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
/* 定义线程控制块 */
static rt_thread_t led_thread = RT_NULL;
static rt_thread_t key_thread = RT_NULL;

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void led_thread_entry(void *parameter);

static void key_thread_entry(void *parameter);
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
    led_thread = rt_thread_create("led_thread",
                                led_thread_entry,
                                RT_NULL,
                                1024,
                                3,
                                20);

    /* 启动线程，开启调度 */
    if (led_thread != RT_NULL)
        rt_thread_startup(led_thread);
    else
        return -1;

    key_thread =                          /* 线程控制块指针 */
        rt_thread_create("key",              /* 线程名字 */
                        key_thread_entry,   /* 线程入口函数 */
                        RT_NULL,             /* 线程入口函数参数 */
                        512,                 /* 线程栈大小 */
                        5,                   /* 线程的优先级 */
                        20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (key_thread != RT_NULL)
        rt_thread_startup(key_thread);
    else
        return -1;
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

//按键线程
static void key_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    while (1) {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON) {

            rt_kprintf("挂起led灯线程\n");
            //如果按键按下，挂起led线程
            uwRet = rt_thread_suspend(led_thread);

            if (RT_EOK == uwRet) {
                rt_kprintf("挂起LED1线程成功！\n");
            } else {
                rt_kprintf("挂起LED1线程失败！失败代码：0x%lx\n", uwRet);
            }
        }
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON) {
            rt_kprintf("恢复led灯线程\n");
            uwRet = rt_thread_resume(led_thread);
            if (RT_EOK == uwRet) {
                rt_kprintf("恢复LED1线程成功！\n");
            } else {
                rt_kprintf("恢复LED1线程失败！失败代码：0x%lx\n", uwRet);
            }
        }

        rt_thread_delay(50);
    }
}

/*******************************END OF FILE****************************/