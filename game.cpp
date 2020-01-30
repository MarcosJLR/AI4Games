#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <string>

#include "position.hpp"
#include "kinematic.hpp"
#include "steering.hpp"
#include "texture.hpp"

using namespace std;
using namespace aifg;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow;
SDL_Renderer* gRenderer;

void renderKinematic(SDL_Renderer* renderer, Kinematic& object, LTexture& texture, SDL_Rect* clip = NULL)
{
    int x = round(object.position.x);
    int y = round(object.position.z);
    double angle = (object.orientation * 180) / M_PI;
    texture.render(renderer, x, y, clip, angle, 0.25);
}

bool init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
        fprintf(stderr, "Warning: Linear texture filtering not enabled!\n");
    }

    gWindow = 
        SDL_CreateWindow("AIFG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(gWindow == NULL){
        fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | 
                                               SDL_RENDERER_PRESENTVSYNC);
    if(gRenderer == NULL){
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if( !(IMG_Init(imgFlags) & imgFlags) ){
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void close()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main()
{

    if(!init()){
        fprintf(stderr, "Failed to initialize\n");
        return 0;
    }

    LTexture dotTexture;
    if(!dotTexture.loadFromFile(gRenderer, "img/arrows.png")){
        fprintf(stderr, "Failed to load dot texture!\n");
        return 0;
    }

    bool quit = false;

    SDL_Event e;

    Kinematic character;
    Kinematic enemy(Vector3(300, 0, 220), M_PI);

    bool flee = false;

    SDL_Rect clip = {160, 400, 120, 160};

    Uint32 startTime = 0;

    double speed = 0.5;

    Wander wander = Wander(enemy, 0.001, 100, 150, 1, 0);
    LookWhereYoureGoing look(enemy, 0.0005, (speed * M_PI) / 360, 0.15, 4);
    
    while(!quit){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT)
                quit = true;

            else if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
                switch(e.key.keysym.sym){
                    case SDLK_UP: character.velocity -= speed * Vector3::Z; break;
                    case SDLK_DOWN: character.velocity += speed * Vector3::Z; break;
                    case SDLK_LEFT: character.velocity -= speed * Vector3::X; break;
                    case SDLK_RIGHT: character.velocity += speed * Vector3::X; break;
                    case SDLK_q: character.rotation -= (speed * M_PI) / 180; break;
                    case SDLK_e: character.rotation += (speed * M_PI) / 180; break;
                    case SDLK_f: flee = true; break;
                }
            }

            else if(e.type == SDL_KEYUP && e.key.repeat == 0){
                switch(e.key.keysym.sym){
                    case SDLK_UP: character.velocity += speed * Vector3::Z; break;
                    case SDLK_DOWN: character.velocity -= speed * Vector3::Z; break;
                    case SDLK_LEFT: character.velocity += speed * Vector3::X; break;
                    case SDLK_RIGHT: character.velocity -= speed * Vector3::X; break;
                    case SDLK_q: character.rotation += (speed * M_PI) / 180; break;
                    case SDLK_e: character.rotation -= (speed * M_PI) / 180; break;
                    case SDLK_f: flee = false; break;
                }
            }
        }

        if(flee) 
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0x00 );
        else     
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        
        SDL_RenderClear( gRenderer );

        character.update(SDL_GetTicks() - startTime);
        
        SteeringOutput steering(wander.getSteering().linear, look.getSteering().angular);
        enemy.update(steering, 0.25, 0.1, SDL_GetTicks() - startTime);

        startTime = SDL_GetTicks();
        
        renderKinematic(gRenderer, character, dotTexture, &clip);
        renderKinematic(gRenderer, enemy, dotTexture, &clip);

        SDL_RenderPresent(gRenderer);
    }

    dotTexture.free();
    close();

    return 0;
}