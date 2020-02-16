#include "position.hpp"

namespace aifg
{
    void Kinematic::update(double t)
    {
        // Update position and orientation
        position += velocity * t;
        orientation += rotation * t;

        // Wrapping orientation around (-pi, pi)
        if(orientation > M_PI)
            orientation -= 2*M_PI;
    }

    void Kinematic::update(SteeringOutput steering, double maxSpeed, double maxRotation, double t)
    {
        // Update position and orientation
        position += velocity * t;
        orientation += rotation * t;

        // Wrapping orientation around (-pi, pi)
        if(orientation > M_PI)
            orientation -= 2*M_PI;

        // Update velocity and rotation
        velocity += steering.linear * t;
        rotation += steering.angular * t;

        // Drag
        velocity *= drag;
        rotation *= drag;

        if (velocity.norm() > maxSpeed) {
            velocity.normalize();
            velocity *= maxSpeed;
        }

        if(abs(rotation) > maxRotation)
            rotation = (rotation < 0 ? -1 : 1) * maxRotation;
        
        if(velocity.norm() < EPS)
            velocity = {0,0,0};

        if(abs(rotation) < EPS/2)
            rotation = 0;

        //orientation = newOrientation(orientation, steering.linear);
    }

    void Kinematic::update(KSteeringOutput steering, double t)
    {
        // Update position and orientation
        position += steering.velocity * t;
        // orientation += steering.rotation * t; 
        orientation = newOrientation(orientation, steering.velocity);
    }

    double newOrientation(double current, Vector3 velocity)
    {
        if (velocity.norm() > 0) {
            return atan2(-velocity.x, velocity.z) + M_PI/2;
        }
        return current;
    }

    double minAngularDifference(double alfa, double beta)
    {
        double result = alfa - beta;

        if(result < -M_PI - EPS) 
            result += 2*M_PI;
        if(result > M_PI + EPS)
            result -= 2*M_PI;

        return result;
    }

    double randomBinomial(){
        double rand1 = rand() / (double) RAND_MAX;
        double rand2 = rand() / (double) RAND_MAX;
           
        return rand1 - rand2;
    }
};