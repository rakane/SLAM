#ifndef MAPPER_H
#define MAPPER_H

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <map>
#include <chrono>

#include "MapperInterface.h"

namespace SLAM
{
    class Mapper: public MapperInterface
    {
    public:
        Mapper(double uploadInterval = 0.0, bool enableMapUploading = true);
        ~Mapper();
        
        void processMeasurementData(MeasurementNode measurement[], unsigned int numMeasurements);
    private:
        void uploadMapData(MeasurementNode measurement[], unsigned int numMeasurements) const;

        Map map_;

        bool enableMapUploading_;

        bool throttleUploads_;

        double uploadInterval_;

        std::chrono ::time_point<std::chrono::system_clock> lastUploadTime_;
    };
}

#endif