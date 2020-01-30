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

    SteeringOutput Align::getSteering()
    {
        SteeringOutput result;

        double rotation = minAngularDifference(target.orientation, character.orientation);
        double rotationSize = abs(rotation);

        if(rotationSize < targetRadius)
            return SteeringOutput();

        double targetRotation;
        if(rotation > slowRadius)
            targetRotation = maxRotation;
        else
            targetRotation = maxRotation * rotationSize / slowRadius;

        targetRotation *= (rotation < 0 ? -1 : 1);

        result.angular = targetRotation - character.rotation;
        result.angular /= timeToTarget;

        double angularSize = abs(result.angular);
        if(angularSize > maxAngular){
            result.angular /= angularSize;
            result.angular *= maxAngular;
        } 

        result.linear = Vector3::ZEROV;
        return result;
    }

    SteeringOutput VelocityMatch::getSteering()
    {
        SteeringOutput result;

        result.linear = target.velocity - character.velocity;
        result.linear /= timeToTarget;

        if(result.linear.norm() > maxAcceleration){
            result.linear.normalize();
            result.linear *= maxAcceleration;
        }

        result.angular = 0;
        return result;
    }

    SteeringOutput Pursue::getSteering()
    {
        Vector3 direction = target.position - character.position;
        double distance = direction.norm();

        double speed = character.velocity.norm();
        double prediction;
        
        if(speed <= distance / maxPrediction)
            prediction = maxPrediction;
        else
            prediction = distance / speed;

        Seek::target.position = target.position + target.velocity * prediction;

        return Seek::getSteering();
    }

    SteeringOutput Face::getSteering()
    {
        Vector3 direction = target.position - character.position;
        Align::target.orientation = newOrientation(character.orientation, direction);

        return Align::getSteering();
    }

    SteeringOutput LookWhereYoureGoing::getSteering()
    {
        target.orientation = newOrientation(character.orientation, character.velocity);

        return Align::getSteering();
    }

    SteeringOutput Wander::getSteering()
    {
        wanderOrientation += randomBinomial() * wanderRate;

        target.position = character.position + wanderOffset * (Vector3(character.orientation));

        target.position += wanderRadius * (Vector3(wanderOrientation));

        return Seek::getSteering();
    }
}