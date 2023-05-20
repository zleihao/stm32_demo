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
//创建一个消息句柄
static rt_mq_t test_mq = RT_NULL;

//创建一个接收、发送线程句柄
static rt_thread_t send_thread = RT_NULL;
static rt_thread_t receive_thread = RT_NULL;

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void receive_thread_entry(void* paramer);
static void send_thread_entry(void* paramer);

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
   
    //创建消息
    test_mq = rt_mq_create("test_mq",
                            40,
                            20,
                            RT_IPC_FLAG_FIFO);

    if (test_mq != RT_NULL) {
        rt_kprintf("消息队列创建成功！\n\n");
    }

    //创建任务
    receive_thread = rt_thread_create("receive_thread",
                                       receive_thread_entry,
                                       RT_NULL,
                                       512,
                                       3,
                                       20);
    if (receive_thread != RT_NULL) {
        rt_thread_startup(receive_thread);
    }

    send_thread = rt_thread_create("send_thread",
                                       send_thread_entry,
                                       RT_NULL,
                                       512,
                                       3,
                                       20);
    if (send_thread != RT_NULL) {
        rt_thread_startup(send_thread);
    }
}

/*
*************************************************************************
*                             线程定义
*************************************************************************
*/
//接收消息
static void receive_thread_entry(void* paramer)
{
    rt_err_t uwRet = RT_NULL;
    uint32_t r_queue;

    while (1) {
        uwRet = rt_mq_recv(test_mq,
                           &r_queue, 
                           sizeof(r_queue),
                           RT_WAITING_FOREVER);

        if (uwRet == RT_EOK) {
            rt_kprintf("接收到的消息为:%ld\n", r_queue);
        } else {
            rt_kprintf("数据接收出错,错误代码: 0x%lx\n",uwRet);
        }
        rt_thread_delay(200);
    }

}

static void send_thread_entry(void* paramer)
{
    rt_err_t uwRet = RT_NULL;
    uint32_t send_data1 = 1;
    uint32_t send_data2 = 2;

    while (1) {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON) {
            uwRet = rt_mq_send(test_mq,
                                &send_data1,
                                sizeof(send_data1));

            if (uwRet != RT_EOK) {
                rt_kprintf("数据不能发送到消息队列！错误代码: %lx\n", uwRet);
            }
        }
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON) {
            uwRet = rt_mq_send(test_mq,
                                &send_data2,
                                sizeof(send_data2));
                            
            if (uwRet != RT_EOK) {
                rt_kprintf("数据不能发送到消息队列！错误代码: %lx\n", uwRet);
            }
        }

        rt_thread_delay(20);
    }
}

/*******************************END OF FILE****************************/