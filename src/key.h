/**
 * ��������
 */

#ifndef _KEY_H_
#define _KEY_H_

/**
 * ������ʼ��
 */
void key_init(void);

/**
 * ��ȡ��Դ��״̬
 * @return
 *     1 -- ����
 *     0 -- δ����
 */
int key_power_state(void);

/**
 * ��ȡ���ȵ��ڼ�״̬
 * @return
 *     1 -- ����
 *     0 -- δ����
 */
int key_light_step_state(void);

#endif

