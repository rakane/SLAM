#include <iostream>

#include "../BBIO/PWM.h"

#include "Motor.h"
#include "MotorController.h"

SLAM::MotorController::MotorController()
{
    std::cout << "MotorController::MotorController()" << std::endl;

    frontLeftMotor_ = new SLAM::Motor(SLAM::MotorLabel::FRONT_LEFT, 75, 77);
    frontRightMotor_ = new SLAM::Motor(SLAM::MotorLabel::FRONT_RIGHT, 79, 80);
    backLeftMotor_ = new SLAM::Motor(SLAM::MotorLabel::BACK_LEFT, 73, 71);
    backRightMotor_ = new SLAM::Motor(SLAM::MotorLabel::BACK_RIGHT, 68, 67);
}

SLAM::MotorController::~MotorController()
{
    std::cout << "MotorController::~MotorController()" << std::endl;

    // Write STOP to all motors
    frontLeftMotor_->setDirection(MotorDirection::OFF);
    frontRightMotor_->setDirection(MotorDirection::OFF);
    backLeftMotor_->setDirection(MotorDirection::OFF);
    backRightMotor_->setDirection(MotorDirection::OFF);


    delete frontLeftMotor_;
    delete frontRightMotor_;
    delete backLeftMotor_;
    delete backRightMotor_;
}

void SLAM::MotorController::processDirectionCommand(Direction direction)
{
    std::cout << "MotorController::processDirectionCommand(): " << static_cast<int>(direction) << std::endl;

    // Command mapping:
    //
    // F (Forward): All motors set to FORWARD
    // B (Backward): All motors set to BACKWARD
    // L (Left): Left 2 motors set FORWARD, right 2 set to BACKWARD
    // R (Right): Right 2 motors set FORWARD, left 2 set to BACKWARD
    // S (Stop) : All motors set to OFF
    //

    MotorDirection frontLeftDirection = MotorDirection::OFF;
    MotorDirection frontRightDirection = MotorDirection::OFF;
    MotorDirection backLeftDirection = MotorDirection::OFF;
    MotorDirection backRightDirection = MotorDirection::OFF;
    
    if(direction == Direction::FORWARD)
    {
        frontLeftDirection = MotorDirection::FORWARD;
        frontRightDirection = MotorDirection::FORWARD;
        backLeftDirection = MotorDirection::FORWARD;
        backRightDirection = MotorDirection::FORWARD;
    }
    else if(direction == Direction::BACKWARD)
    {
        frontLeftDirection = MotorDirection::BACKWARD;
        frontRightDirection = MotorDirection::BACKWARD;
        backLeftDirection = MotorDirection::BACKWARD;
        backRightDirection = MotorDirection::BACKWARD;
    }
    else if(direction == Direction::LEFT)
    {
        frontLeftDirection = MotorDirection::FORWARD;
        frontRightDirection = MotorDirection::BACKWARD;
        backLeftDirection = MotorDirection::FORWARD;
        backRightDirection = MotorDirection::BACKWARD;
    }
    else if(direction == Direction::RIGHT)
    {
        frontLeftDirection = MotorDirection::BACKWARD;
        frontRightDirection = MotorDirection::FORWARD;
        backLeftDirection = MotorDirection::BACKWARD;
        backRightDirection = MotorDirection::FORWARD;
    }

    frontLeftMotor_->setDirection(frontLeftDirection);
    frontRightMotor_->setDirection(frontRightDirection);
    backLeftMotor_->setDirection(backLeftDirection);
    backRightMotor_->setDirection(backRightDirection);
}

