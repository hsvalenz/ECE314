#include "wavreader.h"
#include <algorithm>
#include <iostream>

SPECTLIB::WavReader::WavReader()
{
    // setup default parameters
    m_currentChunkIndex = 0;
    m_chunkSize = 256;
    m_chunkOverlap = 128;
    m_prevEnd = 0;
    m_haveData = false;
}

SPECTLIB::WavReader::~WavReader()
{
    // here we will tidy up variables we have created
    // when we have finished, free the buffer
    if(m_haveData)
        SDL_FreeWAV(m_dataBuffer);
}

// Function to open the .wav file 
bool SPECTLIB::WavReader::OpenFile(char *fileName)
{
    // store the filename
    m_fileName = fileName;

    // attempt to open the file
    if (SDL_LoadWAV(m_fileName, &m_dataType, &m_dataBuffer, &m_dataLength) == nullptr)
    {
        // attempt failed
        return false;
    }
    else 
    {
        // we have opened the file successfully, so extract whwat we need and return true
        m_haveData = true;
        m_dataFormat = m_dataType.format;
        return true;
    }
}

// Functions to set the parameters
void SPECTLIB::WavReader::SetChunkSize(int chunkSize)
{
    m_chunkSize = chunkSize;
}

void SPECTLIB::WavReader::SetChunkOverLap(int chunkOverlap)
{
    m_chunkOverlap = chunkOverlap;
}

// Function to return the next chunk
bool SPECTLIB::WavReader::GetNextChunk(std::vector<std::complex<double>> *signal)
{
    // First, check that we actually have some data to return
    if (m_dataBuffer == nullptr)
        return false;

    // Delete the previous contents of signal, if any
    signal->clear();

    // Compute start point of the next chunk
    int startIndex = std::max((m_prevEnd - m_chunkOverlap), 0);

    // Compute end point of the next chunk
    int endIndex = startIndex + m_chunkSize;
    m_prevEnd = endIndex;

    // extract samples from buffer, convert to appropriate format and use to populate signal
    int bytesPerSample = static_cast<int>(SDL_AUDIO_BITSIZE(m_dataFormat)) / 8;
    int numChannels = static_cast<int>(m_dataType.channels);

    // Compute the step size we will use to loop through the buffer
    int byteStep = numChannels * bytesPerSample;

    // compute the start position in the buffer, in bytes
    int byteStartIndex = startIndex * byteStep;

    // Compute the end position in the buffer, in bytes
    int byteEndIndex = endIndex * byteStep;

    // Loop through the buffer and extract the samples we need
    int currentIndex = byteStartIndex;
    short int currentSample;
    while((currentIndex < byteEndIndex) && (currentIndex < (m_dataLength-1)))
    {
        /* Extract teh current sample. note that for now we assuming either 
        8-bit or 16-bit data. For greater than 16-bit cases, we would need 
        to use a loop to extract each of the bytes and convert*/
        if (bytesPerSample > 1)
        {
            /*We need to convert the individual bytes into a 16-bit number and 
            the way that we do this will depend on whether the data were encoded as 
            big-endian or little-endian. */
            if (SDL_AUDIO_ISBIGENDIAN(m_dataFormat))
            {
                currentSample = (m_dataBuffer[currentIndex] << 8) + m_dataBuffer[currentIndex+1];

            }
            else 
            {
                currentSample = (m_dataBuffer[currentIndex+1] << 8) + m_dataBuffer[currentIndex];

            }
        }
        else 
        {
            // in this case, we have only one byte per sample, so it is very simple
            currentSample = m_dataBuffer[currentIndex];
        }

        // Convert the sample to the format we use for our signals (complex double)
        std::complex<double> signalSample(static_cast<double>(currentSample), 0.0);

        // Add this sample to the signal vector
        signal->push_back(signalSample);

        /* Increment current Index taking into account the number of channels and 
        number of bytes per sample*/
        currentIndex += byteStep;
    }

    // Return true if this all happened successfully
    // Return false if we have reached the end of the signal
    if (currentIndex >= m_dataLength)
        return false;
    else
        return true;

}
