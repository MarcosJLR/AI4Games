/* 
 * Classes required to store position of an 
 * object/character in the game
 *
 * Author: Marcos Lerones
 */
#pragma once 

#include "vector3D.hpp"

namespace aifg
{
    struct SteeringOutput
    {
        // Accelerations
        Vector3 linear;
        double angular;

        SteeringOutput() : linear(), angular(0) {}
        SteeringOutput(const Vector3& linear, double angular = 0) 
            : linear(linear), angular(angular) {}
    };
    
    struct KSteeringOutput
    {
        // Linear and angular velocity
        Vector3 velocity;
        double rotation;

        KSteeringOutput() : velocity(), rotation(0) {}
        KSteeringOutput(const Vector3& velocity, double rotation = 0)
            : velocity(velocity), rotation(rotation) {}
    };

    struct Kinematic
    {
        // Static info
        Vector3 position;
        double orientation;

        // Kinematic info
        Vector3 velocity;
        double rotation;

        // Default constructor
        Kinematic() 
            : position(), orientation(0), velocity(), rotation(0) {}

        // Constructor with position and orientation
        // sets kinematic info to 0
        Kinematic(const Vector3& position, double orientation) 
            : position(position), orientation(orientation), velocity(), rotation(0) {}
        
        // Constructor with everything specified
        Kinematic(const Vector3& position, double orientation, 
                  const Vector3& velocity, double rotation)
            : position(position), orientation(orientation), 
              velocity(velocity), rotation(rotation) {}

        // Update the position according to kinematic info for t seconds
        void update(double t);

        // Update the position with steering output
        void update(SteeringOutput steering, double maxSpeed, double t);
    
        // Update kinematic info
        void update(KSteeringOutput steering, double t);
    };
};
