
#include "power_ctrl.h"

int main(int argc, char *argv[])
{
    power_ctrl_init();

    while (1) {
        if (power_ctrl_is_boot_key_active() > 0) {
            break;
        }
    }

    power_ctrl_boot_on();

    while (1) {
        
    }
}

