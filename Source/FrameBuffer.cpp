#include "FrameBuffer.h"
#include <algorithm>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/ioctl.h>


// Opens and memory maps the frame buffer file.
FBPainter::FrameBuffer::FrameBuffer(const char* bufferPath)
{
    errno = 0;
    bufferFD = open(bufferPath, O_RDWR);
    if (bufferFD == -1)
    {
        perror("Opening frame buffer file failed");
        return;
    }
    using std::min;
    int ioResult = min(0, ioctl(bufferFD, FBIOGET_VSCREENINFO, &vInfo)); 
    vInfo.grayscale = 0;
    vInfo.bits_per_pixel = 32;
    ioResult = min(ioResult, ioctl(bufferFD, FBIOPUT_VSCREENINFO, &vInfo)); 
    ioResult = min(ioResult, ioctl(bufferFD, FBIOGET_VSCREENINFO, &vInfo)); 
    ioResult = min(ioResult, ioctl(bufferFD, FBIOGET_FSCREENINFO, &fInfo)); 
    if (ioResult == -1)
    {
        perror("Reading frame buffer info failed");
        closeAndClearData();
        return;
    }
    bufferSize = vInfo.yres_virtual * fInfo.line_length;
    bufferData = (uint8_t*) mmap(0, bufferSize, PROT_READ | PROT_WRITE,
                        MAP_SHARED, bufferFD, (off_t) 0);
    if (bufferData == MAP_FAILED)
    {
        perror("Failed to map frame buffer to memory");
        bufferData = nullptr;
        closeAndClearData();
    }
}


// Closes and unmaps the frame buffer file on destruction.
FBPainter::FrameBuffer::~FrameBuffer()
{
    closeAndClearData();
}


// Checks if the buffer is open and ready for IO.
bool FBPainter::FrameBuffer::isBufferOpen() const
{
    return bufferData != nullptr;
}


// Gets the frame buffer's width.
size_t FBPainter::FrameBuffer::getWidth() const
{
    return vInfo.xres;
}


// Gets the frame buffer's height.
size_t FBPainter::FrameBuffer::getHeight() const
{
    return vInfo.yres;
}


// Gets the color set at a specific pixel in the buffer.
FBPainter::RGBPixel FBPainter::FrameBuffer::getPixel
(const size_t xPos, const size_t yPos)
{
    uint32_t* pixelPtr = getMappedPoint(xPos, yPos);
    if (pixelPtr == nullptr)
    {
        return RGBPixel(0, 0, 0);
    }
    const uint32_t cValue = *pixelPtr;
    return RGBPixel((uint8_t) (cValue >> vInfo.red.offset),
            (uint8_t) (cValue >> vInfo.green.offset),
            (uint8_t) (cValue >> vInfo.blue.offset));
}


// Sets the color at a specific pixel in the buffer.
void FBPainter::FrameBuffer::setPixel(const size_t xPos, const size_t yPos,
        const RGBPixel color)
{
    uint32_t* pixelPtr = getMappedPoint(xPos, yPos);
    if (pixelPtr == nullptr)
    {
        return;
    }
    *pixelPtr = getPixelColor(color);
}


// Unmaps the frame buffer from memory, closes the buffer file, and clears all
// buffer information.
void FBPainter::FrameBuffer::closeAndClearData()
{
    if (bufferData != nullptr)
    {
        munmap(bufferData, bufferSize);
        bufferData = nullptr;
    }
    if (bufferFD != 0)
    {
        close(bufferFD);
        bufferFD = 0;
    }
    fInfo = {0};
    vInfo = {0};
    bufferSize = 0;
}


// Gets a 32-bit frame buffer color value from RGB color values.
uint32_t FBPainter::FrameBuffer::getPixelColor
(const uint8_t red, const uint8_t green, const uint8_t blue) const
{
    return (red << vInfo.red.offset)
        | (green << vInfo.green.offset)
        | (blue << vInfo.blue.offset);
}


// Gets a 32-bit frame buffer color value from a RGBPixel object.
uint32_t FBPainter::FrameBuffer::getPixelColor(const RGBPixel& pixel) const
{
    return getPixelColor(pixel.red, pixel.green, pixel.blue);
}


// Gets the address in the frame buffer memory map where a specific
// coordinate's pixel color is stored.
uint32_t* FBPainter::FrameBuffer::getMappedPoint
(const size_t xPos, const size_t yPos)
{
    if (bufferData == nullptr || xPos >= getWidth() || yPos >= getHeight())
    {
        std::cerr << "buffer closed or (" << xPos << ", " << yPos
                << ") out of bounds.\n";
        return nullptr;
    }
    size_t offset = (xPos + vInfo.xoffset) * (vInfo.bits_per_pixel / 8)
            + (yPos + vInfo.yoffset) * fInfo.line_length;
    return reinterpret_cast<uint32_t*>(bufferData + offset);
}
