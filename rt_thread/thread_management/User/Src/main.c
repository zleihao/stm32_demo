/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/
#include "board.h"
#include "rtthread.h"


/*
*************************************************************************
*                               ����
*************************************************************************
*/
/* �����߳̿��ƿ� */
static rt_thread_t led_thread = RT_NULL;
static rt_thread_t key_thread = RT_NULL;

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void led_thread_entry(void *parameter);

static void key_thread_entry(void *parameter);
/*
*************************************************************************
*                             main ����
*************************************************************************
*/
/**
* @brief  ������
* @param  ��
* @retval ��
*/
int main(void)
{
    /*
    * ������Ӳ����ʼ����RTTϵͳ��ʼ���Ѿ���main����֮ǰ��ɣ�
    * ����component.c�ļ��е�rtthread_startup()����������ˡ�
    * ������main�����У�ֻ��Ҫ�����̺߳������̼߳��ɡ�
    */
    led_thread = rt_thread_create("led_thread",
                                led_thread_entry,
                                RT_NULL,
                                1024,
                                3,
                                20);

    /* �����̣߳��������� */
    if (led_thread != RT_NULL)
        rt_thread_startup(led_thread);
    else
        return -1;

    key_thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create("key",              /* �߳����� */
                        key_thread_entry,   /* �߳���ں��� */
                        RT_NULL,             /* �߳���ں������� */
                        512,                 /* �߳�ջ��С */
                        5,                   /* �̵߳����ȼ� */
                        20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (key_thread != RT_NULL)
        rt_thread_startup(key_thread);
    else
        return -1;
}

/*
*************************************************************************
*                             �̶߳���
*************************************************************************
*/
static void led_thread_entry(void *parameter)
{
    /* �߳���һ����ѭ�� */
    while (1) {
        LED_G(ON);
        rt_thread_delay(500);

        LED_G(OFF);
        rt_thread_delay(500);
    }
}

//�����߳�
static void key_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    while (1) {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON) {

            rt_kprintf("����led���߳�\n");
            //����������£�����led�߳�
            uwRet = rt_thread_suspend(led_thread);

            if (RT_EOK == uwRet) {
                rt_kprintf("����LED1�̳߳ɹ���\n");
            } else {
                rt_kprintf("����LED1�߳�ʧ�ܣ�ʧ�ܴ��룺0x%lx\n", uwRet);
            }
        }
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON) {
            rt_kprintf("�ָ�led���߳�\n");
            uwRet = rt_thread_resume(led_thread);
            if (RT_EOK == uwRet) {
                rt_kprintf("�ָ�LED1�̳߳ɹ���\n");
            } else {
                rt_kprintf("�ָ�LED1�߳�ʧ�ܣ�ʧ�ܴ��룺0x%lx\n", uwRet);
            }
        }

        rt_thread_delay(50);
    }
}

/*******************************END OF FILE****************************/