#include "character.hpp"

namespace aifg
{
    void Character::update(double t)
    {
        kinematic.update(behaviour->getSteering(), maxSpeed, maxRotation, t);
    } 

    void Character::draw(SDL_Renderer* renderer)
    {
        sprite.render(renderer, kinematic.position.x, kinematic.position.z,
                      NULL, (kinematic.orientation * 180) / M_PI, 1 + kinematic.position.y);
    }

    void Player::update(double t)
    {
        kinematic.update(behaviour->getSteering() + userSteering, maxSpeed, maxRotation, t);
    }

    void Player::init(const Vector3& pos, LTexture& mSprite)
    {
        kinematic.position = pos;
        // sprite.loadFromFile(renderer, "img/Player.png");
        sprite = mSprite;
        maxSpeed = 0.5;
        maxRotation = (0.5 * M_PI) / 360;
        behaviour = new LookWhereYoureGoing(&kinematic, maxAngular, maxRotation);
    }

    void RedEnemy::init(const Vector3& pos, Kinematic* player, LTexture& mSprite, std::vector<Kinematic*>& enemies, CollisionDetector* mDetector)
    {
        kinematic.position = pos;
        // sprite.loadFromFile(renderer, "img/RedEnemy.png");        

        maxSpeed = 0.25;
        maxRotation = (0.5 * M_PI) / 360;
        behaviour = new BlendedSteering(maxAcceleration, maxAngular);
        sprite = mSprite;
        detector = mDetector;
        BlendedSteering* blendBehaviour = (BlendedSteering*) behaviour;
        PrioritySteering* pBehaviours = new PrioritySteering();

        pBehaviours->addBehaviour(new ObstacleAvoidance(&kinematic, *detector, 100*maxAcceleration, 30, 50, 20, M_PI / 4), 100*maxAcceleration, maxAngular);
        pBehaviours->addBehaviour(new CollisionAvoidance(&kinematic, enemies, maxAcceleration, 30), 2*maxAcceleration, maxAngular);
        pBehaviours->addBehaviour(new Pursue(&kinematic, player, maxAcceleration, 1), maxAcceleration, maxAngular);
        blendBehaviour->addBehaviour(new LookWhereYoureGoing(&kinematic, maxAngular, maxRotation), 1);
        blendBehaviour->addBehaviour(pBehaviours, 1);

        enemies.push_back(&kinematic);
    }
};