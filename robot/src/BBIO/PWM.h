#ifndef PWM_H
#define PWM_H

#include <iostream>

///////////////////////////////////////////
// Beaglebone GW PWM pins to file mapping (trial and error based):
//
// P9_14/P_36 -> /sys/class/pwm/pwmchip5/pwm0/
// P9_16/P9_34 -> /sys/class/pwm/pwmchip5/pwm1/
// P9_22/P9_31 (P9_31 not working, missing from ocp/) -> /sys/class/pwm/pwmchip3/pwm0/
// P9_21/P9_29 (P9-29 not working, missing from ocp/) -> /sys/class/pwm/pwmchip3/pwm1/
// P8_19/P8_45 -> /sys/class/pwm/pwmchip7/pwm0/
// P8_13/P8_46 -> /sys/class/pwm/pwmchip7/pwm1/
// P9_42 (PWM Capture) -> /sys/class/pwm/pwmchip0/pwm0/
// P9_28 (PWM Capture) -> Not working, cannot find in ocp/ folder ???
///////////////////////////////////////////

///////////////////////////////////////////
// Pin Setup (system calls):
//
// config-pin <pin> pwm
// echo 0 > /sys/class/pwm/pwmchip0/export
// echo 1000000 > /sys/class/pwm/pwmchip0/pwm0/period (nanoseconds)
// echo 500000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle (nanoseconds)
// echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable
///////////////////////////////////////////

namespace BBIO
{   
    // PWM label to be used by user to identify PWM pin
    enum PWMLabel
    {
        PWM0 = 0,
        PWM1 = 1,
        PWM2 = 2,
        PWM3 = 3,
        PWM4 = 4,
        PWM5 = 5,
        MAX_NUM_PWM_PINS = 6
    };
    class PWM
    {
    public:
        PWM(PWMLabel pwmLabel);
        ~PWM();

        bool configure();

        bool setDutyCycle(unsigned int dutyCycleInNs);

        bool setPeriod(unsigned int periodInNs);

        bool setEnabled(bool enable);

    private:
        // Pin used for config-pin call
        std::string PWMPin[MAX_NUM_PWM_PINS] = 
        {
            "p9.14",
            "p9.16",
            "p9.22",
            "p9.21",
            "p8.19",
            "p8.13"
        };

        // PWM chip directory
        std::string PWMChip[MAX_NUM_PWM_PINS] =
        {
            "pwmchip5",
            "pwmchip5",
            "pwmchip3",
            "pwmchip3",
            "pwmchip7",
            "pwmchip7"
        };

        // PWM number inside PWM chip directory
        std::string PWMNumber[MAX_NUM_PWM_PINS] =
        {
            "0",
            "1",
            "0",
            "1",
            "0",
            "1"
        };


        // PWM configuration
        PWMLabel pwmLabel_;
        std::string pwmPin_;
        std::string pwmChip_;
        std::string pwmNumber_;
        std::string pwmChipFilePath_;
        std::string pwmPinFilePath_;

        unsigned int dutyCycleInNs_;
        unsigned int periodInNs_;
        bool enabled_;
    };
};

#endif // PWM_H