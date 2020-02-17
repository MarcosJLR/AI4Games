#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "position.hpp"
#include "kinematic.hpp"
#include "steering.hpp"
#include "texture.hpp"
#include "character.hpp"

namespace aifg
{
    class Game
    {
    public:
        void init(const char* title, int width, int height, bool fullscrren);
        void handleEvents();
        void update();
        void render();
        bool running() { return isRunning; }
        void close();

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        LTexture playerTexture;
        LTexture enemyTexture;
        bool isRunning = false;
        int timer = 0;

        Player player;
        std::vector<RedEnemy*> enemies;
        std::vector<Kinematic*> enemiesKin;

        CollisionDetector detector;
    };
};