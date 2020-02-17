/* 
 * Classes required to handle collisions
 *
 * Author: Marcos Lerones
 */

#pragma once

#include <vector>
#include "vector3D.hpp"

namespace aifg
{
    struct Segment
    {
        Vector3 A, B, N;

        Segment() : A(), B(), N() {}
        Segment(Vector3 A, Vector3 B) : A(A), B(B) 
        {
            N.x = (A - B).z;
            N.y = 0;
            N.z = (B - A).x;
            N.normalize();
        }

        static bool properIntersection(Segment& S, Segment& T);
        static Vector3 intersect(Segment& S, Segment& T);
    };

    struct Collision
    {
        Vector3 position;
        Vector3 normal;

        Collision() : position(), normal() {}
        Collision(Segment& S, Segment& T) 
            : position(Segment::intersect(S, T)), normal(S.N) {}

    };

    struct CollisionDetector
    {
        std::vector<Segment> obstacles;

        CollisionDetector() : obstacles() {}
        CollisionDetector(std::vector<Segment>& obstacles) : obstacles(obstacles) {}

        inline void add(Segment S) { obstacles.push_back(S); }
        inline void clear() { obstacles.clear(); }

        Collision* getCollision(Vector3& position, Vector3& moveAmount);
    };
};