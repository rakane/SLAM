#ifndef BLUETOOTH_CONTROLLER_H
#define BLUETOOTH_CONTROLLER_H

#include <mutex>

namespace SLAM
{
    class MotorController;
}

class BluetoothController
{
public:
    BluetoothController(SLAM::MotorController* motorController);
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
    SLAM::MotorController* motorController_;
};

#endif
