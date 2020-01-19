#include "texture.hpp"

using namespace std;

LTexture::LTexture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    free();
}

bool LTexture::loadFromFile(SDL_Renderer* renderer, string path)
{
    free();

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    
    if (loadedSurface == NULL) 
    {
        fprintf(stderr, "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));

    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(newTexture == NULL)
    {
        fprintf(stderr, "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }

    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);

    mTexture = newTexture;
    return true;
}

void LTexture::free()
{
    if(mTexture != NULL){
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, 
                      double angle, double scaleFactor, SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    if(clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    renderQuad.w = round(scaleFactor * renderQuad.w);
    renderQuad.h = round(scaleFactor * renderQuad.h);

    SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}                    
