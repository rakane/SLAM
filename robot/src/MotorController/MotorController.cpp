
#include <iostream>

#include "../BBIO/PWM.h"

#include "MotorController.h"

SLAM::MotorController::MotorController()
{
    std::cout << "MotorController::MotorController()" << std::endl;

    frontLeftMotor_ = SLAM::Motor(SLAM::MotorLabel::FRONT_LEFT, BBIO::PWMLabel::PWM0);
    frontRightMotor_ = SLAM::Motor(SLAM::MotorLabel::FRONT_RIGHT, BBIO::PWMLabel::PWM1);
    backLeftMotor_ = SLAM::Motor(SLAM::MotorLabel::BACK_LEFT, BBIO::PWMLabel::PWM2);
    backRightMotor_ = SLAM::Motor(SLAM::MotorLabel::BACK_RIGHT, BBIO::PWMLabel::PWM3);
}

SLAM::MotorController::~MotorController()
{
    std::cout << "MotorController::~MotorController()" << std::endl;
}