
#include <iostream>

#include "Motor.h"

SLAM::Motor::Motor(MotorLabel motorLabel, BBIO::PWMLabel pwmLabel):
  motorLabel_(motorLabel),
  speed_(SLAM::MotorSpeed::OFF), 
  direction_(SLAM::MotorDirection::FORWARD),
  pwm_(pwmLabel)
{
    std::cout << "Motor::Motor()" << std::endl;

    // Configure PWM pin
    bool success = pwm_.configure();
    
    if (!success)
    {
        std::cout << "Motor::Motor() - pwm_.configure() failed" << std::endl;
    }
}

SLAM::Motor::~Motor()
{
    std::cout << "Motor::~Motor()" << std::endl;
}

void SLAM::Motor::setSpeed(MotorSpeed speed)
{
    speed_ = speed;
    updatePwmPin();
}

void SLAM::Motor::setDirection(MotorDirection direction)
{
    direction_ = direction;
    updatePwmPin();
}

void SLAM::Motor::setSpeedAndDirection(MotorSpeed speed, MotorDirection direction)
{
    speed_ = speed;
    direction_ = direction;
    updatePwmPin();
}

SLAM::MotorSpeed SLAM::Motor::getSpeed()
{
    return speed_;
}

SLAM::MotorDirection SLAM::Motor::getDirection()
{
    return direction_;
}

void SLAM::Motor::updatePwmPin()
{
    std::cout << "Motor::updatePwmPin()" << std::endl;

    // TODO: Convert speed and direction to PWM pin desired duty/period/enabled state
}