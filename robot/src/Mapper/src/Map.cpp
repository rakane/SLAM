#include <vector>


#include "Map.h"

SLAM::Map::Map(): polarMap_(), cartesianMap_()
{
    // Fill in map with default values
    std::cout << "Initializing map..." << std::endl;

    // Polar map, set all angles to MAX_POLAR_DISTANCE
    for (double i = 0; i < NUM_POLAR_ANGLES; i++)
    {
        double angle = i * ANGLE_RESOLUTION;

        std::pair<double, double> polarPair = std::make_pair(angle, MAX_POLAR_DISTANCE);
        polarMap_.insert(polarPair);
    }

    // Cartesian map, set all points to false
    for (int x = -MAX_X_SIZE; x < MAX_X_SIZE; x += CARTESTIAN_MAP_RESOLUTION)
    {
        for (int y = -MAX_Y_SIZE; y < MAX_Y_SIZE; y += CARTESTIAN_MAP_RESOLUTION)
        {
            CartesianPoint point = std::make_pair(x, y);
            std::pair<CartesianPoint, bool> cartesianPair = std::make_pair(point, false);
            cartesianMap_.insert(cartesianPair);
        }
    }
}

SLAM::Map::~Map()
{
}

const SLAM::PolarMap& SLAM::Map::getPolarMap() const
{
    return polarMap_;
}

const SLAM::CartesianMap& SLAM::Map::getCartesianMap() const
{
    return cartesianMap_;
}

double SLAM::Map::getPolarPoint(double angle) const
{
    // Ensure angle is rounded to nearest ANGLE_RESOLUTION
    double roundedAngle = std::round(angle / ANGLE_RESOLUTION) * ANGLE_RESOLUTION;

    PolarMap::const_iterator it = polarMap_.find(roundedAngle);
    if (it != polarMap_.end())
    {
        return it->second;
    }
    else
    {
        return MAX_POLAR_DISTANCE;
    }
}

bool SLAM::Map::getCartesianPoint(double x, double y) const
{
    // Ensure x and y are rounded to nearest CARTESTIAN_MAP_RESOLUTION
    int roundedX = std::round(x / CARTESTIAN_MAP_RESOLUTION) * CARTESTIAN_MAP_RESOLUTION;
    int roundedY = std::round(y / CARTESTIAN_MAP_RESOLUTION) * CARTESTIAN_MAP_RESOLUTION;

    CartesianPoint point = std::make_pair(roundedX, roundedY);
    CartesianMap::const_iterator it = cartesianMap_.find(point); // findCartesianPoint(point);

    if (it != cartesianMap_.end())
    {
        return it->second;
    }
    else
    {
        return false;
    }
}

bool SLAM::Map::updateMap(SLAM::MeasurementNode measurement)
{
    // Measurement already in polar coordinates, round angle to nearest ANGLE_RESOLUTION
    double roundedAngle = std::round(measurement.angle / ANGLE_RESOLUTION) * ANGLE_RESOLUTION;

    // Ensure angle is between 0 and 360
    if(roundedAngle >= 360.0)
    {
        roundedAngle -= 360.0;
    }

    PolarMap::iterator it = polarMap_.find(roundedAngle);
    if(it != polarMap_.end())
    {
        it->second = measurement.distance;
    }
    else
    {
        std::cout << "Angle not found in polar map! angle: " << roundedAngle << std::endl;
    }

    // Convert to all cartesian points along the line from the robot to the end of the map,
    // then check if there are any points marked true on the map, if so, mark them as false since
    // they did not get updated by the lidar. Then set the updated point to true below.

    // Build list of all distance from 0 to measurement.distance, at a step of CARTESTIAN_MAP_RESOLUTION / 2
    std::vector<double> distances;
    distances.reserve(measurement.distance / (CARTESTIAN_MAP_RESOLUTION / 2.0));
    double stepSize = CARTESTIAN_MAP_RESOLUTION / 2.0;
    for (double i = 0; i < measurement.distance; i += stepSize)
    {
        distances.push_back(i);
    }

    // Convert to cartesian coordinates for each distance
    std::vector<CartesianPoint> points;
    points.reserve(distances.size());

    double x, y;
    int roundedX, roundedY;
    for(unsigned int distanceIdx = 0; distanceIdx < distances.size(); distanceIdx++)
    {
        x = distances[distanceIdx] * std::cos(measurement.angle * M_PI / 180.0);
        y = distances[distanceIdx] * std::sin(measurement.angle * M_PI / 180.0);
        roundedX = std::round(x / CARTESTIAN_MAP_RESOLUTION) * CARTESTIAN_MAP_RESOLUTION;
        roundedY = std::round(y / CARTESTIAN_MAP_RESOLUTION) * CARTESTIAN_MAP_RESOLUTION;

        points.emplace_back(roundedX, roundedY);
    }

    // Set all points to false in cartesian map, as they did not get updated by the lidar
    for(unsigned int pointIdx = 0; pointIdx < points.size(); pointIdx++)
    {
        CartesianMap::iterator it2 = cartesianMap_.find(points[pointIdx]);
        if(it2 != cartesianMap_.end())
        {
            it2->second = false;
        } 
        else
        {
            std::cout << "Position not found in cartesian map! x: " << points[pointIdx].first << " y: " << points[pointIdx].second << std::endl;
        }
    }

    // Convert to cartesian coordinates
    x = measurement.distance * std::cos(measurement.angle * M_PI / 180.0);
    y = measurement.distance * std::sin(measurement.angle * M_PI / 180.0);

    // Round x and y to nearest CARTESTIAN_MAP_RESOLUTION
    roundedX = std::round(x / static_cast<double>(CARTESTIAN_MAP_RESOLUTION)) * CARTESTIAN_MAP_RESOLUTION;
    roundedY = std::round(y / static_cast<double>(CARTESTIAN_MAP_RESOLUTION)) * CARTESTIAN_MAP_RESOLUTION;

    CartesianPoint point = std::make_pair(roundedX, roundedY);
    CartesianMap::iterator it2 = cartesianMap_.find(point);

    if(it2 != cartesianMap_.end())
    {
        it2->second = true;
    }
    else
    {
        std::cout << "Position not found in cartesian map! x: " << roundedX << " y: " << roundedY << std::endl;
    }

    return true;
}

void SLAM::Map::resetMap()
{
    polarMap_.clear();
    cartesianMap_.clear();
}
