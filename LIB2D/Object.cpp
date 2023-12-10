// this file is designed to be inherited
#include "Object.h"
#include <iostream>

LIB2D::Object::Object()
{

}

LIB2D::Object::~Object()
{

}


// set and get color are not going to be overwritten
bool LIB2D::Object::SetColor(Color newColor)
{
    m_color = newColor;
    return true;
}

LIB2D::Color LIB2D::Object::GetColor()
{
    return m_color;
}

SDL_Rect LIB2D::Object::GetBounds()
{
    SDL_Rect temp;
    return temp;
}

bool LIB2D::Object::Render()
{
    return true;
}

void LIB2D::Object::Translate(SDL_Point tVec)
{

}

void LIB2D::Object::InitTexture()
{
    Uint32 rmask, gmask, bmask, amask;

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
    #else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
    #endif

    // Create the texture that will store the image of our shape
    SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, m_bounds.w, m_bounds.h, 32, rmask, gmask, bmask, amask);
    m_texture = SDL_CreateTextureFromSurface(m_renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

}

Uint32 LIB2D::Object::ConvertColor(Color color)
{
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        Uint32 pixelColor = (color.GetBlue() << 24) + (color.GetGreen() << 16) + (color.GetRed() << 8) + 255;
    #else
        Uint32 pixelColor = (255 << 24) + (color.GetRed() << 16) + (color.GetGreen() << 8) + color.GetBlue();
    #endif

    return pixelColor;
}

void LIB2D::Object::Rotate(float angle)
{
    
}