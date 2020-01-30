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

    SteeringOutput Separation::getSteering()
    {
        SteeringOutput result;

        for(Kinematic& target : targets){
            Vector3 direction = target.position - character.position;
            double distance = direction.norm();

            if(attract)
                direction *= -1;

            if(distance < threshold){
                double strength = std::min(decayCoefficient / (distance * distance), 
                                      maxAcceleration);
                direction.normalize();
                result.linear += strength * direction;
            }
        }

        return result;
    }

    SteeringOutput CollisionAvoidance::getSteering()
    {
        SteeringOutput result;
        double shortestTime = INF;

        double firstMinSeparation;
        double firstDistance;
        Vector3 firstRelativePos;
        Vector3 firstRelativeVel;
        bool willCollide = false;

        for(Kinematic& target : targets){
            Vector3 relativePos = character.position - target.position;
            Vector3 relativeVel = target.velocity - character.velocity;
            double relativeSpeed = relativeVel.norm();
            double timeToColission = (relativePos * relativeVel) / 
                                     (relativeSpeed * relativeSpeed);
            double distance = relativePos.norm();
            double minSeparation = distance - relativeSpeed * timeToColission;
            
            if(minSeparation > 2 * radius)
                continue;

            if(timeToColission > 0 and timeToColission < shortestTime){
                shortestTime = timeToColission;
                firstMinSeparation = minSeparation;
                firstDistance = distance;
                firstRelativePos = relativePos;
                firstRelativeVel = relativeVel;
                willCollide = true;
            }
        }

        if(!willCollide)
            return result;

        Vector3 relativePos;
        if(firstMinSeparation <= 0 or firstDistance < 2 * radius)
            relativePos = firstRelativePos;
        else
            relativePos = firstRelativePos + firstRelativeVel * shortestTime;

        relativePos.normalize();
        result.linear = relativePos * maxAcceleration;
        result.angular = 0;
        return result;
    }
}