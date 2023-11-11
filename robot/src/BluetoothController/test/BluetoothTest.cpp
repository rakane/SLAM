#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <thread>
#include <unistd.h>

#include "../src/BluetoothController.h"
#include "TestMotorController.h"

// Function to flag ctrl-c
bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main()
{
    printf("Testing...\n");

    SLAM::MotorControllerInterface* testMotorController = new BluetoothTest::TestMotorController();
    BluetoothController* bluetoothController = new BluetoothController(testMotorController);

    // Trap Ctrl-C
    signal(SIGINT, ctrlc);

    std::thread bluetoothThread(&BluetoothController::run, bluetoothController);

    // Wait for Ctrl-C
    while (!ctrl_c_pressed)
    {
        // Do nothing
        usleep(10000);
    }

    std::cout << "Ctrl-C pressed. Shutting down..." << std::endl;

    // Shutdown Bluetooth
    bluetoothController->terminate();
    bluetoothThread.join();

    delete bluetoothController;
    delete testMotorController;

    return 0;
}

