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
        Kinematic& character;
        Kinematic& target;

        double maxAcceleration;

        bool flee;

        Seek() : character(*(new Kinematic())), target(*(new Kinematic())), maxAcceleration(0), flee(false) {}
        Seek(Kinematic& character, Kinematic& target, double maxAcc)
            : character(character), target(target), maxAcceleration(maxAcc), flee(false) {}
        Seek(Kinematic& character, Kinematic& target, double maxAcc, bool flee)
            : character(character), target(target), maxAcceleration(maxAcc), flee(flee) {}

        SteeringOutput getSteering();
    };

    struct Arrive : Behaviour
    {
        Kinematic& character;
        Kinematic& target;

        double maxAcceleration;
        double maxSpeed;

        double targetRadius;
        double slowRadius;

        double timeToTarget = 0.1;

        Arrive() : character(*(new Kinematic())), target(*(new Kinematic())), maxAcceleration(0), targetRadius(0), slowRadius(0) {}
        Arrive(Kinematic& character, Kinematic& target, double maxAcc, double maxSpeed, double targetRadius, double slowRadius)
            : character(character), target(target), maxAcceleration(maxAcc),
              maxSpeed(maxSpeed), targetRadius(targetRadius), slowRadius(slowRadius) {}

        SteeringOutput getSteering();
    };

    struct Align : Behaviour
    {
        Kinematic& character;
        Kinematic& target;

        double maxAngular;
        double maxRotation;

        double targetRadius;
        double slowRadius;

        double timeToTarget = 0.1;

        Align() : character(*(new Kinematic())), target(*(new Kinematic())), maxAngular(0), maxRotation(0), targetRadius(0), slowRadius(0) {}
        Align(Kinematic& character, Kinematic& target, double maxAng, double maxRot, double targetRadius, double slowRadius)
            : character(character), target(target), maxAngular(maxAng), 
              maxRotation(maxRot), targetRadius(targetRadius), slowRadius(slowRadius) {}

        SteeringOutput getSteering(); 
    };

    struct VelocityMatch : Behaviour
    {
        Kinematic& character;
        Kinematic& target;

        double maxAcceleration;

        double timeToTarget = 0.1;

        VelocityMatch() : character(*(new Kinematic())), target(*(new Kinematic())), maxAcceleration(0) {}
        VelocityMatch(Kinematic& character, Kinematic& target, double maxAcc)
            : character(character), target(target), maxAcceleration(maxAcc) {}

        SteeringOutput getSteering();
    };

    struct Pursue : Seek
    {
        Kinematic& target;
        double maxPrediction;

        Pursue() : Seek(), target(*(new Kinematic())), maxPrediction(0) {}
        Pursue(Kinematic& character, Kinematic& target, double maxAcc, double maxPred)
            : Seek(character, *(new Kinematic()), maxAcc), target(target), maxPrediction(maxPred) {}
        Pursue(Kinematic& character, Kinematic& target, double maxAcc, double maxPred, bool evade)
            : Seek(character, *(new Kinematic()), maxAcc, evade), target(target), maxPrediction(maxPred) {}

        SteeringOutput getSteering();
    };

    struct Face : Align
    {
        Kinematic& target;

        Face() : Align(), target(*(new Kinematic())) {}
        Face(Kinematic& character, Kinematic& target, double maxAng, double maxRot, double targetRadius, double slowRadius)
            : Align(character, *(new Kinematic()), maxAng, maxRot, targetRadius, slowRadius), target(target) {}

        SteeringOutput getSteering();
    };

    struct LookWhereYoureGoing : Align
    {
        LookWhereYoureGoing() : Align() {}
        LookWhereYoureGoing(Kinematic& character, double maxAng, double maxRot, double targetRadius, double slowRadius)
            : Align(character, *(new Kinematic()), maxAng, maxRot, targetRadius, slowRadius) {}

        SteeringOutput getSteering();
    };

    struct Wander : Seek
    {
        double wanderOffset;
        double wanderRadius;
        double wanderRate;
        double wanderOrientation;

        Wander() 
            : Seek(), wanderOffset(0), wanderRadius(0), wanderRate(0),
              wanderOrientation(0) {}
        Wander(Kinematic& character, double maxAcc, double wanderOffset, double wanderRadius,
               double wanderRate, double wanderOrientation)
            : Seek(character, *(new Kinematic()), maxAcc), wanderOffset(wanderOffset),
              wanderRadius(wanderRadius), wanderRate(wanderRate), wanderOrientation(character.orientation) {}

        SteeringOutput getSteering();
    };
};