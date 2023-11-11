#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "GPIO.h"

BBIO::GPIO::GPIO(const int pin, GpioDirection direction, GpioValue value)
{
    pin_ = pin;
    direction_ = direction;
    value_ = value;

    updatePin();
}

BBIO::GPIO::GPIO(const int pin, GpioDirection direction) 
{
    pin_ = pin;
    direction_ = direction;
    value_ = GpioValue::LOW; // Default value
}

BBIO::GpioValue BBIO::GPIO::getValue() 
{
    return value_;
}

BBIO::GpioDirection BBIO::GPIO::getDirection() 
{
    return direction_;
}

void BBIO::GPIO::setValue(GpioValue value) 
{
    value_ = value;
    updatePin();
}

void BBIO::GPIO::setDirection(GpioDirection direction) 
{
    direction_ = direction;
    updatePin();
}

bool BBIO::GPIO::updatePin()
{
    bool success = true;

    // Set the direction
    std::ofstream dirStream;
    try 
    {
        std::string path = "/sys/class/gpio/gpio" + std::to_string(pin_) +  "/direction";
        dirStream.open(path);

        if(dirStream.good()) 
        {
            dirStream << GpioDirectionStrings[direction_]; 
            dirStream.close();
        } 
        else 
        {
            std::cout << "Error! Failed to write to GPIO pin " << pin_ << " direction file\n";
            success = false;
        }
    } 
    catch(int e) 
    {
        std::cout << "Error! Failed to write to GPIO pin " << pin_ << " direction file\n";
        success = false;
    }
    
    // If this is an output pin, set the value
    if(success && direction_ == GpioDirection::OUT)
    {
        try 
        {
            std::string path = "/sys/class/gpio/gpio" + std::to_string(pin_) +  "/value";
            std::ofstream valStream;

            if(valStream.good()) 
            {
                valStream.open(path);
                valStream << value_;
                valStream.close();
            } 
            else 
            {
                std::cout << "Error! Failed to write to GPIO pin " << pin_ << " value file\n";
                success = false;
            }
        } 
        catch(int e) 
        {
            std::cout << "Error! Failed to write to GPIO pin " << pin_ << " value file\n";
            success = false;
        }
    }

    return success;
}