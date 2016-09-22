/**
 * 电源控制
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
 * 电源控制初始化
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
 * 低功耗配置
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
 * 电源控制启动
 */
void power_ctrl_on(void)
{
    /* led提示灯常亮 */
    led_power_on();

    /* ctrl引脚先拉高 */
    GPIO_SetBits(GPIO_PORT_CTRL, GPIO_PIN_CTRL);

    /* 等待3秒 */
    delay_ms(3000);

    /* delay引脚拉高 */
    GPIO_SetBits(GPIO_PORT_DELAY, GPIO_PIN_DELAY);

    /* led提示灯闪烁 */
    led_power_twinkle_on();

    /* 开启灯组，并使用上次关闭灯组前亮度 */
    pwm_leds_light_on();
}

/**
 * 电源控制关闭
 */
void power_ctrl_off(void)
{
    /* ctrl引脚先拉低 */
    GPIO_ResetBits(GPIO_PORT_CTRL, GPIO_PIN_CTRL);

    /* led提示灯停止闪烁 */
    led_power_twinkle_off();

    /* led提示灯常亮 */
    led_power_on();

    /* 等待500ms */
    delay_ms(500);

    /* delay引脚拉低 */
    GPIO_ResetBits(GPIO_PORT_DELAY, GPIO_PIN_DELAY);

    /* 灯组熄灭 */
    pwm_leds_light_off();

    /* led提示灯熄灭 */
    led_power_off();

    /* 等待500ms */
    delay_ms(500);
}

