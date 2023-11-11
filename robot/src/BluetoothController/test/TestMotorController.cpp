#include <iostream>

#include "TestMotorController.h"

BluetoothTest::TestMotorController::TestMotorController()
{
    std::cout << "TestMotorController::TestMotorController()" << std::endl;
}

BluetoothTest::TestMotorController::~TestMotorController()
{
    std::cout << "TestMotorController::~TestMotorController()" << std::endl;
}

void BluetoothTest::TestMotorController::processDirectionCommand(std::string command)
{
    std::cout << "TestMotorController::processDirectionCommand(): " << command << std::endl;
}