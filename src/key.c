/**
 * 按键处理
 */

#include "stm32f0xx.h"

#define RCC_IO_KEY                      RCC_AHBPeriph_GPIOA
#define GPIO_PORT_KEY_BOOT              GPIOA
#define GPIO_PIN_KEY_BOOT               GPIO_Pin_8
#define GPIO_PORT_KEY_TCH               GPIOA
#define GPIO_PIN_KEY_TCH                GPIO_Pin_7

/**
 * 按键初始化
 */
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
     
    RCC_AHBPeriphClockCmd(RCC_IO_KEY, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY_BOOT;
    GPIO_Init(GPIO_PORT_KEY_BOOT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY_TCH;
    GPIO_Init(GPIO_PORT_KEY_TCH, &GPIO_InitStructure);
}

/**
 * 获取电源键状态
 * @return
 *     1 -- 激活
 *     0 -- 未激活
 */
int key_power_state(void)
{
    if (GPIO_ReadInputDataBit(GPIO_PORT_KEY_BOOT, GPIO_PIN_KEY_BOOT) == 0) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * 获取亮度调节键状态
 * @return
 *     1 -- 激活
 *     0 -- 未激活
 */
int key_light_step_state(void)
{
    if (GPIO_ReadInputDataBit(GPIO_PORT_KEY_TCH, GPIO_PIN_KEY_TCH) == 0) {
        return 1;
    } else {
        return 0;
    }
}

