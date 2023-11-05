#ifndef LIDAR_H
#define LIDAR_H

#include <iostream>
#include <iomanip>
#include <stdio.h>

// RPLIDAR sdk
#include "../../../include/rplidar/rplidar.h"

#include "../../Mapper/src/Mapper.h"

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

using namespace rp::standalone::rplidar;

namespace SLAM
{
    enum LidarStatus
    {
        FAILED,
        READY,
        RUNNING,
        SHUTDOWN
    };

    class Lidar
    {
    public:
        Lidar(const char* port, Mapper* mapper);
        ~Lidar();
        
        bool setup();
        void run();
        void shutdown();
    private:
        bool checkRPLIDARHealth();

        const char * comPort_;
        _u32 baudrate_;
        u_result opResult_;
        LidarStatus status_;
        RPlidarDriver* driver_;

        // Mapper
        Mapper* mapper_;
    };
}

#endif