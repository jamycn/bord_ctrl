/**
 * PWM波形输出
 */

#ifndef _PWM_H_
#define _PWM_H_

/**
 * pwm输出初始化
 */
void pwm_init(void);

/**
 * 开启灯组，并使用上次关闭灯组前亮度
 */
void pwm_leds_light_on(void);

/**
 * 每调用一次向前步进一级灯组亮度(共三级)
 */
void pwm_leds_light_step(void);

/**
 * 关闭灯组
 */
void pwm_leds_light_off(void);

#endif

