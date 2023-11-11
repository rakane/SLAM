
#include <iostream>

#include "../BBIO/PWM.h"

#include "Motor.h"
#include "MotorController.h"

SLAM::MotorController::MotorController()
{
    std::cout << "MotorController::MotorController()" << std::endl;

    frontLeftMotor_ = new SLAM::Motor(SLAM::MotorLabel::FRONT_LEFT, BBIO::PWMLabel::PWM0);
    frontRightMotor_ = new SLAM::Motor(SLAM::MotorLabel::FRONT_RIGHT, BBIO::PWMLabel::PWM1);
    backLeftMotor_ = new SLAM::Motor(SLAM::MotorLabel::BACK_LEFT, BBIO::PWMLabel::PWM2);
    backRightMotor_ = new SLAM::Motor(SLAM::MotorLabel::BACK_RIGHT, BBIO::PWMLabel::PWM3);
}

SLAM::MotorController::~MotorController()
{
    std::cout << "MotorController::~MotorController()" << std::endl;

    delete frontLeftMotor_;
    delete frontRightMotor_;
    delete backLeftMotor_;
    delete backRightMotor_;
}

void SLAM::MotorController::processDirectionCommand(std::string command)
{
    std::cout << "MotorController::processDirectionCommand(): " << command << std::endl;
}