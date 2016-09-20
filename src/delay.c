
#include "stm32f0xx.h"

#define DELAY_TIMER                   (TIM14)
#define DELAY_TIMER_IRQ               (TIM14_IRQn)
#define DELAY_RCC_TIMER             (RCC_APB1Periph_TIM14)

static int delay_inited = 0;
static int sg_nDelay_ms = 0;

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = DELAY_TIMER_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;//占优先级
          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}

static void Timer_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(DELAY_RCC_TIMER, ENABLE);

    //1ms的定时，即1000Hz的频率, 公式 Frequency=(SYSCLK/(Prescaler+1))/(Period+1)
    TIM_DeInit(DELAY_TIMER);
    TIM_TimeBaseStructure.TIM_Period = 2 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
    TIM_TimeBaseInit(DELAY_TIMER, &TIM_TimeBaseStructure);
    TIM_ClearFlag(DELAY_TIMER, TIM_FLAG_Update); //清除溢出中断标志
    TIM_ITConfig(DELAY_TIMER,TIM_IT_Update,ENABLE); 

    NVIC_Configuration();
    
//    TIM_Cmd(LED_TIMER, ENABLE); //开启时钟
}

/**
 * 精确延迟初始化
 */
static void delay_init(void)
{
    Timer_Configuration();
}

/**
 * 精确延迟(ms级)
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

    TIM_Cmd(DELAY_TIMER, ENABLE);
    
    while (sg_nDelay_ms > 0);
    
    TIM_Cmd(DELAY_TIMER, DISABLE);
} 

void TIM14_IRQHandler(void)
{
    if(TIM_GetITStatus(DELAY_TIMER , TIM_IT_Update) == SET)  
    {  
        TIM_ClearITPendingBit(DELAY_TIMER , TIM_FLAG_Update);

        if (sg_nDelay_ms > 0) {
            sg_nDelay_ms--;
        }
    }
}

