/**
 * @file  RGBAPixel.h
 *
 * @brief  Represents an RGBA pixel color value.
 */

#pragma once
#include "RGBPixel.h"
#include <limits>

namespace FBPainter
{
    class RGBAPixel;
}


class FBPainter::RGBAPixel : public RGBPixel
{
public:
    /**
     * @brief  Creates a non-null RGBAPixel.
     *
     * @param r  The pixel's red color component.
     *
     * @param g  The pixel's green color coordinate.
     *
     * @param b  The pixel's blue color coordinate.
     *
     * @param a  The pixel's alpha color coordinate.
     */
    RGBAPixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    /**
     * @brief  Creates a RGBAPixel from a RGBPixel.
     *
     * @param rgbPixel  The RGBPixel to copy.
     *
     * @param alpha     An optional alpha component to give the new pixel. If
     *                  not specified, the new pixel will be opaque.
     */
    RGBAPixel(const RGBPixel& rgbPixel,
            uint8_t alpha = std::numeric_limits<uint8_t>::max());

    /**
     * @brief  Creates a null pixel value.
     */
    RGBAPixel() { }

    virtual ~RGBAPixel() { }

    /**
     * @brief  Gets the RGBPixel color value created by displaying this pixel
     *         over a fully opaque background pixel.
     *
     * @param bgPixel  A background pixel value to cover with this pixel.
     *
     * @return         The new pixel value, or backgroundPixel if this pixel is
     *                 null.
     */
    RGBPixel getCombinedPixel(const RGBPixel& bgPixel) const;

    /**
     * @brief  Checks if the pixel is fully opaque, with zero transparency.
     *
     * @return  Whether the pixel's alpha component is at the maximum level.
     */
    bool isOpaque() const;

    /**
     * @brief  Checks if the pixel is fully transparent.
     *
     * @return  Whether the pixel's alpha component is at zero.
     */
    bool isTransparent() const;

    /**
     * @brief  Gets the pixel's alpha color component.
     *
     * @return  The eight-bit alpha color component value.
     */
    uint8_t getAlpha() const;

    /**
     * @brief  Checks if two pixel objects are equivalent.
     *
     * @param rhs  Another pixel object to compare with this one.
     *
     * @return     True if both pixels are null, or if both are non-null and
     *             have identical color components.
     */
    bool operator==(const RGBAPixel& rhs) const;

    /**
     * @brief  Checks if two pixel objects are equivalent.
     *
     * @param rhs  Another pixel object to compare with this one.
     *
     * @return     True if both pixels are null, or if both are non-null and
     *             have identical color components.
     */
    bool operator==(const RGBPixel& rhs) const;

    /**
     * @brief  Checks if two pixel objects are not equivalent.
     *
     * @param rhs  Another pixel object to compare with this one.
     *
     * @return     True if one pixel is null and the other is not, or if both
     *             are non-null and do not have identical color components.
     */
    bool operator!=(const RGBAPixel& rhs) const;

    /**
     * @brief  Checks if two pixel objects are not equivalent.
     *
     * @param rhs  Another pixel object to compare with this one.
     *
     * @return     True if one pixel is null and the other is not, or if both
     *             are non-null and do not have identical color components.
     */
    bool operator!=(const RGBPixel& rhs) const;

private:
    uint8_t alpha = 0;
};
