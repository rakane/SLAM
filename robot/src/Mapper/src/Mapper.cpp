#include "Mapper.h"

#include <iostream>
#include <sstream>
#include <string>

SLAM::Mapper::Mapper(std::string serverUrl, double uploadInterval, bool enableMapUploading)
    : displayMap_(serverUrl, uploadInterval, enableMapUploading)
{
}

SLAM::Mapper::~Mapper()
{
}

void SLAM::Mapper::processMeasurementData(MeasurementNode measurements[], unsigned int numMeasurements)
{
    std::cout << "Processing measurement data" << std::endl;

    displayMap_.update(measurements, numMeasurements);
}
