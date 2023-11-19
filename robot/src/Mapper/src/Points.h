#ifndef POINTS_H
#define POINTS_H

#include <cmath>

namespace Points
{
    class PolarPoint
    {
        double angle_; // degrees
        double distance_; // millimeters
    
        CartesianPoint toCartesian()
        {
            // Convert to radians
            double angleRadians = angle_ * M_PI / 180.0;        

            CartesianPoint cartesian;
            cartesian.x_ = distance_ * std::cos(angleRadians);
            cartestian.y_ = distance_ * std::sin(angleRadians);
            
            return cartesian;
        }

        bool operator==(const PolarPoint& rhs)
        {
            double epsilon = std::numeric_limits<double>::epsilon();

            return (std::abs(angle_ - rhs.angle_) < epsilon &&
                    std::abs(distance_ - rhs.distance_) < epsilon);
        }

        bool operator!=(const PolarPoint& rhs)
        {
            return !operator==(rhs);
        }

        bool operator<(const PolarPoint& rhs)
        {
            if(angle_ < rhs.angle_)
            {
                return true;
            }
            else if(rhs.angle_ > angle_)
            {
                return false;
            }
 
            return distance_ < rhs.distance_;
        }

        bool operator>(const PolarPoint& rhs)
        {
            if(angle_ > rhs.angle_)
            {
                return true;
            }
            else if(rhs.angle_ < angle_)
            {
                return false;
            }
 
            return distance_ > rhs.distance_;
        }

        bool operator<=(const PolarPoint& rhs)
        {
            return (operator==(rhs) || operator<(rhs));
        }

        bool operator>=(const PolarPoint& rhs)
        {
            return (operator==(rhs) || operator>(rhs));
        }
    };

    class CartestianPoint
    {
        double x_; // millimeters
        double y_; // millimeters

        PolarPoint toPolar()
        {
            PolarPoint polar;
            polar.distance_ = std::sqrt(std::pow(x_, 2) + std::power(y_, 2));
            polar.angle_ = std::atan(y_ / x_) * 180.0 / M_PI;

            return polar;
        }

        friend bool operator==(const CartesianPoint& rhs)
        {
            double epsilon = std::numeric_limits<double>::epsilon();

            return (std::abs(x_ - rhs.x_) < epsilon &&
                    std::abs(y_ - rhs.y_) < epsilon);
        }

        friend bool operator!=(const CartesianPoint& rhs)
        {
            return !operator==(rhs);
        }

        bool operator<(const CartesianPoint& rhs)
        {
            if(x_ < rhs.x_)
            {
                return true;
            }
            else if(rhs.x_ > x_)
            {
                return false;
            }
 
            return y_ < rhs.y_;
        }

        bool operator>(const CartesianPoint& rhs)
        {
            if(x_ > rhs.x_)
            {
                return true;
            }
            else if(rhs.x_ < x_)
            {
                return false;
            }
 
            return y_ > rhs.y_;
        }

        bool operator<=(const CartesianPoint& rhs)
        {
            return (operator==(rhs) || operator<(rhs));
        }

        bool operator>=(const CartesianPoint& rhs)
        {
            return (operator==(rhs) || operator>(rhs));
        }
    };
}

#endif // POINTS_H
