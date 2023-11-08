#ifndef MOTOR_H
#define MOTOR_H

#include "../BBIO/PWM.h"

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
        FORWARD = 0,
        BACKWARD = 1
    };

    enum MotorSpeed
    {
        OFF = 0,
        ON = 1
    };

    class Motor
    {
    public:
        Motor( MotorLabel motorLabel, BBIO::PWMLabel pwmLabel);
        ~Motor();

        void setSpeed(MotorSpeed speed);
        void setDirection(MotorDirection direction);
        void setSpeedAndDirection(MotorSpeed speed, MotorDirection direction);

        MotorSpeed getSpeed();
        MotorDirection getDirection(); 

    private:
        void updatePwmPin();

        MotorLabel motorLabel_;
        MotorDirection direction_;
        MotorSpeed speed_;

        BBIO::PWM pwm_;
    };
}

#endif // MOTOR_H
