
#include <iostream>

#include "../../MotorController/src/MotorControllerInterface.h"

class TestMotorController : public SLAM::MotorControllerInterface
{
public:
    TestMotorController()
    {
        std::cout << "TestMotorController::TestMotorController()" << std::endl;
    }

    ~TestMotorController()
    {
        std::cout << "TestMotorController::~TestMotorController()" << std::endl;
    }

    void processDirectionCommand(SLAM::Direction direction)
    {
        std::cout << "TestMotorController::processDirectionCommand(" << static_cast<int>(direction) << ")" << std::endl;
    }

};
