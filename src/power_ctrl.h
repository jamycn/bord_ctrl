
#ifndef _POWER_CTRL_H_
#define _POWER_CTRL_H_

/**
 * 电源控制初始化
 */
void power_ctrl_init(void);

/**
 * 获取boot键激活状态
 * @return
 *     1 -- 激活
 *     0 -- 未激活
 */
int power_ctrl_is_boot_key_active(void);

/**
 * 引导系统启动
 */
void power_ctrl_boot_on(void);

#endif

