
#include <iostream>

#include "Motor.h"

SLAM::Motor::Motor(MotorLabel motorLabel, unsigned short gpioPin1, unsigned short gpioPin2):
    motorLabel_(motorLabel),
    direction_(SLAM::MotorDirection::OFF),
    gpio1_(gpioPin1, BBIO::GpioDirection::OUT, BBIO::GpioValue::LOW),
    gpio2_(gpioPin2, BBIO::GpioDirection::OUT, BBIO::GpioValue::LOW)
{
    std::cout << "Motor::Motor()" << std::endl;
}

SLAM::Motor::~Motor()
{
    std::cout << "Motor::~Motor()" << std::endl;
}

void SLAM::Motor::setDirection(MotorDirection direction)
{
    direction_ = direction;
    updateGpioPins();
}

SLAM::MotorDirection SLAM::Motor::getDirection()
{
    return direction_;
}

void SLAM::Motor::updateGpioPins()
{
    std::cout << "Motor::updateGpioPin()" << std::endl;

    // L298N Motor Driver: 
    // - Both pins LOW or HIGH = STOPPED
    // - pin 1 HIGH and pin 2 LOW = FORWARD
    // - pin 1 LOW and pin 2 HIGH = BACKWARDS
    //
    // This makes some assumptions about which order the pins are passed
    // and connected to the motor. If flipped just switch pin order passed in
    //
    
    // Default to stopped
    BBIO::GpioValue pin1Value = BBIO::GpioValue::LOW;
    BBIO::GpioValue pin2Value = BBIO::GpioValue::LOW;

    if(direction_ == SLAM::MotorDirection::FORWARD)
    {
        pin1Value = BBIO::GpioValue::HIGH;
    }
    else if(direction_ == SLAM::MotorDirection::BACKWARD)
    {
        pin2Value = BBIO::GpioValue::HIGH;
    }
     
    gpio1_.setValue(pin1Value);
    gpio2_.setValue(pin2Value);
}


