#ifndef MOTOR_CONTROLLER_INTERFACE_H
#define MOTOR_CONTROLLER_INTERFACE_H

#include <string>

namespace SLAM
{
    enum class Direction
    {
        FORWARD = 0,
        BACKWARD,
        LEFT,
        RIGHT,
        STOP
    };

    class MotorControllerInterface
    {
    public:
        MotorControllerInterface() = default;
        virtual ~MotorControllerInterface() = default;

        virtual void processDirectionCommand(Direction direction) = 0;
    };
}

#endif