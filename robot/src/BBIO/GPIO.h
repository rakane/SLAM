#ifndef GPIO_H
#define GPIO_H

#include <string>

namespace BBIO
{
    enum GpioDirection
    {
        IN = 0,
        OUT,
        MAX_NUM_DIRECTIONS,
    };

    enum GpioValue
    {
        LOW = 0,
        HIGH,
    };

    class GPIO {
    public:
        GPIO(const int pin, GpioDirection direction);
        GPIO(const int pin, GpioDirection direction, GpioValue value);

        GpioValue getValue();
        GpioDirection getDirection();

        void setValue(GpioValue value);
        void setDirection(GpioDirection direction);

    private:

        bool updatePin();

        const std::string GpioDirectionStrings[MAX_NUM_DIRECTIONS] = 
        {
            "in",
            "out",
        };

        int pin_;
        GpioDirection direction_;
        GpioValue value_;
    };
}

#endif