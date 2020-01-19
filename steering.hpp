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
};