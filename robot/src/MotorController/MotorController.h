#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

namespace SLAM
{
    class Motor;

    class MotorController
    {
    public:
        MotorController();
        ~MotorController();

    private:
        SLAM::Motor* frontLeftMotor_;
        SLAM::Motor* frontRightMotor_;
        SLAM::Motor* backLeftMotor_;
        SLAM::Motor* backRightMotor_;
    };
}

#endif // MOTOR_CONTROLLER_H