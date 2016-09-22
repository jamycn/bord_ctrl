
#include "delay.h"
#include "key.h"
#include "plug_det.h"
#include "power_ctrl.h"
#include "pwm.h"

#if 1 //外线插入但未由12V触发时，电源键有效的版本
int main(int argc, char *argv[])
{
    int nPlugState;
    int nPowerOnMode = 0; // 0 -- 未启动；1 -- 电源键启动模式；2 -- 外线插入启动模式。

    power_ctrl_low_consume_set(); //低功耗配置

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

                while (key_power_state() > 0); //等待按键释放
            }
        } else {
            if (key_light_step_state() > 0) {
                pwm_leds_light_step(); //亮度步进一级

                do {
                    delay_ms(200); //去抖
                } while (key_light_step_state() > 0); //等待按键释放
            }
            
            nPlugState = plug_det_state();
            switch (nPlugState) {
            case 0://未插入外线
                if (key_power_state() > 0) {
                    power_ctrl_off();
                    nPowerOnMode = 0;
                    while (key_power_state() > 0); //等待按键释放
                } else {
                    if (nPowerOnMode == 2) {
                        nPowerOnMode = 1; //如果不是电源键启动模式，则切换
                    }
                }
                break;
            case 1: //外线插入但未由12V触发
                if (key_power_state() > 0) {
                    power_ctrl_off();
                    nPowerOnMode = 0;
                    while (key_power_state() > 0); //等待按键释放
                } else {
                    if (nPowerOnMode == 2) {
                        power_ctrl_off();
                        nPowerOnMode = 0;
                    }
                }
                break;
            case 2: //外线插入且已由12V触发
                if (nPowerOnMode == 1) {
                    nPowerOnMode = 2; //如果不是外线插入启动模式，则切换
                }
                break;
            }
        }
    }
}
#else //外线插入时，电源键失效的版本
int main(int argc, char *argv[])
{
    int nPlugState;
    int nPowerOnState = 0; // 0 -- 未启动；1 -- 启动。

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
                pwm_leds_light_step(); //亮度步进一级

                do {
                    delay_ms(200); //去抖
                } while (key_light_step_state() > 0); //等待按键释放
            }
            
            nPlugState = plug_det_state();
            switch (nPlugState) {
            case 0://未插入外线
                if (key_power_state() > 0) {
                    power_ctrl_off();
                    nPowerOnState = 0;
                }
                break;
            case 1: //外线插入但未由12V触发
                power_ctrl_off();
                nPowerOnState = 0;
                break;
            }
        }
    }
}
#endif

