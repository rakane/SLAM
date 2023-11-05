#include "Mapper.h"

#include <iostream>
#include <sstream>
#include <string>

SLAM::Mapper::Mapper(bool enableMapUploading)
    : map_(), enableMapUploading_(enableMapUploading)
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
        uploadMapData(measurement, numMeasurements);
    }

}

void SLAM::Mapper::uploadMapData(MeasurementNode measurement[], unsigned int numMeasurements) const
{
    std::cout << "Uploading map data" << std::endl;

    // Polar map data
    const SLAM::PolarMap& polarMap = map_.getPolarMap();

    // Make a string stream for CURL request
    std::stringstream angleSStream;
    std::stringstream distanceSStream;

    unsigned long numPoints = 0;
    for (PolarMap::const_iterator it = polarMap.begin(); it != polarMap.end(); it++)
    {
        if(it->second < MAX_POLAR_DISTANCE)
        {
            angleSStream << it->first << ",";
            distanceSStream << it->second << ",";
            numPoints++;
        }
    }

    // Add dummy element
    angleSStream << "0";
    distanceSStream << "0";

    std::stringstream ss;
    ss << "curl -X POST http://localhost:8080/upload/polar -H \"Content-Type: application/json\"";
    ss << " -d '{\"angle\": [" << angleSStream.str() << "],";
    ss << " \"distance\": [" << distanceSStream.str() << "]}'";

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

    // Cartesian map data
    const SLAM::CartesianMap& cartesianMap = map_.getCartesianMap();

    // Make a string stream for CURL request
    std::stringstream xSStream;
    std::stringstream ySStream;

    numPoints = 0;
    for (CartesianMap::const_iterator it = cartesianMap.begin(); it != cartesianMap.end(); it++)
    {
        if(it->second)
        {
            xSStream << it->first.first << ",";
            ySStream << it->first.second << ",";
            numPoints++;
        }
    }

    // Add dummy element
    xSStream << "0";
    ySStream << "0";

    std::stringstream ss2;
    ss2 << "curl -X POST http://localhost:8080/upload/cartesian -H \"Content-Type: application/json\"";
    ss2 << " -d '{\"x\": [" << xSStream.str() << "],";
    ss2 << " \"y\": [" << ySStream.str() << "],";

    ss2 << " \"resolution\": " << CARTESTIAN_MAP_RESOLUTION << "}'";

    // Pipe output to /dev/null
    ss2 << " > /dev/null 2>&1";
    ss2 << " &";
    
    try
    {
        std::string command = ss2.str();
        system(command.c_str());
    }
    catch (...)
    {
        std::cout << "Failed to upload cartesian map data" << std::endl;
    }
}

