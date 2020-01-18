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

        if (velocity.norm() > maxSpeed) {
            velocity.normalize();
            velocity *= maxSpeed;
        }
    }

    void Kinematic::update(KSteeringOutput steering, double t)
    {
        // Update position and orientation
        position += steering.velocity * t;
        orientation += steering.rotation * t;        
    }
};