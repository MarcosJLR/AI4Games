#pragma once

#include "vector3D.hpp"
#include "position.hpp"

namespace aifg
{
    struct KSeek
    {
        Kinematic character;
        Kinematic target; 

        double maxSpeed;

        bool flee;

        KSeek() : character(), target(), maxSpeed(0), flee(false) {}
        KSeek(Kinematic& character, Kinematic& target, double maxSpeed)
            : character(character), target(target), maxSpeed(maxSpeed), flee(false) {}
        KSeek(Kinematic& character, Kinematic& target, double maxSpeed, bool flee)
            : character(character), target(target), maxSpeed(maxSpeed), flee(flee) {}

        KSteeringOutput getSteering();
    };

    struct KArrive
    {
        Kinematic character;
        Kinematic target;

        double maxSpeed;
        double radius;
        const double timeToTarget = 0.25;

        KArrive() : character(), target(), maxSpeed(0), radius(0) {}
        KArrive(Kinematic& character, Kinematic& target, double maxSpeed, double radius)
            : character(character), target(target), maxSpeed(maxSpeed), radius(radius) {}

        KSteeringOutput getSteering();
    };
};