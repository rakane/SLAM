#ifndef LIDAR_H
#define LIDAR_H

#include <iostream>
#include <iomanip>
#include <stdio.h>

// RPLIDAR sdk
#include "../../../include/rplidar/rplidar.h"

#include "../../Mapper/src/MapperInterface.h"

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
        Lidar(const char* port, MapperInterface* mapper, double maxDistance = 0.0);
        ~Lidar();
        
        bool setup();
        void run();
        void shutdown();
    private:
        bool checkRPLIDARHealth();

        // RP Lidar Config
        const char * comPort_;
        _u32 baudrate_;
        u_result opResult_;
        RPlidarDriver* driver_;
    
        // Limit sensor measurement distance        
        double maxDistance_;

        // Lidar status
        LidarStatus status_;

        // Mapper
        MapperInterface* mapper_;
    };
}

#endif
