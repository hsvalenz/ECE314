#ifndef WAVREADER_H
#define WAVREADER_H

#include <SDL2/SDL.h>
#include <vector>
#include <complex>

namespace SPECTLIB
{
    class WavReader
    {
      public:
        WavReader();
        ~WavReader();

        // Function to open the .wav file 
        bool OpenFile(char *fileName);

        // Functions to set the parameters
        void SetChunkSize(int chunkSize);
        void SetChunkOverLap(int chunkOverlap);

        // Function to return the next chunk
        bool GetNextChunk(std::vector<std::complex<double>> *signal);

      // private member variables
      private:
        SDL_AudioSpec m_dataType;
        SDL_AudioFormat m_dataFormat;
        Uint32 m_dataLength;
        Uint8 *m_dataBuffer;

        char *m_fileName;
        int m_chunkSize;
        int m_chunkOverlap;
        int m_currentChunkIndex;
        int m_prevEnd;
        bool m_haveData;

    };


} // namespace SPECTLIB



#endif