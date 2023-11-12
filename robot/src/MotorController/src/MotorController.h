#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "./MotorControllerInterface.h"

namespace SLAM
{
    class Motor;

    class MotorController: public MotorControllerInterface
    {
    public:
        MotorController();
        ~MotorController();

        void processDirectionCommand(Direction direction) override;

    private:
        SLAM::Motor* frontLeftMotor_;
        SLAM::Motor* frontRightMotor_;
        SLAM::Motor* backLeftMotor_;
        SLAM::Motor* backRightMotor_;
    };
}

#endif // MOTOR_CONTROLLER_H