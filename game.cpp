#include "game.hpp"

namespace aifg
{
    void Game::init(const char* title, int width, int height, bool fullscreen)
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0){
            fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
            return;
        }

        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
            fprintf(stderr, "Warning: Linear texture filtering not enabled!\n");
        }

        int flags = SDL_WINDOW_SHOWN;
        if(fullscreen)
            flags |= SDL_WINDOW_FULLSCREEN;

        window = 
            SDL_CreateWindow("AIFG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                             width, height, flags);

        if(window == NULL){
            fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
            return;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | 
                                                   SDL_RENDERER_PRESENTVSYNC);
        if(renderer == NULL){
            fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            return;
        }

        int imgFlags = IMG_INIT_PNG;
        if( !(IMG_Init(imgFlags) & imgFlags) ){
            fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            return;
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        if(!enemyTexture.loadFromFile(renderer, "img/RedEnemy.png")){
            fprintf(stderr, "Failed to load enemy texture!\n");
            return;
        }

        if(!playerTexture.loadFromFile(renderer, "img/Player.png")){
            fprintf(stderr, "Failed to load texture!\n");
            return;
        }

        detector.add(Segment({0,0,960}, {0,0,0}));
        detector.add(Segment({1280,0,960}, {0,0,960}));
        detector.add(Segment({1280,0,0}, {1280,0,960}));
        detector.add(Segment({0,0,0}, {1280,0,0}));

        player.init({0, 0, 0}, playerTexture);

        for(int i = 100; i < 1200; i += 100){
            enemies.push_back(new RedEnemy());
            enemies.back()->init({ (double) i, 0, 400}, &player.kinematic, enemyTexture, enemiesKin, detector);
        }

        isRunning = true;
    } 

    void Game::close()
    {
        enemyTexture.free();
        playerTexture.free();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
    }

    void Game::handleEvents()
    {
        SDL_Event event;

        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT)
                isRunning = false;

            else if(event.type == SDL_KEYDOWN && event.key.repeat == 0){
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                        player.userSteering.linear -= player.maxAcceleration * Vector3::Z;
                        break;
                    case SDLK_DOWN: 
                        player.userSteering.linear += player.maxAcceleration * Vector3::Z;
                        break;
                    case SDLK_LEFT:
                        player.userSteering.linear -= player.maxAcceleration * Vector3::X;
                        break;
                    case SDLK_RIGHT:
                        player.userSteering.linear += player.maxAcceleration * Vector3::X;
                        break;
                    default: break;
                }
            } 

            else if(event.type == SDL_KEYUP && event.key.repeat == 0){
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                        player.userSteering.linear += player.maxAcceleration * Vector3::Z;
                        break;
                    case SDLK_DOWN: 
                        player.userSteering.linear -= player.maxAcceleration * Vector3::Z;
                        break;
                    case SDLK_LEFT:
                        player.userSteering.linear += player.maxAcceleration * Vector3::X;
                        break;
                    case SDLK_RIGHT:
                        player.userSteering.linear -= player.maxAcceleration * Vector3::X;
                        break;
                    default: break;
                }
            }
        }
    }

    void Game::update()
    {
        int deltaTime = SDL_GetTicks() - timer;
        timer = SDL_GetTicks();

        player.update(deltaTime);
        
        for(RedEnemy* enemy : enemies)
            enemy->update(deltaTime);
    }

    void Game::render()
    {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        player.draw(renderer);

        for(RedEnemy* enemy : enemies)
            enemy->draw(renderer);

        SDL_RenderPresent(renderer);
    }
};