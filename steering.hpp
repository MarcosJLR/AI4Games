#pragma once

#include "vector3D.hpp"
#include "position.hpp"

namespace aifg
{
    // Behaviour Abstract class
    struct Behaviour
    {
        virtual SteeringOutput getSteering() = 0;
    };

    struct Seek : Behaviour
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

    struct Arrive : Behaviour
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

    struct Align : Behaviour
    {
        Kinematic character;
        Kinematic target;

        double maxAngular;
        double maxRotation;

        double targetRadius;
        double slowRadius;

        double timeToTarget = 0.1;

        Align() : character(), target(), maxAngular(0), maxRotation(0), targetRadius(0), slowRadius(0) {}
        Align(Kinematic character, Kinematic target, double maxAng, double maxRot, double targetRadius, double slowRadius)
            : character(character), target(target), maxAngular(maxAng), 
              maxRotation(maxRot), targetRadius(targetRadius), slowRadius(slowRadius) {}

        SteeringOutput getSteering(); 
    };
};