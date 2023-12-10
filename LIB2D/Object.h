#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include "Color.h"

namespace LIB2D
{
    class Object
    {
      public:
        Object();
        virtual ~Object();

        bool SetColor(Color newColor);
        Color GetColor();
        virtual SDL_Rect GetBounds();
        virtual bool Render();
        virtual void Translate(SDL_Point tVec);
        virtual void InitTexture();
        static Uint32 ConvertColor(Color color);
        virtual void Rotate(float angle);

      protected:
        Color m_color;
        SDL_Rect m_bounds;
        SDL_Renderer *m_renderer;
        SDL_Texture *m_texture;

    };

}



#endif
