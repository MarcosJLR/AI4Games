#include "collision.hpp"

namespace aifg
{
    bool Segment::properIntersection(Segment& S, Segment& T)
    {
        double osa = S.A.orient2D(S.B, T.A),
               osb = S.A.orient2D(S.B, T.B),
               ota = T.A.orient2D(T.B, S.A),
               otb = T.A.orient2D(T.B, S.B);

        return osa*osb < 0 && ota*otb < 0;
    }

    Vector3 Segment::intersect(Segment& S, Segment& T)
    {
        double osa = S.A.orient2D(S.B, T.A),
               osb = S.A.orient2D(S.B, T.B);

        Vector3 result = ((S.A * osb) - (S.B * osa)) / (osb - osa); 
    
        return result;
    }

    Collision* CollisionDetector::getCollision(Vector3& position, Vector3& moveAmount)
    {
        Collision* result = NULL;
        Segment move(position, position + moveAmount);
        double distance;

        for(Segment& obstacle : obstacles){
            if(!Segment::properIntersection(obstacle, move))
                continue;

            Collision* collision = new Collision(obstacle, move);
            double newDistance = position.distance(collision->position);

            if(!result || newDistance < distance){
                distance = newDistance;
                std::swap(result, collision);
            }

            if(collision) 
                free(collision);
        }

        return result;
    }
}