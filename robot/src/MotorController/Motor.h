#ifndef MOTOR_H
#define MOTOR_H

#include "../BBIO/PWM.h"
#include "../BBIO/GPIO.h"

namespace SLAM
{
    enum MotorLabel
    {
        FRONT_LEFT = 0,
        FRONT_RIGHT = 1,
        BACK_LEFT = 2,
        BACK_RIGHT = 3
    };

    enum MotorDirection
    {
        OFF = 0,
        FORWARD = 1,
        BACKWARD = 2,
    };

    class Motor
    {
    public:
        Motor( MotorLabel motorLabel, BBIO::PWMLabel pwmLabel);
        ~Motor();

        void setDirection(MotorDirection direction);
        MotorDirection getDirection(); 

    private:
        void updatePwmPin();

        MotorLabel motorLabel_;
        MotorDirection direction_;

        BBIO::PWM pwm_;
    };
}

#endif // MOTOR_H
