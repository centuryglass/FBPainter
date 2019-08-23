#include "ImagePainter.h"
#include "FrameBuffer.h"
#include <algorithm>
#include <new>
#include <limits>

// Represents an invalid index:
const size_t FBPainter::ImagePainter::invalidIndex
        = std::numeric_limits<size_t>::max();

// Stores image data on construction.
FBPainter::ImagePainter::ImagePainter(Image* image) : image(image)
{
    if (image != nullptr)
    {
        imageWidth = image->getWidth();
        imageHeight = image->getHeight();
    }
    replacedPixels = new (std::nothrow) RGBPixel [imageWidth * imageHeight];
    if (replacedPixels == nullptr)
    {
        this->image.reset(nullptr);
    }
}


// Clears buffered data on destruction.
FBPainter::ImagePainter::~ImagePainter()
{
    if (replacedPixels != nullptr)
    {
        delete [] replacedPixels;
    }
}

// Gets the width of the image.
size_t FBPainter::ImagePainter::getWidth() const
{
    return imageWidth;
}


// Gets the height of the image.
size_t FBPainter::ImagePainter::getHeight() const
{
    return imageHeight;
}


// Gets the image's origin's x-coordinate in the FrameBuffer.
size_t FBPainter::ImagePainter::getImageXOrigin() const
{
    return xOrigin;
}


// Gets the image's origin's y-coordinate in the FrameBuffer.
size_t FBPainter::ImagePainter::getImageYOrigin() const
{
    return yOrigin;
}


// Draws the entire image into the frame buffer.
void FBPainter::ImagePainter::drawImage(FrameBuffer* const frameBuffer)
{
    if (image == nullptr || frameBuffer == nullptr)
    {
        return;
    }
    const size_t frameWidth = frameBuffer->getWidth();
    const size_t frameHeight = frameBuffer->getHeight();
    const size_t xMax = std::min(frameWidth, xOrigin + imageWidth) - 1;
    const size_t yMax = std::min(frameHeight, yOrigin + imageHeight) - 1;
    for (int y = yOrigin; y <= yMax; y++)
    {
        {
            for (int x = xOrigin; x <= xMax; x++)
            {
                drawPixel(x, y, frameBuffer);
            }
        }
    }
}


// Clears drawn image date from the frame buffer.
void FBPainter::ImagePainter::clearImage(FrameBuffer* const frameBuffer)
{
    if (image == nullptr || frameBuffer == nullptr)
    {
        return;
    }
    const size_t frameWidth = frameBuffer->getWidth();
    const size_t frameHeight = frameBuffer->getHeight();
    const size_t xMax = std::min(frameWidth, xOrigin + imageWidth) - 1;
    const size_t yMax = std::min(frameHeight, yOrigin + imageHeight) - 1;
    for (int y = yOrigin; y <= yMax; y++)
    {
        {
            for (int x = xOrigin; x <= xMax; x++)
            {
                clearPixel(x, y, frameBuffer);
            }
        }
    }
}


// Sets the image's origin in the FrameBuffer.
void FBPainter::ImagePainter::setImageOrigin(const size_t xPos,
        const size_t yPos, FrameBuffer* const frameBuffer)
{
    if (image == nullptr || (xPos == xOrigin && yPos == yOrigin))
    {
        return;
    }
    if (frameBuffer == nullptr)
    {
        xOrigin = xPos;
        yOrigin = yPos;
        return;
    }
    const size_t oldXMax = std::min(xOrigin + imageWidth,
            frameBuffer->getWidth()) - 1;
    const size_t oldYMax = std::min(yOrigin + imageHeight,
            frameBuffer->getHeight()) - 1;
    const size_t newXMax = std::min(xPos + imageWidth,
            frameBuffer->getWidth()) - 1;
    const size_t newYMax = std::min(yPos + imageHeight,
            frameBuffer->getHeight()) - 1;
    for (int y = yOrigin; y <= oldYMax; y++)
    {
        const bool yOverlap = (y >= yPos && y <= newYMax);
        for (int x = xOrigin; x <= oldXMax; x++)
        {
            const bool xOverlap = (x >= xPos && x <= newXMax);
            if(! yOverlap || ! xOverlap)
            {
                clearPixel(x, y, frameBuffer);
            }
            else
            {
                RGBAPixel imagePixel = image->getRGBAPixel(x - xPos, y - yPos);
                if (imagePixel.isTransparent())
                {
                    clearPixel(x, y, frameBuffer);
                }
            }
        }
    }
    xOrigin = xPos;
    yOrigin = yPos;
    drawImage(frameBuffer);
}


