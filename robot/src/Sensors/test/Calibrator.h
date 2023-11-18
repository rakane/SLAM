#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include "../../Mapper/src/MapperInterface.h"

namespace LidarCalibrationTest
{
    class Calibrator : public SLAM::MapperInterface
    {
    public:
        Calibrator();
        ~Calibrator();

        void processMeasurementData(SLAM::MeasurementNode measurement[], unsigned int numMeasurements);
    
        private:
            double angleMin_;
            double angleMax_;
            double resolution_;

        std::vector<double, double> measurements_;    
    };
}

LidarCalibrationTest::Calibrator::Calibrator(double angleMin, double angleMax, double resolution)
{
    angleMin_ = angleMin;
    angleMax_ = angleMax;
    resolution_ = resolution;

    unsigned long long NUM_MEASUREMENTS = (angleMax_ - angleMin_) / resolution_;
    measurements_.reserve(NUM_MEASUREMENTS);
}

LidarCalibrationTest::Calibrator::~Calibrator()
{

}

void LidarCalibrationTest::Calibrator::processMeasurementData(SLAM::MeasurementNode measurement[], unsigned int numMeasurements)
{   
    ,
}

#endif // CALIBRATOR_H
