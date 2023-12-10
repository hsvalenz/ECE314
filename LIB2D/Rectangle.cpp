#include "Rectangle.h"
#include <algorithm>
#include <iostream>

// The constructor
LIB2D::Rectangle::Rectangle(SDL_Renderer *renderer, int left, int top, int width, int height, Color color)
{
    // Store what we need to 
    m_renderer = renderer;
    m_color = color;

    // Store the vertices 
    m_vX[0] = left;
    m_vY[0] = top;
    m_vX[1] = left;
    m_vY[1] = top + height;
    m_vX[2] = left + width;
    m_vY[2] = top + height;
    m_vX[3] = left + width;
    m_vY[3] = top;

    // Store the floating point versions of the vertices
    for (int i = 0; i<4; ++i)
    {
        m_vXf[i] = static_cast<float>(m_vX[i]);
        m_vYf[i] = static_cast<float>(m_vY[i]);
    }

    // Compute the dimensions of the bounding box
    ComputeBounds();

    // Render the shape into the texture
    Update();

}

// The destructor
LIB2D::Rectangle::~Rectangle()
{
    // If a texture has been defined, make sure that we destroy it
    if(m_texture != nullptr)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

// Function to return the bounding box
SDL_Rect LIB2D::Rectangle::GetBounds()
{
    return m_bounds;
}

// Function to perform the rendering
bool LIB2D::Rectangle::Render()
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

// Function to apply transformations to the shape
void LIB2D::Rectangle::Translate(SDL_Point tVec) 
{
    // Loop through each vertex and apply the translation to each one
    for (int i = 0; i<4; ++i)
    {
        m_vX[i] += tVec.x;
        m_vY[i] += tVec.y;
        m_vXf[i] += static_cast<float>(tVec.x);
        m_vYf[i] += static_cast<float>(tVec.y);
    }

    // Apply the translation to the bounding box
    m_bounds.x += tVec.x;
    m_bounds.y += tVec.y;

}
void LIB2D::Rectangle::Rotate(float theta) 
{
    // we want to rotate about the middle of the bounding box
    float rCentreX = static_cast<float>(m_bounds.x) + (static_cast<float>(m_bounds.w) / 2.0);
    float rCentreY = static_cast<float>(m_bounds.y) + (static_cast<float>(m_bounds.h) / 2.0);

    // Loop through all vertices and apply rotation
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);
    for (int i=0; i<4; ++i)
    {
        float normX = m_vXf[i] - rCentreX;
        float normY = m_vYf[i] - rCentreY;
        float newX = normX*cosTheta - normY*sinTheta;
        float newY = normY*sinTheta + normY*cosTheta;
        m_vXf[i] = newX + rCentreX;
        m_vYf[i] = newY + rCentreY;
        m_vX[i] = static_cast<int>(round(m_vXf[i]));
        m_vY[i] = static_cast<int>(round(m_vYf[i]));

    }

    // re-compute the bounding box
    ComputeBounds();

    // and update the texture
    Update();
}

void LIB2D::Rectangle::ComputeBounds()
{
    // Determine the minimum and maximum values
    // Note - this has to be done this way to handle the case where the rectangle may have 
    // been rotated
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

void LIB2D::Rectangle::Update()
{
    // Initialize the texture
    InitTexture();

    // Allocate memory for a pixel buffer
    Uint32 *tempPixels = new Uint32[m_bounds.w * m_bounds.h];

    // Clear the buffer
    memset(tempPixels, 0, m_bounds.w *m_bounds.h * sizeof(Uint32));

    // Render triangles into the pixel buffer
    std::array<int, 3> vX1 {m_vX[0], m_vX[1], m_vX[3]};
    std::array<int, 3> vY1 {m_vY[0], m_vY[1], m_vY[3]};
    std::array<int, 3> vX2 {m_vX[1], m_vX[2], m_vX[3]};
    std::array<int, 3> vY2 {m_vY[1], m_vY[2], m_vY[3]};
    LIB2D::Triangle::RenderTriangle(tempPixels, vX1, vY1, m_bounds, m_color);
    LIB2D::Triangle::RenderTriangle(tempPixels, vX2, vY2, m_bounds, m_color);

    // update the texture with the pixel buffer
    SDL_UpdateTexture(m_texture, NULL, tempPixels, m_bounds.w * sizeof(Uint32));

    // destroy the pixel buffer
    delete[] tempPixels;
}

// Function to set the position of the rectangle
void LIB2D::Rectangle::SetPosition(int left, int top, int width, int height)
{
    // Store the vertices 
    m_vX[0] = left;
    m_vY[0] = top;
    m_vX[1] = left;
    m_vY[1] = top + height;
    m_vX[2] = left + width;
    m_vY[2] = top + height;
    m_vX[3] = left + width;
    m_vY[3] = top;

    // Store the floating point versions of the vertices
    for (int i = 0; i<4; ++i)
    {
        m_vXf[i] = static_cast<float>(m_vX[i]);
        m_vYf[i] = static_cast<float>(m_vY[i]);
    }

    // Compute the dimensions of the bounding box
    ComputeBounds();

    // Render the shape into the texture
    Update();

}