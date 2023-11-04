#include "Map.h"

#include <iostream>
#include <iomanip>
#include <stdio.h>

#ifndef MAPPER_H
#define MAPPER_H

namespace SLAM
{
    struct MeasurementNode {
        float angle; // degrees
        float distance; // millimeters
    };

    const unsigned int MAX_MEASUREMENTS = 8192;

    class Mapper
    {
    public:
        Mapper();
        ~Mapper();
        
        void reset();
        void processMeasurementData(MeasurementNode measurement[], unsigned int numMeasurements);

        void getMapPoint(float angle, float& distance);
    private:
        Map map_;
    };
}

#endif