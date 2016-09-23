/**
 * led����
 */

#include "stm32f0xx.h"

#define RCC_IO_LEDS                 RCC_AHBPeriph_GPIOB
#define GPIO_PORT_LED1              GPIOB
#define GPIO_PIN_LED1               GPIO_Pin_14
#define TIMER_LED                   TIM3
#define TIMER_IRQ_LED               TIM3_IRQn
#define RCC_TIMER_LED               RCC_APB1Periph_TIM3

static int sg_nIsInited = 0;

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQ_LED; //ͨ��LED_TIMER_IRQ 
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;//ռ���ȼ�
          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}

static void Timer_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_TIMER_LED, ENABLE);

    //0.5S�Ķ�ʱ����2Hz��Ƶ��, ��ʽ Frequency=(SYSCLK/(Prescaler+1))/(Period+1)
    TIM_DeInit(TIMER_LED);
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 24000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
    TIM_TimeBaseInit(TIMER_LED, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIMER_LED, TIM_FLAG_Update); //�������жϱ�־
    TIM_ITConfig(TIMER_LED,TIM_IT_Update,ENABLE); 

    NVIC_Configuration();

    RCC_APB1PeriphClockCmd(RCC_TIMER_LED, DISABLE);
}

/**
 * led��ʼ��
 */
void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_IO_LEDS, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_LED1;
    GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);
    GPIO_ResetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);

    sg_nIsInited = 1;

    Timer_Configuration();
}

/**
 * ������Դ��
 */
void led_power_on(void)
{
    if (!sg_nIsInited)
    {
        led_init();
    }

    GPIO_SetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);
}

/**
 * Ϩ���Դ��
 */
void led_power_off(void)
{
    if (!sg_nIsInited)
    {
        led_init();
    }

    GPIO_ResetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);
}

/**
 * ��ʼ��Դ����˸
 */
void led_power_twinkle_on(void)
{
    if (!sg_nIsInited)
    {
        led_init();
    }

    led_power_on();
    RCC_APB1PeriphClockCmd(RCC_TIMER_LED, ENABLE);
    TIM_Cmd(TIMER_LED, ENABLE); //����ʱ��
}

/**
 * ֹͣ��Դ����˸
 */
void led_power_twinkle_off(void)
{
    if (sg_nIsInited)
    {
        TIM_Cmd(TIMER_LED, DISABLE); //�ر�ʱ��
        RCC_APB1PeriphClockCmd(RCC_TIMER_LED, DISABLE);
        led_power_off();
    }
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIMER_LED , TIM_IT_Update) == SET)  
    {  
        TIM_ClearITPendingBit(TIMER_LED , TIM_FLAG_Update);

        if (GPIO_ReadOutputDataBit(GPIO_PORT_LED1, GPIO_PIN_LED1) > 0)
        {
            led_power_off();
        }
        else
        {
            led_power_on();
        }
    }
}

