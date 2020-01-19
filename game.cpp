#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <string>

#include "position.hpp"
#include "texture.hpp"

using namespace std;
using namespace aifg;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void renderKinematic(SDL_Renderer* renderer, Kinematic& object, LTexture& texture)
{
    int x = round(object.position.x);
    int y = round(object.position.z);
    texture.render(renderer, x, y);
}

int main()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
        fprintf(stderr, "Warning: Linear texture filtering not enabled!\n");
    }

    SDL_Window* window = 
        SDL_CreateWindow("AIFG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(window == NULL){
        fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_Renderer* renderer = 
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    if(renderer == NULL){
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if( !(IMG_Init(imgFlags) & imgFlags) ){
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 0;
    }

    LTexture dotTexture;
    if(!dotTexture.loadFromFile(renderer, "img/dot.bmp")){
        fprintf(stderr, "Failed to load dot texture!\n");
        return 0;
    }

    bool quit = false;

    SDL_Event e;

    Kinematic character;

    Uint32 startTime = 0;

    double speed = 0.5;

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
                }
            }

            else if(e.type == SDL_KEYUP && e.key.repeat == 0){
                switch(e.key.keysym.sym){
                    case SDLK_UP: character.velocity += speed * Vector3::Z; break;
                    case SDLK_DOWN: character.velocity -= speed * Vector3::Z; break;
                    case SDLK_LEFT: character.velocity += speed * Vector3::X; break;
                    case SDLK_RIGHT: character.velocity -= speed * Vector3::X; break;
                }
            }
        }

        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( renderer );

        character.update(SDL_GetTicks() - startTime);

        startTime = SDL_GetTicks();
        
        renderKinematic(renderer, character, dotTexture);

        SDL_RenderPresent(renderer);
    }

    dotTexture.free();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    IMG_Quit();
    SDL_Quit();

    return 0;
}