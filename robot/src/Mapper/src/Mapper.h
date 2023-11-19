#ifndef MAPPER_H
#define MAPPER_H

#include <string>

#include "MapperInterface.h"
#include "DisplayMap.h"

namespace SLAM
{
    class Mapper: public MapperInterface
    {
    public:
        Mapper(std::string serverUrl, double uploadInterval = 0.0, bool enableMapUploading = true);
        ~Mapper();
        
        void processMeasurementData(MeasurementNode measurements[], unsigned int numMeasurements);
    private:
        
        DisplayMap displayMap_; 
    };
}

#endif
