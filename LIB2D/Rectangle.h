#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL2/SDL.h>
#include <array>
#include "Triangle.h"
#include "Object.h"
#include "Color.h"

namespace LIB2D
{
    class Rectangle : public Object
    {
      public:
        Rectangle(SDL_Renderer *renderer, int left, int top, int width, int height, Color color);
        virtual ~Rectangle() override;

        // Function to return the bounding box
        virtual SDL_Rect GetBounds() override;

        // Function to perform the rendering
        virtual bool Render() override;

        // Function to apply transformations to the shape
        virtual void Translate(SDL_Point tVec) override;
        virtual void Rotate(float theta) override;

        // Functions to update the shape
        void SetPosition(int left, int top, int width, int height);

        // Functions to return parameters
        // ?

        // Private functions
      private:
        void ComputeBounds();
        void Update();

        // Private memeber variables
      private:
        std::array<int, 4> m_vX;
        std::array<int, 4> m_vY;
        std::array<float, 4> m_vXf;
        std::array<float, 4> m_vYf;

    };
}


#endif