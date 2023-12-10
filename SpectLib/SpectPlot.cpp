#include "SpectPlot.h"

SPECTLIB::SpectPlot::SpectPlot(SDL_Renderer *renderer, int numBins)
{
    // store the parameters
    m_renderer = renderer;
    m_numBins = numBins;
    SDL_GetRendererOutputSize(m_renderer, &m_xSize, &m_ySize);

    // Setup any default parameters
    m_barHeightScale = 0.5;
    m_barSpacing = 2;
    m_dftLength = numBins * 2;

    // pre-generate the window function according to the dft length
    m_signalWindow = DSP::WindowHann(m_dftLength);

    // Create the bar objets for the display
    m_binWidth = m_xSize / numBins;

    // Compute the width of each bar, taking into account the
    // spacing required between them
    m_barWidth = m_binWidth - (2 * m_barSpacing);

    // configure the bars with random heights
    for (int i = 0; i<numBins; ++i)
        m_barHeight.push_back(static_cast<float>(((rand()%10)+1)/10.0));
    

    // Create the 'bar' m_objectList
    for(int i = 0; i<numBins; ++i)
    {
        int left = (i * m_binWidth) + m_barSpacing;
        int width = m_barWidth;
        int height = static_cast<int>(round(m_ySize * m_barHeight[i]));
        int bottom = m_ySize - height;
        m_objectList.push_back(new LIB2D::Rectangle 
            {m_renderer, left, bottom, width, height, LIB2D::Color(1.0, 0.0, 0.0)});
    }
}

SPECTLIB::SpectPlot::~SpectPlot()
{
    // Delete all of the bojects that have been created for the display
    if (m_objectList.size() > 0)
    {
        for (int i=0; i<m_objectList.size(); ++i)
        {
            delete m_objectList[i];
        }
    }
}

// method to set the number of dft bins
void SPECTLIB::SpectPlot::SetDFTLength(int length)
{
    // Store the length 
    m_dftLength = length;

    // Pregenerate the window function according to the DFT Length
    m_signalWindow.clear();
    m_signalWindow = DSP::WindowHann(m_dftLength);
}

void SPECTLIB::SpectPlot::SetBarHeightScale(float scale)
{
    m_barHeightScale = scale;
}

// Methods to set the data to plot
void SPECTLIB::SpectPlot::SetData(const std::vector<std::complex<double>> data)
{
    // make a temporary copy of the data, so that we can't modify it
    std::vector<std::complex<double>> tempData;
    for(auto currentItem : data)
        tempData.push_back(currentItem);

    // If the supplied vector is longer than the DFT length then truncate it. 
    // if it is shorter, then zero-pad
    if(tempData.size() != m_dftLength)
        tempData.resize(m_dftLength, std::complex<double>(0.0,0.0));

    // apply the window function
    for(int i=0; i<tempData.size(); ++i)
        tempData[i] *= m_signalWindow[i];
    
    // Compute the DFT of the data
    std::vector<std::complex<double>> dftResult = DSP::dft(tempData);

    // Compute the power spectrum
    std::vector<double> absDFT;
    for (auto currentSample : dftResult)
        absDFT.push_back(fabs(currentSample));

    // Compute the maximum value of the power spectrum
    //double maxValue = *std::max_element(absDFT.begin(), absDFT.end());
    double maxValue = 50e3;

    // use this value to compute the normalized power spectrum
    for (int i=0; i<absDFT.size(); ++i)
        absDFT[i] /= maxValue;

    /* Convert to dB, limit to only the first half of the DFT result and 
    limit minimum value to -30dB (teh side-lobe level of the hann window).
    We will also shift the values up to be between 0 and +30, before dividing by 30 
    so we get values taht range between 0 and 1 for the final result. */

    std::vector<double> powerSpectrum;
    for(int i=0; i<(absDFT.size() / 2); ++i)
    {
        double tempValue = 10 * log10(absDFT[i]);
        if(tempValue < -30.0)
        {
            powerSpectrum.push_back(0.01);
        }
        else
        {
            if (tempValue <= 0.0)
                powerSpectrum.push_back((tempValue + 30.0) / 30.0);
            else
                powerSpectrum.push_back(1.0);
        }
    }

    int barStep = powerSpectrum.size() / m_numBins;

    // use this value for the bar height
    int count = 0;
    for(int i=0; i<powerSpectrum.size(); i += barStep)
    {
        m_barHeight[count] = powerSpectrum[i] * m_barHeightScale;
        count++;
    }

    // update the bars
    UpdateBars();

}

// Method to cause the display to be rendered to the renderer
void SPECTLIB::SpectPlot::Render()
{
    // Check that we have a valid renderer
    if (m_renderer != nullptr)
    {
        // we have, so proceed with the render
        for(auto currentobject : m_objectList)
            currentobject -> Render();
    }
}

// Method to update the bars
void SPECTLIB::SpectPlot::UpdateBars()
{
    for(int i=0; i<m_numBins; ++i)
    {
        int left = (i * m_binWidth) + m_barSpacing;
        int width = m_barWidth;
        int height = static_cast<int>(round(m_ySize * m_barHeight[i]));
        int top = m_ySize - height;
        m_objectList[i]->SetPosition(left, top, width, height);
    }
}