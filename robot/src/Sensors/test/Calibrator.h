#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <map>
#include <vector>

#include "../../Mapper/src/MapperInterface.h"

#define SERVER_URL "192.168.1.172"

namespace LidarCalibrationTest
{
    class Calibrator : public SLAM::MapperInterface
    {
    public:
        Calibrator(double angleMin, double angleMax, double resolution, unsigned long long maxScanCount);
        ~Calibrator();

        void processMeasurementData(SLAM::MeasurementNode measurement[], unsigned int numMeasurements);
    
    private:
        void uploadCalibrationData();

        double angleMin_;
        double angleMax_;
        double resolution_;
        unsigned long long maxScanCount_;
        unsigned long long scanCount_;
        typedef std::map<unsigned int, std::vector<double>> MeasurementMap;
        MeasurementMap measurements_;    
    };
}

LidarCalibrationTest::Calibrator::Calibrator(double angleMin, double angleMax, double resolution, unsigned long long maxScanCount)
{
    angleMin_ = angleMin;
    angleMax_ = angleMax;
    resolution_ = resolution;
    maxScanCount_ = maxScanCount;
    scanCount_ = 0;
    resolution_ = 0.1;
   
    unsigned int angleStart = std::round(angleMin_ * 1.0 / resolution_);
    unsigned int angleStop = std::round(angleMax_ * 1.0 / resolution_);

    for(unsigned int angle = angleStart; angle <= angleStop; angle++)
    {    
        std::vector<double> v;
        std::pair<unsigned int, std::vector<double>> anglePair = std::make_pair(angle, v);
        measurements_.insert(anglePair);
    }
}

LidarCalibrationTest::Calibrator::~Calibrator()
{

}

void LidarCalibrationTest::Calibrator::processMeasurementData(SLAM::MeasurementNode measurement[], unsigned int numMeasurements)
{   
    float angle;

    for(unsigned int measIdx = 0; measIdx < numMeasurements; measIdx++)
    {
        // Round angle
        angle = std::round(measurement[measIdx].angle / resolution_) * resolution_;
        
        if(angle >= angleMin_ && angle <= angleMax_)
        {
            unsigned int angleKey = std::round(angle * 1.0 / resolution_);

            MeasurementMap::iterator it = measurements_.find(angleKey);

            if(it != measurements_.end())
            {
                it->second.push_back(measurement[measIdx].distance);
            } 
            else
            {
                std::cout << "Failed to find angle " << angle << " in map\n";
            }
        }    
    }

    uploadCalibrationData();

    scanCount_++;

    if(scanCount_ >= maxScanCount_)
    {
        std::cout << "Variance Data:\n";

        // Calculate variance
        for(MeasurementMap::const_iterator it = measurements_.begin(); it != measurements_.end(); it++)
        {
            double sum = 0.0;
            double count = it->second.size();
            for(unsigned int i = 0; i < it->second.size(); i++)
            {
                sum += it->second[i];
            }

            double mean = sum / count;

            double diffSum = 0.0;
            for(unsigned int i = 0; i < it->second.size(); i++)
            {
                diffSum += std::pow(it->second[i] - mean, 2);
            }

            double variance = diffSum / count;

            std::cout << "\tAngle " << it->first / (1.0 / resolution_) << ": mean = " << mean << ", variance = " << variance << std::endl;
        }

        std::cout << "Clearing measurements! Scan count " << scanCount_ << "\n";
        
        for(MeasurementMap::iterator it = measurements_.begin(); it != measurements_.end(); it++)
        {
            it->second.clear();
        }

        scanCount_ = 0;
    }
}

void LidarCalibrationTest::Calibrator::uploadCalibrationData()
{
    std::stringstream angleSStream;
    std::stringstream distanceSStream;

    for(MeasurementMap::const_iterator it = measurements_.begin(); it != measurements_.end(); it++)
    {
        for (unsigned int i = 0; i < it->second.size(); i++)
        {
            angleSStream << it->first / (1.0 / resolution_) << ",";
            distanceSStream << it->second[i] << ",";
        }
    }

    // Add dummy element
    angleSStream << "0";
    distanceSStream << "0";

    std::stringstream ss;
    ss << "curl -X POST " << SERVER_URL << ":8080/upload/calibration -H \"Content-Type: application/json\"";
    ss << " -d '{\"angle\": [" << angleSStream.str() << "],";
    ss << " \"distance\": [" << distanceSStream.str() << "],";
    ss << " \"angleMin\": " << angleMin_ << ",";
    ss << " \"angleMax\": " << angleMax_ << ","; 
    ss << " \"scanCount\": " << scanCount_ << ",";
    ss << " \"resolution\": " << resolution_ << "}'";

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

#endif // CALIBRATOR_H
