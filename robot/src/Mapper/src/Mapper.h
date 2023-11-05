#ifndef MAPPER_H
#define MAPPER_H

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <map>
#include <chrono>

#include "Map.h"

namespace SLAM
{
    const unsigned int MAX_MEASUREMENTS = 8192;

    class Mapper
    {
    public:
        Mapper(bool enableMapUploading = true);
        ~Mapper();
        
        void reset();
        void processMeasurementData(MeasurementNode measurement[], unsigned int numMeasurements);
    private:
        void uploadMapData(MeasurementNode measurement[], unsigned int numMeasurements) const;

        Map map_;

        bool enableMapUploading_;
    };
}

#endif