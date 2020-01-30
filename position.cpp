#include "position.hpp"

namespace aifg
{
    void Kinematic::update(double t)
    {
        // Update position and orientation
        position += velocity * t;
        orientation += rotation * t;
    }

    void Kinematic::update(SteeringOutput steering, double maxSpeed, double t)
    {
        // Update position and orientation
        position += velocity * t;
        orientation += rotation * t;

        // Update velocity and rotation
        velocity += steering.linear * t;
        rotation += steering.angular * t;

        // Drag
        velocity -= velocity*drag;

        if (velocity.norm() > maxSpeed) {
            velocity.normalize();
            velocity *= maxSpeed;
        }
        
        if(velocity.norm() < EPS)
            velocity = {0,0,0};


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
};