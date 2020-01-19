#include "kinematic.hpp"

namespace aifg
{
    KSteeringOutput KSeek::getSteering()
    {
        KSteeringOutput result;

        result.velocity = target.position - character.position;

        if(flee) result.velocity *= -1;

        result.velocity.normalize();
        result.velocity *= maxSpeed;

        result.rotation = 0;
        return result;
    }

    KSteeringOutput KArrive::getSteering()
    {
        KSteeringOutput result;

        result.velocity = target.position - character.position;

        if(result.velocity.norm() < radius) 
            return KSteeringOutput();

        result.velocity /= timeToTarget;
        if(result.velocity.norm() > maxSpeed){
            result.velocity.normalize();
            result.velocity *= maxSpeed;
        }

        return result;
    }
};