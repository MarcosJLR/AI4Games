/* 
 * Classes required to store position of an 
 * object/character in the game
 *
 * Author: Marcos Lerones
 */
#pragma once 

#include <random>
#include "vector3D.hpp"

#define EPS 0.01

namespace aifg
{
    const double INF = 1.0/0.0;

    struct SteeringOutput
    {
        // Accelerations
        Vector3 linear;
        double angular;

        SteeringOutput() : linear(), angular(0) {}
        SteeringOutput(const Vector3& linear, double angular = 0) 
            : linear(linear), angular(angular) {}

        // Addition
        inline SteeringOutput& operator+=(const SteeringOutput& rhs)
        {
            linear += rhs.linear;
            angular += rhs.angular;
            return *this;
        }
        inline friend SteeringOutput operator+(SteeringOutput lhs, const SteeringOutput& rhs)
        {
            lhs += rhs;
            return lhs;
        }

        // Substraction
        inline SteeringOutput& operator-=(const SteeringOutput& rhs)
        {
            linear -= rhs.linear;
            angular -= rhs.angular;
            return *this;
        }
        inline friend SteeringOutput operator-(SteeringOutput lhs, const SteeringOutput& rhs)
        {
            lhs -= rhs;
            return lhs;
        }

        // Multiplication by a scalar
        inline SteeringOutput& operator*=(const double rhs)
        {
            linear *= rhs;
            angular *= rhs;
            return *this;
        }
        inline friend SteeringOutput operator*(SteeringOutput lhs, const double rhs)
        {
            lhs *= rhs;
            return lhs;
        }
        inline friend SteeringOutput operator*(const double lhs, SteeringOutput rhs)
        {
            rhs *= lhs;
            return rhs;
        }

        // Division by a scalar
        inline SteeringOutput& operator/=(const double rhs)
        {
            linear /= rhs;
            angular /= rhs;
            return *this;
        }
        inline friend SteeringOutput operator/(SteeringOutput lhs, const double rhs)
        {
            lhs /= rhs;
            return lhs;
        }
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

        const double drag = 0.05;

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
        void update(SteeringOutput steering, double maxSpeed, double maxRotation, double t);
    
        // Update kinematic info
        void update(KSteeringOutput steering, double t);
    };

    double newOrientation(double current, Vector3 velocity);

    double minAngularDifference(double alfa, double beta);

    double randomBinomial();
};
