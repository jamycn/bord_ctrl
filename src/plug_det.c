/**
 * ������
 */

#include "stm32f0xx.h"
#include "delay.h"

#define RCC_IO_PLUG_DET             RCC_AHBPeriph_GPIOA
#define GPIO_PORT_PLUG_DET          GPIOA
#define GPIO_PIN_PLUG_DET           GPIO_Pin_6
#define GPIO_PORT_PLUG_TR_A         GPIOA
#define GPIO_PIN_PLUG_TR_A          GPIO_Pin_5

/**
 * �������ʼ��
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
 * ��ȡ����״̬
 * @return
 *     0 -- δ����
 *     1 -- ���뵫δ��12V����
 *     2 -- ����������12V����
 */
int plug_det_state(void)
{
    if (GPIO_ReadInputDataBit(GPIO_PORT_PLUG_DET, GPIO_PIN_PLUG_DET) > 0) {
        if (GPIO_ReadInputDataBit(GPIO_PORT_PLUG_TR_A, GPIO_PIN_PLUG_TR_A) == 0) {
            delay_ms(50); //ȥ����ȥ������
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

