#include <vector>

#include "DisplayMap.h"

SLAM::DisplayMap::DisplayMap(std::string serverUrl, double uploadInterval, bool enableMapUploading)
    : polarMap_(), 
      cartesianMap_(),
      serverUrl_(serverUrl),
      enableMapUploading_(enableMapUploading),
      throttleUploads_(uploadInterval > 0.0), 
      uploadInterval_(uploadInterval),
      lastUploadTime_(std::chrono::system_clock::now())
{
    // Fill in map with default values
    std::cout << "Initializing map..." << std::endl;

    // Polar map, set all angles to MAX_POLAR_DISTANCE
    for (double i = 0; i < NUM_POLAR_ANGLES; i++)
    {
        double angle = i * ANGLE_RESOLUTION;

        std::pair<double, double> polarPair = std::make_pair(angle, MAX_POLAR_DISTANCE);
        polarMap_.insert(polarPair);
    }

    // Cartesian map, set all points to false
    for (int x = -MAX_X_SIZE; x < MAX_X_SIZE; x += CARTESTIAN_MAP_RESOLUTION)
    {
        for (int y = -MAX_Y_SIZE; y < MAX_Y_SIZE; y += CARTESTIAN_MAP_RESOLUTION)
        {
            CartesianPoint point = std::make_pair(x, y);
            std::pair<CartesianPoint, bool> cartesianPair = std::make_pair(point, false);
            cartesianMap_.insert(cartesianPair);
        }
    }

    std::cout << "Map initialization complete!" << std::endl;
}

SLAM::DisplayMap::~DisplayMap()
{
}

bool SLAM::DisplayMap::update(SLAM::MeasurementNode measurements[], unsigned int numMeasurements)
{
    for(unsigned int measIdx = 0; measIdx < numMeasurements; measIdx++)
    {
        update(measurements[measIdx]);
    }
 
    // Upload map data if enabled
    if(enableMapUploading_)
    {
        if(throttleUploads_)
        {
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsedSinceLastUpload = now - lastUploadTime_;

            if(elapsedSinceLastUpload.count() < uploadInterval_)
            {
                return true;
            }

            // Update last upload time 
            lastUploadTime_ = now;
        }

        uploadMap(measurements, numMeasurements);
    }

    return true;
}

void SLAM::DisplayMap::update(const SLAM::MeasurementNode& measurement)
{
    // Measurement already in polar coordinates, round angle to nearest ANGLE_RESOLUTION
    double roundedAngle = std::round(measurement.angle / ANGLE_RESOLUTION) * ANGLE_RESOLUTION;

    // Ensure angle is between 0 and 360
    if(roundedAngle >= 360.0)
    {
        roundedAngle -= 360.0;
    }

    PolarMap::iterator polarIter = polarMap_.find(roundedAngle);
    if(polarIter != polarMap_.end())
    {
        polarIter->second = measurement.distance;
    }
    else
    {
        std::cout << "Angle not found in polar map! angle: " << roundedAngle << std::endl;
    }

    // Convert to all cartesian points along the line from the robot to the end of the map,
    // then check if there are any points marked true on the map, if so, mark them as false since
    // they did not get updated by the lidar. Then set the updated point to true below.

    double measurementRadians = measurement.angle * M_PI / 180.0;
    double cartesianXFactor = std::cos(measurementRadians);
    double cartesianYFactor = std::sin(measurementRadians);

    // Build list of all distance from 0 to measurement.distance, at a step of CARTESTIAN_MAP_RESOLUTION / 2
    double stepSize = CARTESTIAN_MAP_RESOLUTION / 2.0;

    double x, y;
    CartesianPoint point;
    for(double distance = 0.0; distance < measurement.distance; distance += stepSize)
    {
        x = distance * cartesianXFactor;
        y = distance * cartesianYFactor;
        point.first = std::round(x / CARTESTIAN_MAP_RESOLUTION) * CARTESTIAN_MAP_RESOLUTION;
        point.second = std::round(y / CARTESTIAN_MAP_RESOLUTION) * CARTESTIAN_MAP_RESOLUTION;

        // Set all points to false in cartesian map, as they did not get updated by the lidar
        CartesianMap::iterator cartesianIter = cartesianMap_.find(point);
        if(cartesianIter != cartesianMap_.end())
        {
            cartesianIter->second = false;
        } 
        else
        {
            std::cout << "Position not found in cartesian map! x: " << point.first << " y: " << point.second << std::endl;
        }
    }

    // Convert to cartesian coordinates
    x = measurement.distance * cartesianXFactor;
    y = measurement.distance * cartesianYFactor;

    // Round x and y to nearest CARTESTIAN_MAP_RESOLUTION
    point.first = std::round(x / static_cast<double>(CARTESTIAN_MAP_RESOLUTION)) * CARTESTIAN_MAP_RESOLUTION;
    point.second = std::round(y / static_cast<double>(CARTESTIAN_MAP_RESOLUTION)) * CARTESTIAN_MAP_RESOLUTION;

    CartesianMap::iterator cartesianIter = cartesianMap_.find(point);

    if(cartesianIter != cartesianMap_.end())
    {
        cartesianIter->second = true;
    }
    else
    {
        std::cout << "Position not found in cartesian map! x: " << point.first << " y: " << point.second << std::endl;
    }
}

void SLAM::DisplayMap::resetMap()
{
    polarMap_.clear();
    cartesianMap_.clear();
}

void SLAM::DisplayMap::uploadMap(MeasurementNode measurements[], unsigned int numMeasurements) const
{
    // Make a string stream for CURL request
    std::stringstream angleSStream;
    std::stringstream distanceSStream;
    std::stringstream xSStream;
    std::stringstream ySStream;
    std::stringstream measAngleSStream;
    std::stringstream measDistanceSStream;

    for (PolarMap::const_iterator it = polarMap_.begin(); it != polarMap_.end(); it++)
    {
        if(it->second < MAX_POLAR_DISTANCE)
        {
            angleSStream << it->first << ",";
            distanceSStream << it->second << ",";
        }
    }

    for (CartesianMap::const_iterator it = cartesianMap_.begin(); it != cartesianMap_.end(); it++)
    {
        if(it->second)
        {
            xSStream << it->first.first << ",";
            ySStream << it->first.second << ",";
        }
    }

    for(unsigned int measIdx = 0; measIdx < numMeasurements; measIdx++)
    {
        measAngleSStream << measurements[measIdx].angle << ",";
        measDistanceSStream << measurements[measIdx].distance << ",";
    }

    // Add dummy element
    angleSStream << "0";
    distanceSStream << "0";
    xSStream << "0";
    ySStream << "0";
    measAngleSStream << "0";
    measDistanceSStream << "0";

    std::stringstream ss;
    ss << "curl -X POST " << serverUrl_ << ":8080/upload -H \"Content-Type: application/json\"";
    ss << " -d '{\"angle\": [" << angleSStream.str() << "],";
    ss << " \"distance\": [" << distanceSStream.str() << "],";
    ss << " \"x\": [" << xSStream.str() << "],";
    ss << " \"y\": [" << ySStream.str() << "],";
    ss << " \"latestAngle\": [" << measAngleSStream.str() << "],";
    ss << " \"latestDistance\": [" << measDistanceSStream.str() << "],";
    ss << " \"resolution\": " << CARTESTIAN_MAP_RESOLUTION << "}'";

    // Pipe output to /dev/null
    ss << " > /dev/null 2>&1";
    ss << " &";

    try
    {
        std::string command = ss.str();
        system(command.c_str());
    }
    catch (...)
    {
        std::cout << "Failed to upload polar map data" << std::endl;
    }
}
