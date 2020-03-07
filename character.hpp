#include <math.h>

#include "position.hpp"
#include "steering.hpp"
#include "texture.hpp"
#include "graph.hpp" 
#include "collision.hpp"

namespace aifg
{
    struct Character
    {
        Kinematic kinematic;
        Behaviour* behaviour;
        CollisionDetector* detector;
        LTexture sprite;
        double maxSpeed;
        double maxRotation;
        const double maxAcceleration = 0.001;
        const double maxAngular = 0.00005;

        void update(double t);
        void draw(SDL_Renderer* renderer);
    };

    struct Player : Character
    {
        SteeringOutput userSteering;

        void init(const Vector3& pos, LTexture& mSprite);
        void update(double t);
    };

    struct RedEnemy : Character
    {
        void init(const Vector3& pos, Kinematic* player, LTexture& mSprite, std::vector<Kinematic*>& enemies, CollisionDetector* mDetector);
    };

    struct RedEnemy2 : Character
    {
        void init(const Vector3& pos, Kinematic* player, LTexture& mSprite, Graph* graph);
    };
};