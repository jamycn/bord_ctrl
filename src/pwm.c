/**
 * PWM�������
 */

#include "stm32f0xx.h"
#include "delay.h"

#define TIMER_PWM                 TIM15
#define TIMER_IRQ_PWM             TIM15_IRQn
#define RCC_TIMER_PWM             RCC_APB2Periph_TIM15
#define RCC_IO_PWM                RCC_AHBPeriph_GPIOB
#define GPIO_PORT_PWM             GPIOB
#define GPIO_PIN_PWM              GPIO_Pin_12

static int sg_nIsInited = 0;
static int sg_nPwmCnt = 0;
static int sg_nStep = 0;

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQ_PWM;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;//ռ���ȼ�
          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}

static void Timer_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB2PeriphClockCmd(RCC_TIMER_PWM, ENABLE);

    //1us�Ķ�ʱ����1MHz��Ƶ��, ��ʽ Frequency=(SYSCLK/(Prescaler+1))/(Period+1)
    TIM_DeInit(TIMER_PWM);
    TIM_TimeBaseStructure.TIM_Period = 2 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 24 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
    TIM_TimeBaseInit(TIMER_PWM, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIMER_PWM, TIM_FLAG_Update); //�������жϱ�־
    TIM_ITConfig(TIMER_PWM,TIM_IT_Update,ENABLE); 

    NVIC_Configuration();
    
    RCC_APB2PeriphClockCmd(RCC_TIMER_PWM, DISABLE);
}

/**
 * pwm�����ʼ��
 */
void pwm_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_IO_PWM, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_PWM;
    GPIO_Init(GPIO_PORT_PWM, &GPIO_InitStructure);

    GPIO_ResetBits(GPIO_PORT_PWM, GPIO_PIN_PWM);
    delay_ms(3); //��ʱ3msȷ��оƬΪ�µ�״̬
    sg_nStep = 0;
    sg_nIsInited = 1;
    
    Timer_Configuration();
}

/**
 * ���2us���ڵ�pwm,�ȵͺ��
 * @param
 *     nCount �������
 */
static void pwm_out(int nCount)
{
    if (!sg_nIsInited)
    {
        pwm_init();
    }

    sg_nPwmCnt = nCount * 2;

    RCC_APB2PeriphClockCmd(RCC_TIMER_PWM, ENABLE);
    TIM_Cmd(TIMER_PWM, ENABLE);
    
    while (sg_nPwmCnt > 0);
    
    TIM_Cmd(TIMER_PWM, DISABLE);    
    RCC_APB2PeriphClockCmd(RCC_TIMER_PWM, DISABLE);
} 

/**
 * ÿ����һ����ǰ����һ����������(������)
 */
void pwm_leds_light_step(void)
{
    switch (sg_nStep) {
        case 1:
            pwm_out(10);
            break;
        case 2:
            pwm_out(5);
            break;
        case 3:
            pwm_out(1);
            break;
        default:
            return;
    }
    
    if (sg_nStep < 3) {
        sg_nStep++;
    } else {
        sg_nStep = 1;
    }
} 

/**
 * �������飬��ʹ���ϴιرյ���ǰ����
 */
void pwm_leds_light_on(void)
{
    GPIO_SetBits(GPIO_PORT_PWM, GPIO_PIN_PWM);
    delay_ms(1); //��ʱ>=20usȷ��оƬΪ�ϵ�״̬

    if (sg_nStep < 2) {
        sg_nStep = 1;
    } else {
        /* ʹ���ϴιرյ���ǰ���� */
        int nSteps = sg_nStep;
        sg_nStep = 1;
        while (sg_nStep < nSteps) {
            pwm_leds_light_step();
        }
    }
}

/**
 * �رյ���
 */
void pwm_leds_light_off(void)
{
    GPIO_ResetBits(GPIO_PORT_PWM, GPIO_PIN_PWM);
    delay_ms(3); //��ʱ3msȷ��оƬΪ�µ�״̬
}

void TIM15_IRQHandler(void)
{
    if(TIM_GetITStatus(TIMER_PWM , TIM_IT_Update) == SET)  
    {  
        TIM_ClearITPendingBit(TIMER_PWM , TIM_FLAG_Update);

        if (sg_nPwmCnt > 0) {
            sg_nPwmCnt--;
            if (sg_nPwmCnt % 2 > 0)
            {
                GPIO_ResetBits(GPIO_PORT_PWM, GPIO_PIN_PWM);
            }
            else
            {
                GPIO_SetBits(GPIO_PORT_PWM, GPIO_PIN_PWM);
            }
        }
    }
}

