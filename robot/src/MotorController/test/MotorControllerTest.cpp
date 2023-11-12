#include <iostream>
#include <string>
#include <cstring>

#include "../src/MotorController.h"

int main()
{
    SLAM::MotorController* motorController = new SLAM::MotorController();

    char command[1024];
    while(1)
    {
        printf("Enter command: ");
        std::cin >> command;

        switch(command[0])
        {
            case 'F':
                motorController->processDirectionCommand(SLAM::Direction::FORWARD);
                break;
            case 'B':
                motorController->processDirectionCommand(SLAM::Direction::BACKWARD);
                break;
            case 'L':
                motorController->processDirectionCommand(SLAM::Direction::LEFT);
                break;
            case 'R':
                motorController->processDirectionCommand(SLAM::Direction::RIGHT); 
                break;
            case 'S':
                motorController->processDirectionCommand(SLAM::Direction::STOP);
                break;
        }

        if(strcmp(command, "exit") == 0)
        {
            break;
        }
    }

    delete motorController;

    return 0;
}

