/**
 * PWM�������
 */

#ifndef _PWM_H_
#define _PWM_H_

/**
 * pwm�����ʼ��
 */
void pwm_init(void);

/**
 * �������飬��ʹ���ϴιرյ���ǰ����
 */
void pwm_leds_light_on(void);

/**
 * ÿ����һ����ǰ����һ����������(������)
 */
void pwm_leds_light_step(void);

/**
 * �رյ���
 */
void pwm_leds_light_off(void);

#endif

