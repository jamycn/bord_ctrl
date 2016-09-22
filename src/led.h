/**
 * led控制
 */

#ifndef _LED_H_
#define _LED_H_

/**
 * led初始化
 */
void led_init(void);

/**
 * 点亮电源灯
 */
void led_power_on(void);

/**
 * 熄灭电源灯
 */
void led_power_off(void);

/**
 * 开始电源灯闪烁
 */
void led_power_twinkle_on(void);

/**
 * 停止电源灯闪烁
 */
void led_power_twinkle_off(void);

#endif

