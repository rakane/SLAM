#include "Mapper.h"

#include <iostream>
#include <sstream>
#include <string>

SLAM::Mapper::Mapper()
{
}

SLAM::Mapper::~Mapper()
{
}

void SLAM::Mapper::reset()
{
    map_.resetMap();
}

static unsigned long measurementCount = 0;
void SLAM::Mapper::processMeasurementData(MeasurementNode measurement[], unsigned int numMeasurements)
{
    measurementCount++;

    printf("Num measurements: %u\n", numMeasurements);

    if(measurementCount % 10 == 0)
    {
        // Make a string stream for CURL request
        std::stringstream ss;
        ss << "curl -X POST http://localhost:8080/upload -H \"Content-Type: application/json\" -d";
        ss << " '{\"angle\": [";

        for (unsigned int i = 0; i < numMeasurements; i++)
        {
            ss << measurement[i].angle;
            if (i < numMeasurements - 1)
            {
                ss << ", ";
            }
        }
        ss << "],";

        ss << "\"distance\": [";

        for (unsigned int i = 0; i < numMeasurements; i++)
        {
            ss << measurement[i].distance;
            if (i < numMeasurements - 1)
            {
                ss << ", ";
            }
        }
        ss << "]}'";

        // Pipe output to /dev/null
        ss << " > /dev/null 2>&1";


        try
        {
            std::string command = ss.str();
            system(command.c_str());
        }
        catch (...)
        {
            std::cout << "Failed to upload map data" << std::endl;
        }
    }
}

void SLAM::Mapper::getMapPoint(float angle, float& distance)
{
    map_.getMapPoint(angle, distance);
}

