#include "Mapper.h"

#include <iostream>
#include <sstream>
#include <string>

SLAM::Mapper::Mapper(double uploadInterval, bool enableMapUploading)
    : map_(), enableMapUploading_(enableMapUploading),
      throttleUploads_(uploadInterval > 0.0), uploadInterval_(uploadInterval),
      lastUploadTime_(std::chrono::system_clock::now())
{
}

SLAM::Mapper::~Mapper()
{
}

void SLAM::Mapper::reset()
{
    map_.resetMap();
}

void SLAM::Mapper::processMeasurementData(MeasurementNode measurement[], unsigned int numMeasurements)
{
    // Update map
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    for(unsigned int measIdx = 0; measIdx < numMeasurements; measIdx++)
    {
        map_.updateMap(measurement[measIdx]);
    }
    std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Map update took " << elapsed.count() << " seconds" << std::endl;


    // Upload map data if enabled
    if(enableMapUploading_)
    {
        if(throttleUploads_)
        {
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsedSinceLastUpload = now - lastUploadTime_;

            if(elapsedSinceLastUpload.count() < uploadInterval_)
            {
                return;
            }

            // Update last upload time 
            lastUploadTime_ = now;
        }

        uploadMapData(measurement, numMeasurements);
    }
}

void SLAM::Mapper::uploadMapData(MeasurementNode measurement[], unsigned int numMeasurements) const
{
    std::cout << "Uploading map data" << std::endl;

    // Map data
    const SLAM::PolarMap& polarMap = map_.getPolarMap();
    const SLAM::CartesianMap& cartesianMap = map_.getCartesianMap();

    // Make a string stream for CURL request
    std::stringstream angleSStream;
    std::stringstream distanceSStream;
    std::stringstream xSStream;
    std::stringstream ySStream;

    for (PolarMap::const_iterator it = polarMap.begin(); it != polarMap.end(); it++)
    {
        if(it->second < MAX_POLAR_DISTANCE)
        {
            angleSStream << it->first << ",";
            distanceSStream << it->second << ",";
        }
    }

    for (CartesianMap::const_iterator it = cartesianMap.begin(); it != cartesianMap.end(); it++)
    {
        if(it->second)
        {
            xSStream << it->first.first << ",";
            ySStream << it->first.second << ",";
        }
    }

    // Add dummy element
    angleSStream << "0";
    distanceSStream << "0";
    xSStream << "0";
    ySStream << "0";

    std::stringstream ss;
    ss << "curl -X POST http://localhost:8080/upload -H \"Content-Type: application/json\"";
    ss << " -d '{\"angle\": [" << angleSStream.str() << "],";
    ss << " \"distance\": [" << distanceSStream.str() << "],";
    ss << " \"x\": [" << xSStream.str() << "],";
    ss << " \"y\": [" << ySStream.str() << "],";
    ss << " \"resolution\": " << ANGLE_RESOLUTION << "}'";

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

