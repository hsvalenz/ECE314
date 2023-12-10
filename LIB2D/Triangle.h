#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <SDL2/SDL.h>
#include <array>
#include <vector>
#include "Object.h"
#include "Color.h"

namespace LIB2D
{
    class Triangle : public Object
    {
      public:
        Triangle(SDL_Renderer *renderer, SDL_Point v1, SDL_Point v2, SDL_Point v3, Color color);
        virtual ~Triangle() override;

        // Function to return the bounding box
        virtual SDL_Rect GetBounds() override;

        // Function to perform the rendering
        virtual bool Render() override;

        // function to apply transformations to the shape
        virtual void Translate(SDL_Point tVec) override;
        virtual void Rotate(float angle) override;

        // Function to set parameters
        // void SetVertices(SDL_Point v1, SDL_Point v2, SDL_Point v3);
        void SetColor(Color color);

        // Function to actually perform the rendering of the two traingles
        static void RenderTriangle(Uint32 *pixelbuffer, std::array<int,3> vX, std::array<int,3> vY, SDL_Rect bounds, Color color);

        // Private Functions
      private:
        void ComputeBounds();
        void Update();
        static void Interpolate(int x1, int y1, int x2, int y2, std::vector<SDL_Point> *edgePoints);
        static bool PointSortFcn(SDL_Point i, SDL_Point j);

      private:
        std::array<int,3> m_vX;
        std::array<int,3> m_vY;
        std::array<float,3> m_vXf;
        std::array<float,3> m_vYf;

    };
}

#endif