#ifndef MAPPER_INTERFACE_H
#define MAPPER_INTERFACE_H

namespace SLAM
{
    struct MeasurementNode 
    {
        float angle; // degrees
        float distance; // millimeters
        float distanceVariance; // millimeters
    };

    class MapperInterface
    {
    public:
        MapperInterface() = default;
        virtual ~MapperInterface() = default;

        virtual void processMeasurementData(MeasurementNode measurement[], unsigned int numMeasurements) = 0;
    };
}

#endif // MAPPER_INTERFACE_H
