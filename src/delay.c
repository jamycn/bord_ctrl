/**
 * �ӳ�(���뼶)
 */

#include "stm32f0xx.h"

#define TIMER_DELAY                 TIM14
#define TIMER_IRQ_DELAY             TIM14_IRQn
#define RCC_TIMER_DELAY             RCC_APB1Periph_TIM14

static int delay_inited = 0;
static int sg_nDelay_ms = 0;

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQ_DELAY;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;//ռ���ȼ�
          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}

static void Timer_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_TIMER_DELAY, ENABLE);

    //1ms�Ķ�ʱ����1000Hz��Ƶ��, ��ʽ Frequency=(SYSCLK/(Prescaler+1))/(Period+1)
    TIM_DeInit(TIMER_DELAY);
    TIM_TimeBaseStructure.TIM_Period = 2 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 24000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
    TIM_TimeBaseInit(TIMER_DELAY, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIMER_DELAY, TIM_FLAG_Update); //�������жϱ�־
    TIM_ITConfig(TIMER_DELAY,TIM_IT_Update,ENABLE); 

    NVIC_Configuration();
    
//    TIM_Cmd(LED_TIMER, ENABLE); //����ʱ��
}

/**
 * ��ȷ�ӳٳ�ʼ��
 */
static void delay_init(void)
{
    Timer_Configuration();
}

/**
 * ��ȷ�ӳ�(ms��)
 * @param 
 */
void delay_ms(int nDelay_ms)
{
    if (!delay_inited)
    {
        delay_init();
        delay_inited = 1;
    }

    sg_nDelay_ms = nDelay_ms;

    TIM_Cmd(TIMER_DELAY, ENABLE);
    
    while (sg_nDelay_ms > 0);
    
    TIM_Cmd(TIMER_DELAY, DISABLE);
} 

void TIM14_IRQHandler(void)
{
    if(TIM_GetITStatus(TIMER_DELAY , TIM_IT_Update) == SET)  
    {  
        TIM_ClearITPendingBit(TIMER_DELAY , TIM_FLAG_Update);

        if (sg_nDelay_ms > 0) {
            sg_nDelay_ms--;
        }
    }
}

