#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <thread>
#include <unistd.h>

#include "../../../include/rplidar/rplidar.h" //RPLIDAR sdk
#include "./Calibrator.h"
#include "../src/Lidar.h"

// Function to flag ctrl-c
bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

// #define PORT "/dev/tty.SLAB_USBtoUART"   // macOS UART for RPLIDAR
#define PORT "/dev/ttyUSB0"
#define RUN_TCP_CONTROLLER false

using namespace rp::standalone::rplidar;

int main(int argc, char **argv)
{
    float ANGLE_MIN = 15.0;
    float ANGLE_MAX = 45.0;
    float ANGLE_RESOLUTION = 1.0;
    unsigned long long MAX_SCAN_COUNT = 100;

    if(argc == 5)
    {
        std::cout << "Calibration parameter overrides: " << std::endl;

        ANGLE_MIN = atof(argv[1]);
        ANGLE_MAX = atof(argv[2]);
        ANGLE_RESOLUTION = atof(argv[3]);    
        MAX_SCAN_COUNT = atoi(argv[4]);

        std::cout << "\tANGLE_MIN: " << ANGLE_MIN << std::endl;
        std::cout << "\tANGLE_MAX: " << ANGLE_MAX << std::endl;
        std::cout << "\tANGLE_RESOLUTION: " << ANGLE_RESOLUTION << std::endl;
        std::cout << "\tMAX_SCAN_COUNT: " << MAX_SCAN_COUNT << std::endl;    
    } 
    else if(argc != 1)
    {
        std::cout << "Bad arguement usage: LidarCalibrationTest <angle min> <angle max> <angle resolution> <max scan count>\n";
    }

    // Trap Ctrl-C
    signal(SIGINT, ctrlc);

    SLAM::MapperInterface* calibrator = new LidarCalibrationTest::Calibrator(ANGLE_MIN, ANGLE_MAX, ANGLE_RESOLUTION, MAX_SCAN_COUNT);
    SLAM::Lidar* lidar = new SLAM::Lidar(PORT, calibrator);

    // Start LIDAR
    bool success = lidar->setup();

    if(!success)
    {
        std::cout << "LIDAR setup failed" << std::endl;
        return 1;
    }

    // Run threads
    std::thread lidarThread(&SLAM::Lidar::run, lidar);
    
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

    delete calibrator;
    delete lidar;

    return 0;
}