// Draws one image pixel into a FrameBuffer.
void FBPainter::ImagePainter::drawPixel
(const size_t xPos, const size_t yPos, FrameBuffer* const frameBuffer)
{
    // Ignore pixels outside of the frame or image bounds:
    if (outOfBounds(xPos, yPos, frameBuffer))
    {
        return;
    }
    const size_t imageX = xPos - xOrigin;
    const size_t imageY = yPos - yOrigin;
    const RGBAPixel sourcePixel = image->getRGBAPixel(imageX, imageY);
    if (sourcePixel.isTransparent())
    {
        clearPixel(xPos, yPos, frameBuffer);
        return;
    }

    // If relevant, apply transparency to get the new pixel color:
    const RGBPixel bufferPixel = frameBuffer->getPixel(xPos, yPos);
    const int pixelIdx = bufferIndex(imageX, imageY);
    const RGBPixel& replacedPixel = replacedPixels[pixelIdx];
    RGBPixel pixelToDraw = sourcePixel.getCombinedPixel(
            replacedPixel.isNull() ? bufferPixel : replacedPixel);
    // Ignore pixels that already match the frame buffer pixel:
    if (pixelToDraw == bufferPixel)
    {
        return;
    }

    // Update the frame buffer pixel, saving the old frame buffer pixel to the
    // replaced pixel buffer:
    if (replacedPixel.isNull())
    {
        replacedPixels[pixelIdx] = bufferPixel;
    }
    frameBuffer->setPixel(xPos, yPos, pixelToDraw);
}


// Removes image data from one pixel in a frame buffer.
void FBPainter::ImagePainter::clearPixel(const size_t xPos, const size_t yPos,
        FrameBuffer* const frameBuffer)
{
    // Ignore pixels outside of the frame or image bounds:
    if (outOfBounds(xPos, yPos, frameBuffer))
    {
        return;
    }
    const size_t imageX = xPos - xOrigin;
    const size_t imageY = yPos - yOrigin;
    const int pixelIdx = bufferIndex(imageX, imageY);
    const RGBPixel& oldPixel = replacedPixels[pixelIdx];
    if (oldPixel.isNull())
    {
        return;
    }
    frameBuffer->setPixel(xPos, yPos, oldPixel);
    replacedPixels[pixelIdx] = RGBPixel();
}


// Gets the index of a pixel in the replaced pixel buffer.
size_t FBPainter::ImagePainter::bufferIndex
(const size_t xPos, const size_t yPos) const
{
    if (xPos >= imageWidth || yPos >= imageHeight)
    {
        return invalidIndex;
    }
    return yPos * imageWidth + xPos;
}


// Checks if a coordinate is outside either the framebuffer bounds or the image
// bounds.
bool FBPainter::ImagePainter::outOfBounds(const size_t xPos, const size_t yPos,
        FrameBuffer* const frameBuffer)
{
    if (image == nullptr || frameBuffer == nullptr)
    {
        return true;
    }
    const size_t frameWidth = frameBuffer->getWidth();
    const size_t frameHeight = frameBuffer->getHeight();
    const size_t xMax = std::min(frameWidth, xOrigin + imageWidth) - 1;
    const size_t yMax = std::min(frameHeight, yOrigin + imageHeight) - 1;

    return xPos > xMax || yPos > yMax || xPos < xOrigin || yPos < yOrigin;
}
