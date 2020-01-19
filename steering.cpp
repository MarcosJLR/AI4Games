#include "steering.hpp"

namespace aifg
{
    SteeringOutput Seek::getSteering()
    {
        SteeringOutput result;

        result.linear = target.position - character.position;

        result.linear.normalize();
        result.linear *= maxAcceleration;
        if(flee) result.linear *= -1;

        result.angular = 0;
        return result;
    }
}