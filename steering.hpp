#pragma once

#include "vector3D.hpp"
#include "position.hpp"

namespace aifg
{
    struct Seek
    {
        Kinematic character;
        Kinematic target;

        double maxAcceleration;

        bool flee;

        Seek() : character(), target(), maxAcceleration(0), flee(false) {}
        Seek(Kinematic& character, Kinematic& target, double maxAcc)
            : character(character), target(target), maxAcceleration(maxAcc), flee(false) {}
        Seek(Kinematic& character, Kinematic& target, double maxAcc, bool flee)
            : character(character), target(target), maxAcceleration(maxAcc), flee(flee) {}

        SteeringOutput getSteering();
    };

    struct Arrive
    {
        Kinematic character;
        Kinematic target;

        double maxAcceleration;
        double maxSpeed;

        double targetRadius;
        double slowRadius;

        double timeToTarget = 0.1;

        Arrive() : character(), target(), maxAcceleration(0), targetRadius(0), slowRadius(0) {}
        Arrive(Kinematic character, Kinematic target, double maxAcc, double maxSpeed, double targetRadius, double slowRadius)
            : character(character), target(target), maxAcceleration(maxAcc),
              maxSpeed(maxSpeed), targetRadius(targetRadius), slowRadius(slowRadius) {}

        SteeringOutput getSteering();
    };
};