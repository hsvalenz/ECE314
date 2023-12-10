#ifndef CAPP_H
#define CAPP_H

#include <SDL2/SDL.h>
#include <vector>
#include "LIB2D/LIB2D.h"
#include "SpectLib/SpectLib.h"

class CApp
{
  public:
    CApp();

    int Execute(char *fileName);
    bool Initialize();
    void Tick();

    //void OnEvent(SDL_Event *event);
    //void OnLoop();
    //void OnRender();
    //void OnExit();

  private: 
    int m_xSize, m_ySize;
    bool m_isRunning;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    char *m_fileName;
    std::vector<std::complex<double>> m_signal;

    // Create the plot object
    SPECTLIB::SpectPlot *m_spectPlot;

    // create the wavReader object
    SPECTLIB::WavReader m_wavReader;

    // Stuff we need for writing images
    SDL_Surface *m_imageSurface;
    Uint32 m_chromaKey;
    int m_frameCount;

    std::array<int, 3> m_redList {51,255,255};
    std::array<int, 3> m_greeList {128,128,191};
    std::array<int, 3> m_blueList {204,0,0};
    
};

#endif