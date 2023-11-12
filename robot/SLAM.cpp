#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <thread>
#include <unistd.h>

#include "./include/rplidar/rplidar.h" //RPLIDAR sdk
#include "./src/Mapper/src/Mapper.h"
#include "./src/Sensors/src/Lidar.h"
#include "./src/MotorController/src/MotorController.h"
#include "./src/TcpController/src/TcpController.h"

// Function to flag ctrl-c
bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

#define PORT "/dev/tty.SLAB_USBtoUART"   // macOS UART for RPLIDAR

using namespace rp::standalone::rplidar;

int main()
{
    std::cout << "SLAM Experiment Configuration: " << std::endl;
    std::cout << "ANGLE_RESOLUTION: " << SLAM::ANGLE_RESOLUTION << std::endl;
    std::cout << "MAX_X_SIZE (mm): " << SLAM::MAX_X_SIZE << std::endl;
    std::cout << "MAX_Y_SIZE (mm): " << SLAM::MAX_Y_SIZE << std::endl;
    std::cout << "CARTESTIAN_MAP_RESOLUTION: " << SLAM::CARTESTIAN_MAP_RESOLUTION << std::endl; 
    std::cout << "MAX_MAP_POINTS: " << SLAM::MAX_MAP_POINTS << std::endl;
    std::cout << "NUM_POLAR_ANGLES: " << SLAM::NUM_POLAR_ANGLES << std::endl;
    std::cout << "MAX_POLAR_DISTANCE: " << SLAM::MAX_POLAR_DISTANCE << std::endl;

    // Trap Ctrl-C
    signal(SIGINT, ctrlc);

    SLAM::Mapper* mapper = new SLAM::Mapper();
    SLAM::Lidar* lidar = new SLAM::Lidar(PORT, mapper);
    SLAM::MotorControllerInterface* motorController = new SLAM::MotorController();    
    TcpController* tcpController = new TcpController(motorController);

    // Start LIDAR
    bool success = lidar->setup();

    if(!success)
    {
        std::cout << "LIDAR setup failed" << std::endl;
        return 1;
    }

    // Run threads
    std::thread lidarThread(&SLAM::Lidar::run, lidar);
    std::thread controllerThread(&TcpController::run, tcpController);

    // Wait for Ctrl-C
    while (!ctrl_c_pressed)
    {
        // Do nothing
        usleep(10000);
    }

    std::cout << "Ctrl-C pressed. Shutting down..." << std::endl;

    // Stop LIDAR
    lidar->shutdown();
    lidarThread.join();    

    // Stop TCP Controller
    tcpController->terminate();
    controllerThread.join();

    delete mapper;
    delete lidar;
    delete motorController;
    delete tcpController;

    return 0;
}
