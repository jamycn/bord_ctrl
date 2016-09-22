/**
 * ��Դ����
 */

#include "stm32f0xx.h"
#include "delay.h"
#include "led.h"
#include "pwm.h"

#define POWER_CTRL_RCC                    RCC_AHBPeriph_GPIOB
#define GPIO_PORT_CTRL                    GPIOB
#define GPIO_PIN_CTRL                     GPIO_Pin_13
#define GPIO_PORT_DELAY                   GPIOB
#define GPIO_PIN_DELAY                    GPIO_Pin_2

/**
 * ��Դ���Ƴ�ʼ��
 */
void power_ctrl_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
     
    RCC_AHBPeriphClockCmd(POWER_CTRL_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CTRL;
    GPIO_Init(GPIO_PORT_CTRL, &GPIO_InitStructure);
    GPIO_ResetBits(GPIO_PORT_CTRL, GPIO_PIN_CTRL);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_DELAY;
    GPIO_Init(GPIO_PORT_DELAY, &GPIO_InitStructure);
    GPIO_ResetBits(GPIO_PORT_DELAY, GPIO_PIN_DELAY);
}

/**
 * �͹�������
 */
void power_ctrl_low_consume_set(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
     
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                                  GPIO_Pin_4 | /*GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | */ \
                                  /*GPIO_Pin_8 | */GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
                                  GPIO_Pin_12 | /*GPIO_Pin_13 | GPIO_Pin_14 | */GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | /*GPIO_Pin_2 | */GPIO_Pin_3 | \
                                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | \
                                  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
                                  /*GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | */GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * ��Դ��������
 */
void power_ctrl_on(void)
{
    /* led��ʾ�Ƴ��� */
    led_power_on();

    /* ctrl���������� */
    GPIO_SetBits(GPIO_PORT_CTRL, GPIO_PIN_CTRL);

    /* �ȴ�3�� */
    delay_ms(3000);

    /* delay�������� */
    GPIO_SetBits(GPIO_PORT_DELAY, GPIO_PIN_DELAY);

    /* led��ʾ����˸ */
    led_power_twinkle_on();

    /* �������飬��ʹ���ϴιرյ���ǰ���� */
    pwm_leds_light_on();
}

/**
 * ��Դ���ƹر�
 */
void power_ctrl_off(void)
{
    /* ctrl���������� */
    GPIO_ResetBits(GPIO_PORT_CTRL, GPIO_PIN_CTRL);

    /* led��ʾ��ֹͣ��˸ */
    led_power_twinkle_off();

    /* led��ʾ�Ƴ��� */
    led_power_on();

    /* �ȴ�500ms */
    delay_ms(500);

    /* delay�������� */
    GPIO_ResetBits(GPIO_PORT_DELAY, GPIO_PIN_DELAY);

    /* ����Ϩ�� */
    pwm_leds_light_off();

    /* led��ʾ��Ϩ�� */
    led_power_off();

    /* �ȴ�500ms */
    delay_ms(500);
}

