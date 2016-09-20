
#include "stm32f0xx.h"

#define LED1_RCC                    RCC_AHBPeriph_GPIOB
#define LED1_GPIO_PORT              (GPIOB)
#define LED1_GPIO_PIN               (GPIO_Pin_14)
#define LED_TIMER                   (TIM3)
#define LED_TIMER_IRQ               (TIM3_IRQn)
#define RCC_LED_TIMER               (RCC_APB1Periph_TIM3)

static int led_inited = 0;

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = LED_TIMER_IRQ; //通道LED_TIMER_IRQ 
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;//占优先级
          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}

static void Timer_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_LED_TIMER, ENABLE);

    //0.5S的定时，即2Hz的频率, 公式 Frequency=(SYSCLK/(Prescaler+1))/(Period+1)
    TIM_DeInit(LED_TIMER);
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
    TIM_TimeBaseInit(LED_TIMER, &TIM_TimeBaseStructure);
    TIM_ClearFlag(LED_TIMER, TIM_FLAG_Update); //清除溢出中断标志
    TIM_ITConfig(LED_TIMER,TIM_IT_Update,ENABLE); 

    NVIC_Configuration();
    
//    TIM_Cmd(LED_TIMER, ENABLE); //开启时钟
}

/**
 * led初始化
 */
void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(LED1_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

    GPIO_InitStructure.GPIO_Pin   = LED1_GPIO_PIN;
    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);

    Timer_Configuration();
}

/**
 * 点亮led
 */
void led_on(void)
{
    if (!led_inited)
    {
        led_init();
        led_inited = 1;
    }

    GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
}

/**
 * 熄灭led
 */
void led_off(void)
{
    if (!led_inited)
    {
        led_init();
        led_inited = 1;
    }

    GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
}

/**
 * 开始led闪烁
 */
void led_twinkle_on(void)
{
    if (!led_inited)
    {
        led_init();
        led_inited = 1;
    }

    led_on();
    TIM_Cmd(LED_TIMER, ENABLE); //开启时钟
}

/**
 * 停止led闪烁
 */
void led_twinkle_off(void)
{
    if (led_inited)
    {
        TIM_Cmd(LED_TIMER, DISABLE); //关闭时钟
        led_off();
    }
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(LED_TIMER , TIM_IT_Update) == SET)  
    {  
        TIM_ClearITPendingBit(LED_TIMER , TIM_FLAG_Update);

        if (GPIO_ReadOutputDataBit(LED1_GPIO_PORT, LED1_GPIO_PIN) > 0)
        {
            led_off();
        }
        else
        {
            led_on();
        }
    }
}

