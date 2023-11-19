#ifndef DISPLAY_MAP_H
#define DISPLAY_MAP_H

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <map>
#include <cmath>
#include <chrono>

#include "MapperInterface.h"

namespace SLAM
{
    typedef std::map<double, double> PolarMap;        
    typedef std::pair<int, int> CartesianPoint; // mm
    typedef std::map<CartesianPoint, bool> CartesianMap;

    // Constants to bound the size of our experiment for now:
    // max 10m x 10m room, angle resolution of 0.01 degrees
    //
    const double ANGLE_RESOLUTION = 0.1; // degree resolution
    
    const int MAX_X_SIZE = std::round(5.0 * 1000.0); // 10 m in mm
    const int MAX_Y_SIZE = std::round(5.0 * 1000.0); // 10 m in mm
    const unsigned int CARTESTIAN_MAP_RESOLUTION = 10; // mm resolution
    const unsigned int MAX_MAP_POINTS = std::round(MAX_X_SIZE * MAX_Y_SIZE / (CARTESTIAN_MAP_RESOLUTION * CARTESTIAN_MAP_RESOLUTION));
    const unsigned int NUM_POLAR_ANGLES = std::round(360.0 / ANGLE_RESOLUTION);
    const double MAX_POLAR_DISTANCE = std::sqrt(MAX_X_SIZE * MAX_X_SIZE + MAX_Y_SIZE * MAX_Y_SIZE);

    class DisplayMap
    {

    public:
        DisplayMap(std::string serverUrl, double uploadInterval = 0.0, bool enableMapUploading = true);
        ~DisplayMap();

        bool update(SLAM::MeasurementNode measurements[], unsigned int numMeasurements);

        void resetMap();
    private:
        
        void update(const SLAM::MeasurementNode& measurement);      
 
        void uploadMap(MeasurementNode measurements[], unsigned int numMeasurements) const;

        // Polar map relative to robot
        PolarMap polarMap_;

        // Cartesian map relative to origin (robot start), true if occupied, false if free
        CartesianMap cartesianMap_;

        // Map upload configuration
        std::string serverUrl_;
        bool enableMapUploading_;
        bool throttleUploads_;
        double uploadInterval_;
        std::chrono::time_point<std::chrono::system_clock> lastUploadTime_; 
    };
}

#endif // DISPLAY_MAP_H
