/**
 * @file  Main.cpp
 *
 * @brief  Tests the FBPainter library.
 */

#include "../FBPainter.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <cstdlib>


static const int defaultPixelsPerSecond = 300;

int main(int argc, char** argv)
{
    long pps = 0;
    std::string imageFile = "cursor.png";

    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);
        if (arg.find_first_not_of("0123456789") >= arg.length())
        {
            pps = std::stol(arg, nullptr, 10);
        }
        else if (arg.find_last_of(".png") == arg.length() - 1)
        {
            imageFile = arg;
        }
    }
    if (pps <= 0)
    {
        pps = defaultPixelsPerSecond;
    }
    static const std::chrono::nanoseconds loopDuration(1000000000 / pps);
    using namespace FBPainter;

    FrameBuffer frameBuffer("/dev/fb0");
    const int frameWidth = frameBuffer.getWidth();
    const int frameHeight = frameBuffer.getHeight();
    std::cout << "Screen is " << frameWidth << " x " << frameHeight << "\n";

    Image testImage(imageFile.c_str(), frameWidth, frameHeight);
    const int xMax = frameWidth - testImage.getWidth();
    const int yPos = (frameHeight - testImage.getHeight()) / 2;
    int x = xMax / 2;
    bool moveRight = true;
    testImage.setImageOrigin(x, yPos, &frameBuffer);
    testImage.drawImage(&frameBuffer);

    struct timespec sleepTimer = {0, 0};
    for(;;)
    {
        typedef std::chrono::nanoseconds Nanoseconds;
        typedef std::chrono::time_point<std::chrono::high_resolution_clock,
                Nanoseconds> Time;
        const Time loopStart = std::chrono::high_resolution_clock::now();
        if (moveRight)
        {
            x++;
            moveRight = (x != xMax);
        }
        else
        {
            x--;
            moveRight = (x == 0);
        }
        testImage.setImageOrigin(x, yPos, &frameBuffer);
        const Time loopEnd = std::chrono::high_resolution_clock::now();
        Nanoseconds timePassed = loopEnd - loopStart;
        if (timePassed < loopDuration)
        {
            Nanoseconds sleepTime = loopDuration - timePassed;
            sleepTimer.tv_nsec = sleepTime.count();
            nanosleep(&sleepTimer, nullptr);
        }
    }
}
