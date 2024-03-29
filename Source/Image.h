/**
 * @file Image.h
 *
 * @brief  An abstract basis for image data classes
 */

#pragma once
#include "RGBPixel.h"
#include "RGBAPixel.h"
#include <cstddef>

namespace FBPainter
{
    class Image;
}

class FBPainter::Image
{
public:
    Image() { }

    virtual ~Image() { }

    /**
     * @brief  Gets the width of the image.
     *
     * @return  The image width in pixels.
     */
    virtual size_t getWidth() const = 0;

    /**
     * @brief  Gets the height of the image.
     *
     * @return  The image height in pixels.
     */
    virtual size_t getHeight() const = 0;

    /**
     * @brief  Gets pixel color data at a specific image coordinate.
     *
     * @param xPos  The pixel's x-coordinate.
     *
     * @param yPos  The pixel's y-coordinate.
     *
     * @return      The pixel value at the given coordinate, or
     *              RGBPixel(0, 0, 0) if the coordinate is out of bounds.
     */
    virtual RGBPixel getRGBPixel(const size_t xPos, const size_t yPos)
            const = 0;

    /**
     * @brief  Gets pixel color data at a specific image coordinate.
     *
     * @param xPos  The pixel's x-coordinate.
     *
     * @param yPos  The pixel's y-coordinate.
     *
     * @return      The pixel value at the given coordinate, or
     *              RGBAPixel(0, 0, 0, 0) if the coordinate is out of bounds.
     */
    virtual RGBAPixel getRGBAPixel(const size_t xPos, const size_t yPos)
            const = 0;
};

