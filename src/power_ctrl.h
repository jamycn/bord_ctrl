
#ifndef _POWER_CTRL_H_
#define _POWER_CTRL_H_

/**
 * ��Դ���Ƴ�ʼ��
 */
void power_ctrl_init(void);

/**
 * ��ȡboot������״̬
 * @return
 *     1 -- ����
 *     0 -- δ����
 */
int power_ctrl_is_boot_key_active(void);

/**
 * ����ϵͳ����
 */
void power_ctrl_boot_on(void);

#endif

