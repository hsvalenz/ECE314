#ifndef SPECTPLOT_H
#define SPECTPLOT_H

#include <vector>
#include <complex>
#include <algorithm>
#include <SDL2/SDL.h>
#include "dsp.h"
#include "../LIB2D/LIB2D.h"

namespace SPECTLIB
{
    class SpectPlot
    {
      public:
        SpectPlot(SDL_Renderer *renderer, int numBins);
        ~SpectPlot();

        // Methods to set the data to plot
        void SetData(std::vector<std::complex<double>> data);

        // Method to modify the number of dft bins
        void SetDFTLength(int length);
        
        // Method to set the bar height scale
        void SetBarHeightScale(float scale);

        // Method to cause the display to be rendered to the renderer
        void Render();

        

      private:
        void UpdateBars();

        SDL_Renderer *m_renderer;
        std::vector<LIB2D::Rectangle *> m_objectList;
        std::vector<float> m_barHeight;
        std::vector<std::complex<double>> m_signalWindow;
        int m_xSize, m_ySize, m_numBins;
        int m_barSpacing, m_binWidth, m_barWidth;
        int m_dftLength;
        float m_barHeightScale;

    };
}


#endif


