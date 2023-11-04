#include "Map.h"

SLAM::Map::Map(): map_()
{
    resetMap();
}

SLAM::Map::~Map()
{
}

void SLAM::Map::getMapPoint(float angle, float& distance)
{
    distance = map_[angleToIndex(angle)];
}

void SLAM::Map::setMapPoint(float angle, float distance)
{
    map_[angleToIndex(angle)] = distance;
}

void SLAM::Map::resetMap()
{
    for (unsigned int i = 0; i < MAX_MAP_POINTS; i++)
    {
        map_[i] = 0.0f;
    }
}

unsigned int SLAM::Map::angleToIndex(float angle)
{
    // Convert angle to range 0 - 360
    while (angle < 0.0f)
    {
        angle += 360.0f;
    }
    while (angle >= 360.0f)
    {
        angle -= 360.0f;
    }

    // Convert angle to index
    return (unsigned int)(angle * 10.0f);
}

