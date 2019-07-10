#include "Image.h"
#include "FrameBuffer.h"
#include <algorithm>


// Loads image data on construction.
Image::Image
(const char* imagePath, const size_t frameWidth, const size_t frameHeight):
    sourceImage(imagePath),
    drawnPixels(frameWidth, frameHeight),
    replacedPixels(frameWidth, frameHeight),
    frameWidth(frameWidth),
    frameHeight(frameHeight) { }


// Gets the width of the image.
size_t Image::getWidth() const
{
    return sourceImage.get_width();
}


// Gets the height of the image.
size_t Image::getHeight() const
{
    return sourceImage.get_height();
}


// Gets the image's origin's x-coordinate in the FrameBuffer.
size_t Image::getImageXOrigin() const
{
    return xOrigin;
}


// Gets the image's origin's y-coordinate in the FrameBuffer.
size_t Image::getImageYOrigin() const
{
    return yOrigin;
}


// Sets the image's origin in the FrameBuffer.
void Image::setImageOrigin(const size_t xPos, const size_t yPos,
        FrameBuffer* frameBuffer)
{
    if (xPos == xOrigin && yPos == yOrigin)
    {
        return;
    }
    if (frameBuffer != nullptr)
    {
        const size_t oldXMax = std::min(xOrigin + getWidth(), frameWidth);
        const size_t oldYMax = std::min(yOrigin + getHeight(), frameHeight);
        const size_t newXMax = std::min(xPos + getWidth(), frameWidth);
        const size_t newYMax = std::min(yPos + getHeight(), frameHeight);
        for (int y = yOrigin; y < oldYMax; y++)
        {
            const bool yOverlap = (y >= yPos && y < newYMax);
            for (int x = xOrigin; x < oldXMax; x++)
            {
                const bool xOverlap = (x >= xPos && x < newXMax);
                if(! yOverlap || ! xOverlap)
                {
                    clearPixel(x, y, frameBuffer);
                }
                else
                {
                    RGBAPixel imagePixel = sourceImage.get_pixel(x - xPos,
                            y - yPos);
                    if (imagePixel.alpha == 0)
                    {
                        clearPixel(x, y, frameBuffer);
                    }
                }
            }
        }
    }
    xOrigin = xPos;
    yOrigin = yPos;
    if (frameBuffer != nullptr)
    {
        drawImage(frameBuffer);
    }
}

// Checks if two RGB pixel colors are equal.
static bool equals(const Image::RGBPixel& lhs, const Image::RGBPixel& rhs)
{
    return lhs.red == rhs.red && lhs.green == rhs.green
            && lhs.blue == rhs.blue;
}

// Checks if a RGB pixel color has all zero values.
static bool isNull(const Image::RGBPixel& pixel)
{
    return pixel.red == 0 && pixel.green == 0 && pixel.blue == 0;
}

// Draws one image pixel into a FrameBuffer.
void Image::drawPixel
(const size_t xPos, const size_t yPos, FrameBuffer* frameBuffer)
{
    // Ignore pixels outside of the image bounds:
    if (xPos >= getWidth() || yPos >= getHeight())
    {
        return;
    }
    const size_t frameX = xPos + xOrigin;
    const size_t frameY = yPos + yOrigin;
    // Ignore pixels outside of the frame bounds:
    if (frameX >= frameWidth || frameY >= frameHeight)
    {
        return;
    }

    const RGBAPixel sourcePixel = sourceImage.get_pixel(xPos, yPos);
    if (sourcePixel.alpha == 0) // Don't draw transparent image pixels.
    {
        clearPixel(xPos, yPos, frameBuffer);
        return;
    }

    const RGBPixel bufferPixel = frameBuffer->getPixel(frameX, frameY);
    const RGBPixel replacedPixel = replacedPixels.get_pixel(frameX, frameY);
    RGBPixel pixelToDraw(sourcePixel.red, sourcePixel.green, sourcePixel.blue);
    if (sourcePixel.alpha < 255)
    {
        const RGBPixel& coveredPixel = (isNull(replacedPixel))
                ? bufferPixel : replacedPixel;
        const uint8_t coveredAlpha = 255 - sourcePixel.alpha;
        pixelToDraw.red = (sourcePixel.red * sourcePixel.alpha / 255)
                + (coveredPixel.red * coveredAlpha / 255);
        pixelToDraw.green = (sourcePixel.green * sourcePixel.alpha / 255)
                + (coveredPixel.green * coveredAlpha / 255);
        pixelToDraw.blue = (sourcePixel.blue * sourcePixel.alpha / 255)
                + (coveredPixel.blue * coveredAlpha / 255);
    }

    // Ignore pixels that already match the frame buffer pixel:
    if (equals(pixelToDraw, bufferPixel))
    {
        return;
    }
    // Update the frame buffer pixel, saving the old frame buffer pixel:
    const RGBPixel lastDrawnPixel = drawnPixels.get_pixel(frameX, frameY);
    frameBuffer->setPixel(frameX, frameY, pixelToDraw);
    drawnPixels.set_pixel(frameX, frameY, pixelToDraw);
    if (! equals(lastDrawnPixel, bufferPixel))
    {
        replacedPixels.set_pixel(frameX, frameY, bufferPixel);
    }
}


// Removes image data from one pixel in a frame buffer.
void Image::clearPixel(const size_t xPos, const size_t yPos,
        FrameBuffer* frameBuffer)
{
    if (xPos >= frameWidth || yPos >= frameHeight)
    {
        return;
    }
    RGBPixel oldPixel = replacedPixels.get_pixel(xPos, yPos);
    if (isNull(oldPixel))
    {
        return;
    }
    RGBPixel drawnPixel = drawnPixels.get_pixel(xPos, yPos);
    RGBPixel bufferPixel = frameBuffer->getPixel(xPos, yPos);
    if (equals(drawnPixel, bufferPixel))
    {
        frameBuffer->setPixel(xPos, yPos, oldPixel);
    }
    const RGBPixel nullPixel(0, 0, 0);
    drawnPixels.set_pixel(xPos, yPos, nullPixel);
    replacedPixels.set_pixel(xPos, yPos, nullPixel);
}


// Draws the entire image into the frame buffer.
void Image::drawImage(FrameBuffer* frameBuffer)
{
    const size_t xMax = std::min(frameBuffer->getWidth() - xOrigin,
            getWidth());
    const size_t yMax = std::min(frameBuffer->getHeight() - yOrigin,
            getHeight());
    for (int y = 0; y < yMax; y++)
    {
        for (int x = 0; x < xMax; x++)
        {
            drawPixel(x, y, frameBuffer);
        }
    }
}


// Clears drawn image date from the frame buffer.
void Image::clearImage(FrameBuffer* frameBuffer)
{
    for (int y = 0; y < getHeight(); y++)
    {
        for (int x = 0; x < getWidth(); x++)
        {
            clearPixel(x, y, frameBuffer);
        }
    }
}
