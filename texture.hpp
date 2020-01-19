#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

class LTexture
{
    public:
        // Constructor
        LTexture();

        // Destructor
        ~LTexture();

        // Loads image
        bool loadFromFile(SDL_Renderer* renderer, std::string path);

        // Deallocates texture
        void free();

        void render(SDL_Renderer* renderer, int x, int y, 
                    SDL_Rect* clip = NULL, double angle = 0.0, double scaleFactor = 1, 
                    SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        inline int getWidth()
        {
            return mWidth;
        }
        inline int getHeight()
        {
            return mHeight;
        }

    private:
        // The real texture
        SDL_Texture* mTexture;

        // Dimensions
        int mWidth;
        int mHeight;
};