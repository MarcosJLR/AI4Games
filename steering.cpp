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

    SteeringOutput Arrive::getSteering()
    {
        SteeringOutput result;

        Vector3 direction = target.position - character.position;
        double distance = direction.norm();
        Vector3 targetVelocity;
        double targetSpeed;

        if(distance < targetRadius)
            return SteeringOutput();

        
        if(distance > slowRadius)
            targetSpeed = maxSpeed;
        else
            targetSpeed = maxSpeed * distance / slowRadius;

        targetVelocity = direction;
        targetVelocity.normalize();
        targetVelocity *= targetSpeed;

        result.linear = targetVelocity - character.velocity;
        result.linear /= timeToTarget;

        if(result.linear.norm() > maxAcceleration){
            result.linear.normalize();
            result.linear *= maxAcceleration;
        }

        result.angular = 0;
        return result;
    }
}