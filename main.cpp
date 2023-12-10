#include "CApp.h"
#include "iostream"

int main(int argc, char* argv[])
{
    CApp theApp;
    if (argc == 2)
    {
        return theApp.Execute(argv[1]);
    }
    else
    {
        std::cout << "Correct usage: main filename.wav" << std::endl;
        return -1;
    }
}