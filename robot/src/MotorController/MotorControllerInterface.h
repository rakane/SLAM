#ifndef MOTOR_CONTROLLER_INTERFACE_H
#define MOTOR_CONTROLLER_INTERFACE_H

#include <string>

namespace SLAM
{
    class MotorControllerInterface
    {
    public:
        MotorControllerInterface() = default;
        virtual ~MotorControllerInterface() = default;

        virtual void processDirectionCommand(std::string command) = 0;
    };
}

#endif