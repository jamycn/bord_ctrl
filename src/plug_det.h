
#ifndef _PLUG_DETECT_H_
#define _PLUG_DETECT_H_

/**
 * �������ʼ��
 */
void plug_det_init(void);

/**
 * ��ȡ����״̬
 * @return
 *     0 -- δ����
 *     1 -- ���뵫δ��12V����
 *     2 -- ����������12V����
 */
int plug_det_state(void);

#endif

