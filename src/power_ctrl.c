
#include "stm32f0xx.h"
#include "led.h"
#include "delay.h"

#define POWER_CTRL_RCC                    (RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB)
#define BOOT_CHECK_GPIO_PORT              GPIOA
#define BOOT_CHECK_GPIO_PIN               GPIO_Pin_8
#define CTRL_GPIO_PORT                    GPIOB
#define CTRL_GPIO_PIN                     GPIO_Pin_13
#define DELAY_GPIO_PORT                   GPIOB
#define DELAY_GPIO_PIN                    GPIO_Pin_2

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

    GPIO_InitStructure.GPIO_Pin = CTRL_GPIO_PIN;
    GPIO_Init(CTRL_GPIO_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(CTRL_GPIO_PORT, CTRL_GPIO_PIN);

    GPIO_InitStructure.GPIO_Pin = DELAY_GPIO_PIN;
    GPIO_Init(DELAY_GPIO_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(DELAY_GPIO_PORT, DELAY_GPIO_PIN);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin = BOOT_CHECK_GPIO_PIN;
    GPIO_Init(BOOT_CHECK_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * ��ȡboot������״̬
 * @return
 *     1 -- ����
 *     0 -- δ����
 */
int power_ctrl_is_boot_key_active(void)
{
    if (GPIO_ReadInputDataBit(BOOT_CHECK_GPIO_PORT, BOOT_CHECK_GPIO_PIN) == 0) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * ����ϵͳ����
 */
void power_ctrl_boot_on(void)
{
    /* led��ʾ�Ƴ��� */
    led_on();

    /* ctrl���������� */
    GPIO_SetBits(CTRL_GPIO_PORT, CTRL_GPIO_PIN);

    /* �ȴ�3�� */
    delay_ms(3000);

    /* delay�������� */
    GPIO_SetBits(DELAY_GPIO_PORT, DELAY_GPIO_PIN);

    /* led��ʾ����˸ */
    led_twinkle_on();
}

