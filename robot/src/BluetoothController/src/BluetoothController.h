#ifndef BLUETOOTH_CONTROLLER_H
#define BLUETOOTH_CONTROLLER_H

#include <mutex>

namespace SLAM
{
    class MotorControllerInterface
}

class BluetoothController
{
public:
    BluetoothController(SLAM::MotorControllerInterface* motorController);
    ~BluetoothController();

    void run();
    void terminate();

private:

    enum BluetoothState
    {
        IDLE = 0,
        SCANNING,
        RUNNING,
        TERMINATED,
    };

    // Bluetooth state
    std::mutex stateMutex_;
    BluetoothState state_;

    int bluetoothSocket_;
    int bluetoothClient_;

    // Motor controller
    SLAM::MotorControllerInterface* motorController_;
};

#endif
