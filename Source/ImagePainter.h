/**
 * @file ImagePainter.h
 *
 * @brief  Draws an image into the frame buffer.
 */

#pragma once
#include "Image.h"
#include "RGBPixel.h"
#include "RGBAPixel.h"
#include <memory>

namespace FBPainter
{
    class ImagePainter;
    class FrameBuffer;
}

class FBPainter::ImagePainter
{
public:
    /**
     * @brief  Stores image data on construction.
     *
     * @param image  An image data object, to be deleted when the ImagePainter
     *               is destroyed.
     */
    ImagePainter(Image* image);

    /**
     * @brief  Clears buffered data on destruction.
     */
    virtual ~ImagePainter();

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
            FrameBuffer* const frameBuffer = nullptr);

    /**
     * @brief  Draws the entire image into the frame buffer.
     *
     * @param frameBuffer  The frame buffer object.
     */
    void drawImage(FrameBuffer* const frameBuffer);

    /**
     * @brief  Clears drawn image date from the frame buffer.
     *
     * @param frameBuffer  The frame buffer object.
     */
    void clearImage(FrameBuffer* const frameBuffer);

private:
    /**
     * @brief  Draws one image pixel into a FrameBuffer.
     *
     * @param xPos         X-coordinate of a pixel in the frameBuffer.
     *
     * @param yPos         Y-coordinate of a pixel in the frameBuffer.
     *
     * @param frameBuffer  Frame buffer where the pixel will be drawn.
     */
    void drawPixel(const size_t xPos, const size_t yPos,
            FrameBuffer* const frameBuffer);

    /**
     * @brief  Removes image data from one pixel in a frame buffer.
     *
     * @param xPos         X-coordinate of a pixel in the frameBuffer.
     *
     * @param yPos         Y-coordinate of a pixel in the frameBuffer.
     *
     * @param frameBuffer  Frame buffer where the pixel will be cleared.
     */
    void clearPixel(const size_t xPos, const size_t yPos,
            FrameBuffer* const frameBuffer);

    /**
     * @brief  Gets the index of a pixel in the replaced pixel buffer.
     *
     * @param xPos  The pixel's x-coordinate in the image.
     *
     * @param yPos  The pixel's y-coordinate in the image.
     *
     * @return      The buffer index, or ImagePainter::invalidIndex if the
     *              coordinates are out of bounds.
     */
    size_t bufferIndex(const size_t xPos, const size_t yPos) const;

    /**
     * @brief  Checks if a coordinate is outside either the framebuffer bounds
     *         or the image bounds.
     *
     * @param xPos         X-coordinate of a pixel in the frameBuffer.
     *
     * @param yPos         Y-coordinate of a pixel in the frameBuffer.
     *
     * @param frameBuffer  Frame buffer used to check buffer size.
     *
     * @return             Whether the given coordinate is within both sets of
     *                     bounds.
     */
    bool outOfBounds(const size_t xPos, const size_t yPos,
            FrameBuffer* const frameBuffer);

    // Source image data:
    std::unique_ptr<Image> image;
    // Saved image dimensions:
    size_t imageWidth = 0;
    size_t imageHeight = 0;
    // Holds the image origin within the frame buffer:
    size_t xOrigin = 0;
    size_t yOrigin = 0;
    // Stores framebuffer pixels overwritten by the image, so that they can be
    // restored when the image is moved or cleared:
    RGBPixel* replacedPixels = nullptr;
    // Represents an invalid index:
    static const size_t invalidIndex;
};

