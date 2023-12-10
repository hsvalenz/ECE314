#include "Triangle.h"
#include <algorithm>
#include <iostream>

LIB2D::Triangle::Triangle(SDL_Renderer *renderer, SDL_Point v1, SDL_Point v2, SDL_Point v3, Color color)
{
    m_renderer = renderer;
    m_color = color;

    // Store the vertices into the appropriate arrays
    m_vX[0] = v1.x;
    m_vX[1] = v2.x;
    m_vX[2] = v3.x;
    m_vY[0] = v1.y;
    m_vY[1] = v2.y;
    m_vY[2] = v3.y;

    // Store floating point versions of the vertices
    for(int i = 0; i < 3; ++i)
    {
        m_vXf[i] = static_cast<float>(m_vX[i]);
        m_vYf[i] = static_cast<float>(m_vY[i]);
    }

    // Compute the dimensions of the boinding box
    ComputeBounds();

    // Render the shape into the texture
    Update();
}

LIB2D::Triangle::~Triangle() 
{

}

// Function to return the bounding box
SDL_Rect LIB2D::Triangle::GetBounds() 
{
    return m_bounds;
}

bool LIB2D::Triangle::Render()
{
    if (m_renderer != nullptr)
    {
        // to render, we simply blit the texture into the renderer at the right position
        SDL_Rect srcRect;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = m_bounds.w;
        srcRect.h = m_bounds.h;

        SDL_RenderCopy(m_renderer, m_texture, &srcRect, &m_bounds);

        return true;
    }
    else 
    {
        return false;
    }
}

// function to apply transformations to the shape
void LIB2D::Triangle::Translate(SDL_Point tVec) 
{

}


void LIB2D::Triangle::Rotate(float angle)
{

}


// Function to set parameters
// void SetVerticese(SDL_Point v1, SDL_Point v2, SDL_Point v3);
void LIB2D::Triangle::SetColor(Color color)
{
    m_color = color;
}


// Function to actually perform the rendering of the two traingles
void LIB2D::Triangle::RenderTriangle(Uint32 *pixelBuffer, std::array<int,3> vX, std::array<int,3> vY, SDL_Rect bounds, Color color)
{
    // Normalize the input coordinates
    std::array<int,3> normX;
    std::array<int,3> normY;
    for(int i = 0; i<3; ++i)
    {
        normX[i] = vX[i] - bounds.x;
        normY[i] = vY[i] - bounds.y;
    }

    // Interpolate all the points between the vertices
    std::vector<SDL_Point> edgePoints;
    Interpolate(normX[0], normY[0], normX[1], normY[1], &edgePoints);
    Interpolate(normX[1], normY[1], normX[2], normY[2], &edgePoints);
    Interpolate(normX[2], normY[2], normX[0], normY[0], &edgePoints);

    // Now we need to sort xPoints and yPoints by magnitude and y points 
    std::sort(edgePoints.begin(), edgePoints.end(), PointSortFcn);

    /* Each pair of points in edgePoints should now have the same y value.
    Draw a horizontal line for each y value, between the two corresponding
    x values. */

    Uint32 pixelColor = ConvertColor(color);
    for(int i = 0; i<edgePoints.size() -1; i+=2)
    {
        // Extract the coordinate pair
        int x1 = edgePoints[i].x;
        int x2 = edgePoints[i+1].x;
        int y1 = edgePoints[i].y;

        // Swap x1 and x2 if necessary, to ensure we dreaw from min to max_element
        if (x2 < x1)
            std::swap(x1, x2);

        // Draw from x1 (minimum) to x2 (maximum)
        for (int x = x1; x<=x2; ++x)
        {
            pixelBuffer[y1*bounds.w + x] = pixelColor;
        }
    }
}



void LIB2D::Triangle::ComputeBounds()
{
    // Determine the minimum and maximum values
    int minX = *std::min_element(m_vX.begin(), m_vX.end()) - 1;
    int minY = *std::min_element(m_vY.begin(), m_vY.end()) - 1;
    int maxX = *std::max_element(m_vX.begin(), m_vX.end()) + 1;
    int maxY = *std::max_element(m_vY.begin(), m_vY.end()) + 1;

    // Update the bounding box accordingly 
    m_bounds.x = minX;
    m_bounds.y = minY;
    m_bounds.w = (maxX - minX);
    m_bounds.h = (maxY - minY);
}
void LIB2D::Triangle::Update()
{
    // Initialize the texture
    InitTexture();

    // Allocate memeory for the pixel buffer
    Uint32 *tempPixels = new Uint32[m_bounds.w * m_bounds.h];

    // Clear the buffer
    memset(tempPixels, 0, m_bounds.w * m_bounds.h * sizeof(Uint32));

    // Render the Triangle
    RenderTriangle(tempPixels, m_vX, m_vY, m_bounds, m_color);
   
    // Update the texture with the pixel buffer
    SDL_UpdateTexture(m_texture, NULL, tempPixels, m_bounds.w * sizeof(Uint32));

    // Destroy the pixel buffer
    delete[] tempPixels;
}

void LIB2D::Triangle::Interpolate(int x1, int y1, int x2, int y2, std::vector<SDL_Point> *edgePoints)
{
    /* If the two points describe a horizontal line, then we just ignore 
    it. This will be the case if y1 = y2.  */

    if(y1 != y2)
    {
        // The line is not horizontal, so we need to do stuff
        // Ensure that y2 is larger than y1
        if(y2 < y1)
        {
            std::swap(y1, y2);
            std::swap(x1, x2);
        }

        // Compute the x and y range
        int yRange = y2 - y1;
        int xRange = x2 - x1;

        // Compute the x step size (we assume y step size is 1)
        float xStep = static_cast<float>(xRange) / static_cast<float>(yRange);

        // loop through each y and compute x accordingly
        int count = 0;
        for(int tY= y1; tY<y2; ++tY)
        {
            int tX = static_cast<int>(round(x1 + (xStep * static_cast<float>(count))));
            edgePoints->push_back({tX, tY});
            count++;
        }
    }
}

bool LIB2D::Triangle::PointSortFcn(SDL_Point i, SDL_Point j)
{
    return (i.y < j.y);
}