#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <thread>
#include <unistd.h>

#include "include/rplidar.h" //RPLIDAR sdk
#include "./src/Mapper/Mapper.h"
#include "./src/Sensors/Lidar.h"

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
    // Trap Ctrl-C
    signal(SIGINT, ctrlc);

    SLAM::Mapper* mapper = new SLAM::Mapper();
    SLAM::Lidar* lidar = new SLAM::Lidar(PORT, mapper);

    // Start LIDAR
    bool success = lidar->setup();

    if(!success)
    {
        printf("LIDAR setup failed\n");
        return 1;
    }

    // Run thread
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

    return 0;
}