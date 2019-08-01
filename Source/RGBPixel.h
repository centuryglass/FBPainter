/**
 * @file  RGBPixel.h
 *
 * @brief  Represents an RGB pixel color value.
 */

#pragma once
#include <stdint.h>

namespace FBPainter
{
    class RGBPixel;
}


/**
 * @brief  Represents a nullable pixel color without an alpha value.
 *
 *  RGBPixel represents colors using eight-bit RGB color components. RGBPixel
 * objects may be null, in which case they instead represent a lack of pixel
 * data.
 */
class FBPainter::RGBPixel
{
public:
    /**
     * @brief  Creates a non-null RGBPixel.
     *
     * @param r  The pixel's red color component.
     *
     * @param g  The pixel's green color coordinate.
     *
     * @param b  The pixel's blue color coordinate.
     */
    RGBPixel(uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief  Creates a null pixel value.
     */
    RGBPixel() { }

    virtual ~RGBPixel() { }

    /**
     * @brief  Checks if this object represents a lack of pixel data.
     *
     * @return  Whether the pixel is null.
     */
    bool isNull() const;

    /**
     * @brief  Gets the pixel's red color component.
     *
     * @return  The eight-bit red color component value.
     */
    uint8_t getRed() const;

    /**
     * @brief  Gets the pixel's green color component.
     *
     * @return  The eight-bit green color component value.
     */
    uint8_t getGreen() const;

    /**
     * @brief  Gets the pixel's blue color component.
     *
     * @return  The eight-bit blue color component value.
     */
    uint8_t getBlue() const;

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
    bool operator!=(const RGBPixel& rhs) const;

private:
    bool nullPixel = true;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
};
