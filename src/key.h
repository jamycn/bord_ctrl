/**
 * 按键处理
 */

#ifndef _KEY_H_
#define _KEY_H_

/**
 * 按键初始化
 */
void key_init(void);

/**
 * 获取电源键状态
 * @return
 *     1 -- 激活
 *     0 -- 未激活
 */
int key_power_state(void);

/**
 * 获取亮度调节键状态
 * @return
 *     1 -- 激活
 *     0 -- 未激活
 */
int key_light_step_state(void);

#endif

