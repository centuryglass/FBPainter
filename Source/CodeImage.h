#pragma once
/**
 * @file  CursorImage.h
 *
 * @brief  Allows image data embedded into C++ source code to be accessed as an
 *         Image object.
 */

#pragma once
#include "../FBPainter.hpp"

namespace FBPainter
{
    template <class ImageData> class CodeImage;
}

/**
 * @brief  Packages image data produced by ImageEncoder within an Image object
 *         wrapper.
 *
 * @tparam ImageData  A class produced by the ImageEncoder utility program that
 *                    holds data extracted from a single image.
 */
template <class ImageData>
class FBPainter::CodeImage : public Image
{
public:
    CodeImage() { }

    virtual ~CodeImage() { }

    /**
     * @brief  Gets the width of the image.
     *
     * @return  Image width in pixels.
     */
    size_t getWidth() const override
    {
        return ImageData::width;
    }

    /**
     * @brief  Gets the height of the image.
     *
     * @return  Image height in pixels.
     */
    size_t getHeight() const override
    {
        return ImageData::height;
    }

    /**
     * @brief  Gets a single pixel's RGBA color value.
     *
     * @param xPos  The pixel's x-coordinate.
     *
     * @param yPos  The pixel's y-coordinate.
     *
     * @return      The color of the pixel at the given coordinate, or a null
     *              RGBAPixel if the given coordinates are outside of the image
     *              bounds.
     */
    RGBAPixel getRGBAPixel(const size_t xPos, const size_t yPos) const override
    {
        return ImageData::getColor(xPos, yPos);
    }

    /**
     * @brief  Gets a single pixel's RGB color value.
     *
     * @param xPos  The pixel's x-coordinate.
     *
     * @param yPos  The pixel's y-coordinate.
     *
     * @return      The color of the pixel at the given coordinate, or a null
     *              RGBPixel if the given coordinates are outside of the image
     *              bounds.
     */
    RGBPixel getRGBPixel(const size_t xPos, const size_t yPos) const override
    {
        return ImageData::getColor(xPos, yPos);
    }

};
