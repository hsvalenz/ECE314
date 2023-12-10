#include "CApp.h"
#include <iostream>
#include <string>
#include "LIB2D/LIB2D.h"
#include "SpectLib/SpectLib.h"


CApp::CApp()
{
    m_isRunning = true; 
    m_window = NULL;
    m_renderer = NULL;

}

int CApp::Execute(char *fileName)
{
    // Store the file name
    m_fileName = fileName;

    SDL_Event event;

    if (Initialize() == false)
    {
        return -1;
    }

    // Open the file using the WavReader object
    if (m_wavReader.OpenFile(fileName) == false)
    {
        // failed to open the file, so display an error and quit
        std::cout << "Failed to open .wav file." << std::endl;
        return -1;
    }

    // Configure the WavReader object
    /* 29.97 frames per second = 0.0333 seconds per frame
       44.1e3 samples per second
       0.0333 * 44.1e3 = 1472 samples
    */
   m_wavReader.SetChunkSize(1472);
   m_wavReader.SetChunkOverLap(0);

    // Extract the next chunk
    m_wavReader.GetNextChunk(&m_signal);

    while(m_isRunning)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    m_isRunning = false;
                    break;
            }
        }

        Tick();
    }

    // The code for exiting the application, which runs after we drop
    // out of the main loop

    // If a spectPlot has been defined, then make sure we delete it
    if (m_spectPlot != nullptr)
        delete m_spectPlot;

    // Make sure to free the surface we created for writing images
    SDL_FreeSurface(m_imageSurface);

    // Destroy the window and call SDL_Quit()
    SDL_DestroyWindow(m_window);
    m_window = NULL;
    SDL_Quit();
    
    return 0;
}

bool CApp::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;

    m_window = SDL_CreateWindow("Spectrogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

    if (m_window != nullptr)
    {
        m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    }else
    {
        return false;
    }

    // Store the window dimensions
    SDL_GetRendererOutputSize(m_renderer, &m_xSize, &m_ySize);

    // first the image surface
    m_imageSurface = SDL_CreateRGBSurface(0, m_xSize, m_ySize, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

    // Second define the color that we wish to make transparent (chromakey)
    m_chromaKey = SDL_MapRGB(m_imageSurface->format, 0, 0, 0);

    // set the frame count to 0
    m_frameCount = 0;

    // Initialize the display object
    m_spectPlot = new SPECTLIB::SpectPlot(m_renderer, 128);

    /* Set the DFT length to a power of two value up to the chunk size
    that we use for the wavReader object. Smaller values for DFT length
    will give greater performance*/
    m_spectPlot->SetDFTLength(512);

    // Set the height scaling so the bars don't fill the vertical size of the window
    m_spectPlot->SetBarHeightScale(0.25);

    /*
    // generate a test signal
    int N = 256;
    double sigK = 16.0;
    std::vector<std::complex<double>> signal;
    for(int x = 0; x<N; ++x)
    {
        auto currentSample = std::complex<double>
        (cos((2*M_PI/static_cast<double>(N)) * sigK * static_cast<double>(x))) +
        0.25 * (cos((2*M_PI/static_cast<double>(N)) * (sigK*1.5) * static_cast<double>(x))) +
        0.125 * (cos((2*M_PI/static_cast<double>(N)) * (sigK*2.0) * static_cast<double>(x))) +
        0.0125 * (cos((2*M_PI/static_cast<double>(N)) * (sigK*3.0) * static_cast<double>(x)));

        signal.push_back(currentSample);
    }

    // apply the test signal to m_spectPlot
    m_spectPlot->SetData(signal); */

    return true;
}

void CApp::Tick()
{
    // set the background color to black
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // Send this datat to the m_spectPlot
    m_spectPlot->SetData(m_signal);

    // Render the display
    m_spectPlot->Render();

    // Update the display
    SDL_RenderPresent(m_renderer);

    // Gerenate the file name
    char buf[5];
    std::sprintf(buf, "%04d", m_frameCount);
    std::string fileName="./Animation/Frame";
    fileName.append(buf);
    fileName.append(".bmp");

    // Capture the display to the image surface
    SDL_RenderReadPixels(m_renderer, nullptr, SDL_PIXELFORMAT_RGBA32, m_imageSurface->pixels, m_imageSurface->pitch);

    // Set the chroma-key for the image
    SDL_SetColorKey(m_imageSurface, SDL_TRUE, m_chromaKey);

    // Output the current frame
    SDL_SaveBMP(m_imageSurface, fileName.c_str());

    // Increment the frame counter
    m_frameCount++;

    // Extract the next chunk
    if(m_wavReader.GetNextChunk(&m_signal) == false)
    {
        m_isRunning = false;
        std::cout << "End of file reached." << std::endl;
    }

    // Insert a delay 
    //SDL_Delay(15);

}

/*
void CApp::OnEvent(SDL_Event *event)
{
    if (event->type == SDL_QUIT)
    {
        isRunning = false;
    }
}

void CApp::OnLoop()
{

}

void CApp::OnRender()
{
    SDL_Delay(1);
}

void CApp::OnExit()
{
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    SDL_Quit();
}*/