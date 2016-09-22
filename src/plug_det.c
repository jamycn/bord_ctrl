/**
 * 插入检测
 */

#include "stm32f0xx.h"
#include "delay.h"

#define RCC_IO_PLUG_DET             RCC_AHBPeriph_GPIOA
#define GPIO_PORT_PLUG_DET          GPIOA
#define GPIO_PIN_PLUG_DET           GPIO_Pin_6
#define GPIO_PORT_PLUG_TR_A         GPIOA
#define GPIO_PIN_PLUG_TR_A          GPIO_Pin_5

/**
 * 插入检测初始化
 */
void plug_det_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_IO_PLUG_DET, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_PLUG_DET;
    GPIO_Init(GPIO_PORT_PLUG_DET, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_PLUG_TR_A;
    GPIO_Init(GPIO_PORT_PLUG_TR_A, &GPIO_InitStructure);
}

/**
 * 获取插入状态
 * @return
 *     0 -- 未插入
 *     1 -- 插入但未由12V触发
 *     2 -- 插入且已由12V触发
 */
int plug_det_state(void)
{
    if (GPIO_ReadInputDataBit(GPIO_PORT_PLUG_DET, GPIO_PIN_PLUG_DET) > 0) {
        if (GPIO_ReadInputDataBit(GPIO_PORT_PLUG_TR_A, GPIO_PIN_PLUG_TR_A) == 0) {
            delay_ms(50); //去抖，去余电干扰
            if (GPIO_ReadInputDataBit(GPIO_PORT_PLUG_TR_A, GPIO_PIN_PLUG_TR_A) == 0) {
                return 2;
            } else {
                return 1;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

