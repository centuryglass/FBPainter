/**
 * @file Image.h
 *
 * @brief  A RGBA png image that is drawn into the FrameBuffer.
 */

#pragma once
#ifndef USE_PNG
    #error "FBPainter::Image class included, but libpng support is disabled."
#endif
#include "Pixel.h"
#include <png++/png.hpp>

namespace FBPainter
{
    class Image;
    class FrameBuffer;
}

class FBPainter::Image
{
public:
    /**
     * @brief  Loads image data on construction.
     *
     * @param imagePath    The path to a PNG image file.
     *
     * @param frameWidth   The width of the frame buffer.
     *
     * @param frameHeight  The height of the frame buffer.
     */
    Image(const char* imagePath, const size_t frameWidth,
            const size_t frameHeight);

    virtual ~Image() { }

    /**
     * @brief  Gets the width of the image.
     *
     * @return  The image width in pixels.
     */
    size_t getWidth() const;

    /**
     * @brief  Gets the height of the image.
     *
     * @return  The image height in pixels.
     */
    size_t getHeight() const;

    /**
     * @brief  Gets the image origin's x-coordinate in the FrameBuffer.
     *
     * @return  The x-coordinates of the image's top left corner within the
     *          frame buffer.
     */
    size_t getImageXOrigin() const;

    /**
     * @brief  Gets the image origin's y-coordinate in the FrameBuffer.
     *
     * @return  The y-coordinates of the image's top left corner within the
     *          frame buffer.
     */
    size_t getImageYOrigin() const;

    /**
     * @brief  Sets the image's origin in the FrameBuffer.
     *
     * @param xPos         The new x-coordinate of the image's top left corner
     *                     in the frame buffer.
     *
     * @param yPos         The new y-coordinate of the image's top left corner
     *                     in the frame buffer.
     *
     * @param frameBuffer  If this buffer pointer is non-null, image data will
     *                     be updated with the change in origin.
     */
    void setImageOrigin(const size_t xPos, const size_t yPos,
            FrameBuffer* frameBuffer = nullptr);

    /**
     * @brief  Draws one image pixel into a FrameBuffer.
     *
     * @param xPos         X-coordinate of a pixel in the image.
     *
     * @param yPos         Y-coordinate of a pixel in the image.
     *
     * @param frameBuffer  Frame buffer where the pixel will be drawn.
     */
    void drawPixel(const size_t xPos, const size_t yPos,
            FrameBuffer* frameBuffer);

    /**
     * @brief  Removes image data from one pixel in a frame buffer.
     *
     * @param xPos         X-coordinate of a pixel in the image.
     *
     * @param yPos         Y-coordinate of a pixel in the image.
     *
     * @param frameBuffer  Frame buffer where the pixel will be cleared.
     */
    void clearPixel(const size_t xPos, const size_t yPos,
            FrameBuffer* frameBuffer);

    /**
     * @brief  Draws the entire image into the frame buffer.
     *
     * @param frameBuffer  The frame buffer object.
     */
    void drawImage(FrameBuffer* frameBuffer);

    /**
     * @brief  Clears drawn image date from the frame buffer.
     *
     * @param frameBuffer  The frame buffer object.
     */
    void clearImage(FrameBuffer* frameBuffer);

private:
    // Image type used to store the source image:
    typedef png::image<RGBAPixel, png::solid_pixel_buffer<RGBAPixel>>
            SourceImage;
    // Image type used to buffer saved frame data:
    typedef png::image<RGBPixel> BufferImage;

    // The loaded image:
    SourceImage sourceImage;
    // Tracks where pixels have already been drawn:
    BufferImage drawnPixels;
    // Tracks pixel values replaced with drawn pixels:
    BufferImage replacedPixels;
    // Holds the image origin within the frame buffer:
    size_t xOrigin = 0;
    size_t yOrigin = 0;
    // Saved FrameBuffer dimensions:
    size_t frameWidth = 0;
    size_t frameHeight = 0;
};

