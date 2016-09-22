/**
 * 电源控制
 */

#ifndef _POWER_CTRL_H_
#define _POWER_CTRL_H_

/**
 * 电源控制初始化
 */
void power_ctrl_init(void);

/**
 * 低功耗配置
 */
void power_ctrl_low_consume_set(void);

/**
 * 电源控制启动
 */
void power_ctrl_on(void);

/**
 * 电源控制关闭
 */
void power_ctrl_off(void);

#endif

