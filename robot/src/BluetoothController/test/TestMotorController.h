#ifndef TEST_MOTOR_CONTROLLER_H
#define TEST_MOTOR_CONTROLLER_H

#include <string>

#include "../../MotorController/MotorControllerInterface.h"

namespace BluetoothTest
{
    class TestMotorController: public SLAM::MotorControllerInterface
    {
    public:
        TestMotorController();
        ~TestMotorController();

        void processDirectionCommand(std::string command) override;
    };
}

#endif
