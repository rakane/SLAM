
#include <iostream>
#include <iomanip>

#include "Lidar.h"

SLAM::Lidar::Lidar(const char* port, MapperInterface* mapper)
{
    comPort_ = port;
    mapper_ = mapper;
    status_ = LidarStatus::SHUTDOWN;
    baudrate_ = 115200;
    driver_ = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
}

SLAM::Lidar::~Lidar()
{
    if(status_ != LidarStatus::SHUTDOWN)
    {
        shutdown();
    }
}

bool SLAM::Lidar::setup()
{
    bool connectSuccess = false;
    rplidar_response_device_info_t devinfo;

    if(IS_OK(driver_->connect(comPort_, baudrate_)))
    {
        opResult_ = driver_->getDeviceInfo(devinfo);

        if (IS_OK(opResult_)) 
        {
            connectSuccess = true;
        }
        else
        {
            RPlidarDriver::DisposeDriver(driver_);
            driver_ = NULL;
        }
    }

    if (!connectSuccess) 
    {
        std::cout << "Error, cannot bind to the specified serial port " << comPort_
                    << ". result = " << std::hex << opResult_ << std::endl; 

        RPlidarDriver::DisposeDriver(driver_);
        driver_ = NULL;
        return false;
    }

    // Display RPLIDAR Info
    printf("Connected via %s at %d baud\n", comPort_, baudrate_);
    printf("RPLIDAR S/N: ");
    for (int pos = 0; pos < 16 ;++pos) {
        printf("%02X", devinfo.serialnum[pos]);
    }
    printf("\n"
            "Firmware Ver: %d.%02d\n"
            "Hardware Rev: %d\n"
            , devinfo.firmware_version>>8
            , devinfo.firmware_version & 0xFF
            , (int)devinfo.hardware_version);

    status_ = LidarStatus::READY;

    return true;
}

void SLAM::Lidar::run()
{
    if(status_ != LidarStatus::READY)
    {
        setup();
    }

    if(status_ == LidarStatus::READY)
    {
        driver_->startMotor();
        driver_->startScan(0,1);    
        status_ = LidarStatus::RUNNING;

        std::cout << "Running lidar..." << std::endl;

        while(status_ == LidarStatus::RUNNING)
        {
            // Fetch data from LIDAR
            rplidar_response_measurement_node_hq_t nodes[8192];
            size_t count = 8192;

            opResult_ = driver_->grabScanDataHq(nodes, count);

            if (IS_OK(opResult_)) {
                driver_->ascendScanData(nodes, count);


                SLAM::MeasurementNode measurement[8192];
                int measurementIdx = 0;
                for (int pos = 0; pos < (int)count ; ++pos) 
                {
                    if(nodes[pos].quality != 0)
                    {
                        measurement[measurementIdx].angle = nodes[pos].angle_z_q14 * 90.f / (1 << 14);
                        measurement[measurementIdx].distance = nodes[pos].dist_mm_q2 / 4.0f;

                        measurementIdx++;
                    }
                }

                mapper_->processMeasurementData(measurement, measurementIdx + 1);
            }
            else
            {
                std::cout << "Error, cannot grab scan data. result = " << std::hex << opResult_ << std::endl;
            }
        }        
    }

    std::cout << "Exiting lidar run loop..." << std::endl;
}

void SLAM::Lidar::shutdown()
{
    status_ = LidarStatus::SHUTDOWN;
    driver_->stop();
    driver_->stopMotor();
    driver_->disconnect();
    RPlidarDriver::DisposeDriver(driver_); // Cleans up memory
    driver_ = NULL;
}

bool SLAM::Lidar::checkRPLIDARHealth()
{
    rplidar_response_device_health_t healthinfo;
    opResult_ = driver_->getHealth(healthinfo);

    // the macro IS_OK is the preperred way to judge whether the operation is succeed.
    if (IS_OK(opResult_)) 
    { 
        std::cout << "RPLidar health status : " << healthinfo.status << std::endl;
        if (healthinfo.status == RPLIDAR_STATUS_ERROR) 
        {
            std::cout << "Error, rplidar internal error detected. Please reboot the device to retry." << std::endl;

            return false;
        } else 
        {
            return true;
        }

    } 
    else 
    {
        std::cout << "Error, cannot retrieve the lidar health code: " << std::hex << opResult_ << std::endl;
        return false;
    }
}
