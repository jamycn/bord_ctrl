
#include "delay.h"
#include "key.h"
#include "plug_det.h"
#include "power_ctrl.h"
#include "pwm.h"

#if 1 //���߲��뵫δ��12V����ʱ����Դ����Ч�İ汾
int main(int argc, char *argv[])
{
    int nPlugState;
    int nPowerOnMode = 0; // 0 -- δ������1 -- ��Դ������ģʽ��2 -- ���߲�������ģʽ��

    power_ctrl_low_consume_set(); //�͹�������

    key_init();
    power_ctrl_init();
    plug_det_init();
    pwm_init();

    while (1) {
        if (nPowerOnMode == 0) {
            if (key_power_state() > 0 || plug_det_state() == 2) {
                if (plug_det_state() == 2) {
                    nPowerOnMode = 2;
                } else {
                    nPowerOnMode = 1;
                }

                power_ctrl_on();

                while (key_power_state() > 0); //�ȴ������ͷ�
            }
        } else {
            if (key_light_step_state() > 0) {
                pwm_leds_light_step(); //���Ȳ���һ��

                do {
                    delay_ms(200); //ȥ��
                } while (key_light_step_state() > 0); //�ȴ������ͷ�
            }
            
            nPlugState = plug_det_state();
            switch (nPlugState) {
            case 0://δ��������
                if (key_power_state() > 0) {
                    power_ctrl_off();
                    nPowerOnMode = 0;
                    while (key_power_state() > 0); //�ȴ������ͷ�
                } else {
                    if (nPowerOnMode == 2) {
                        nPowerOnMode = 1; //������ǵ�Դ������ģʽ�����л�
                    }
                }
                break;
            case 1: //���߲��뵫δ��12V����
                if (key_power_state() > 0) {
                    power_ctrl_off();
                    nPowerOnMode = 0;
                    while (key_power_state() > 0); //�ȴ������ͷ�
                } else {
                    if (nPowerOnMode == 2) {
                        power_ctrl_off();
                        nPowerOnMode = 0;
                    }
                }
                break;
            case 2: //���߲���������12V����
                if (nPowerOnMode == 1) {
                    nPowerOnMode = 2; //����������߲�������ģʽ�����л�
                }
                break;
            }
        }
    }
}
#else //���߲���ʱ����Դ��ʧЧ�İ汾
int main(int argc, char *argv[])
{
    int nPlugState;
    int nPowerOnState = 0; // 0 -- δ������1 -- ������

    key_init();
    power_ctrl_init();
    plug_det_init();
    pwm_init();

    while (1) {
        if (nPowerOnState == 0) {
            if (plug_det_state() > 0) {
                if (plug_det_state() == 2) {
                    nPowerOnState = 1;
                    power_ctrl_on();
                }
            } else {
                if (key_power_state() > 0) {
                    nPowerOnState = 1;
                    power_ctrl_on();
                }
            }
        } else {
            if (key_light_step_state() > 0) {
                pwm_leds_light_step(); //���Ȳ���һ��

                do {
                    delay_ms(200); //ȥ��
                } while (key_light_step_state() > 0); //�ȴ������ͷ�
            }
            
            nPlugState = plug_det_state();
            switch (nPlugState) {
            case 0://δ��������
                if (key_power_state() > 0) {
                    power_ctrl_off();
                    nPowerOnState = 0;
                }
                break;
            case 1: //���߲��뵫δ��12V����
                power_ctrl_off();
                nPowerOnState = 0;
                break;
            }
        }
    }
}
#endif

