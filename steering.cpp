#include "steering.hpp"

namespace aifg
{
    SteeringOutput Seek::getSteering()
    {
        SteeringOutput result;

        result.linear = target->position - character->position;

        result.linear.normalize();
        result.linear *= maxAcceleration;
        if(flee) result.linear *= -1;

        result.angular = 0;
        return result;
    }

    SteeringOutput Arrive::getSteering()
    {
        SteeringOutput result;

        Vector3 direction = target->position - character->position;
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

        result.linear = targetVelocity - character->velocity;
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

        double rotation = minAngularDifference(target->orientation, character->orientation);
        double rotationSize = abs(rotation);

        if(rotationSize < targetRadius)
            return SteeringOutput();


        double targetRotation;
        if(rotationSize > slowRadius)
            targetRotation = maxRotation;
        else
            targetRotation = (maxRotation * rotationSize) / slowRadius;

        targetRotation *= (rotation < 0 ? -1 : 1);

        result.angular = targetRotation - character->rotation;
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

        result.linear = target->velocity - character->velocity;
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
        Vector3 direction = target->position - character->position;
        double distance = direction.norm();

        double speed = character->velocity.norm();
        double prediction;
        
        if(speed <= distance / maxPrediction)
            prediction = maxPrediction;
        else
            prediction = distance / speed;

        Seek::target->position = target->position + target->velocity * prediction;

        return Seek::getSteering();
    }

    SteeringOutput Face::getSteering()
    {
        Vector3 direction = target->position - character->position;
        Align::target->orientation = newOrientation(character->orientation, direction);

        return Align::getSteering();
    }

    SteeringOutput LookWhereYoureGoing::getSteering()
    {
        target->orientation = newOrientation(character->orientation, character->velocity);

        return Align::getSteering();
    }

    SteeringOutput Wander::getSteering()
    {
        wanderOrientation += randomBinomial() * wanderRate;

        target->position = character->position + wanderOffset * (Vector3(character->orientation));

        target->position += wanderRadius * (Vector3(wanderOrientation));

        return Seek::getSteering();
    }

    SteeringOutput Separation::getSteering()
    {
        SteeringOutput result;

        for(Kinematic* target : targets){
            Vector3 direction = target->position - character->position;
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

        for(Kinematic* target : targets){
            Vector3 relativePos = character->position - target->position;
            Vector3 relativeVel = target->velocity - character->velocity;
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

    SteeringOutput ObstacleAvoidance::getSteering()
    {
        Vector3 longRay = character->velocity;
        double orientation = newOrientation(character->orientation, longRay);

        longRay.normalize();
        longRay *= longLookahead;

        Vector3 whiskerL(orientation + whiskerAngle);
        Vector3 whiskerR(orientation - whiskerAngle);

        whiskerL *= shortLookahead;
        whiskerR *= shortLookahead;

        Collision* collision = detector.getCollision(character->position, longRay);

        if(!collision){
            collision = detector.getCollision(character->position, whiskerL);
            
            if(!collision){
                collision = detector.getCollision(character->position, whiskerR);
                
                if(!collision)
                    return SteeringOutput();
            }
        }

        target->position = collision->position + collision->normal * avoidDistance;
        return Seek::getSteering();
    }

    SteeringOutput BlendedSteering::getSteering()
    {
        SteeringOutput result;

        for(BehaviourAndWeight& b : behaviours)
            result += b.weight * b.behaviour->getSteering();

        if(result.linear.norm() > maxAcceleration){
            result.linear.normalize();
            result.linear *= maxAcceleration;
        }
        if(abs(result.angular) > maxRotation)
            result.angular = (result.angular < 0) ? -maxRotation : maxRotation; 
        
        return result;
    }

    SteeringOutput PrioritySteering::getSteering()
    {
        SteeringOutput result;

        for(BlendedSteering* group : groups){
            result = group->getSteering();

            if(result.linear.norm() > EPS or abs(result.angular) > EPS)
                return result; 
        }

        return result;
    }

    SteeringOutput PathFindingSeek::getSteering()
    {
        SteeringOutput result;

        if(character->position.distance(target->position) < 32)
            return result;

        int newCharacterNode = graph->whereAmI(character->position, characterNode);
        int newTargetNode = graph->whereAmI(target->position, targetNode);

        if(targetNode == newTargetNode)
        {
            Seek::target->position = path.empty() ? target->position : path.back();
        }
        else
        {
            path.clear();
            path = graph->Astar(newCharacterNode, newTargetNode);
            reverse(path.begin(), path.end());

            if(path.empty())
                return result;

            Seek::target->position = path.back();
        }

        characterNode = newCharacterNode;
        targetNode = newTargetNode;

        while(!path.empty() && character->position.distance(path.back()) < 32) 
            path.pop_back();

        return Seek::getSteering();
    }
}