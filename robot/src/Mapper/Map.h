#include <iostream>
#include <iomanip>
#include <stdio.h>

#ifndef MAP_H
#define MAP_H

namespace SLAM
{
    // Map points, with an angle resolution of 0.1 degrees
    const unsigned int MAX_MAP_POINTS = 360 * 10;

    class Map
    {
    public:
        Map();
        ~Map();
        
        // Getters
        void getMapPoint(float angle, float& distance);
        // Setters
        void setMapPoint(float angle, float distance);

        void resetMap();
    private:
        // Coverts angle to index in map
        unsigned int angleToIndex(float angle);

        // Map point distances, no need to store angle,
        // as it can be calculated based on the index
        float map_[MAX_MAP_POINTS];
    };
}

#endif