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

    void Player::init(LTexture& mSprite)
    {
        maxSpeed = 0.5;
        maxRotation = (0.5 * M_PI) / 360;
        sprite = mSprite;
        behaviour = new LookWhereYoureGoing(kinematic, maxAngular, maxRotation);
    }

    void RedEnemy::init(Kinematic& player, LTexture& mSprite)
    {
        maxSpeed = 0.25;
        maxRotation = 0.5 * M_PI;
        sprite = mSprite;
        behaviour = new Pursue(kinematic, player, maxAcceleration, 1);


    }
};