#include "RGBAPixel.h"


// Creates a non-null RGBAPixel.
FBPainter::RGBAPixel::RGBAPixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
        RGBPixel(r, g, b), alpha(a) { }


// Creates a RGBAPixel from a RGBPixel.
FBPainter::RGBAPixel::RGBAPixel(const RGBPixel& rgbPixel, uint8_t alpha) :
    RGBPixel(rgbPixel), alpha(alpha)
{
    if (rgbPixel.isNull())
    {
        alpha = 0;
    }
}

// Combines two color components, applying an alpha value to the top component.
static inline uint8_t combineComp(uint8_t cTop, uint8_t cBottom, uint8_t alpha)
{
    using cLimit = std::numeric_limits<uint8_t>;
    return (cTop * alpha / cLimit::max()) + (cBottom * (cLimit::max() - alpha)
            / cLimit::max());
}


// Gets the RGBPixel color value created by displaying this pixel over a fully
// opaque background pixel.
FBPainter::RGBPixel FBPainter::RGBAPixel::getCombinedPixel
(const RGBPixel& bgPixel) const
{
    if (isNull())
    {
        return bgPixel;
    }
    if (isOpaque())
    {
        return RGBPixel(getRed(), getGreen(), getBlue());
    }
    return RGBPixel(combineComp(getRed(), bgPixel.getRed(), alpha),
            combineComp(getGreen(), bgPixel.getGreen(), alpha),
            combineComp(getBlue(), bgPixel.getBlue(), alpha));
}


// Checks if the pixel is fully opaque, with zero transparency.
bool FBPainter::RGBAPixel::isOpaque() const
{
    return alpha == std::numeric_limits<uint8_t>::max();
}


// Checks if the pixel is fully transparent.
bool FBPainter::RGBAPixel::isTransparent() const
{
    return alpha == 0;
}



// Gets the pixel's alpha color component.
uint8_t FBPainter::RGBAPixel::getAlpha() const
{
    return alpha;
}

// Checks if two pixel objects are equivalent.
bool FBPainter::RGBAPixel::operator==(const RGBAPixel& rhs) const
{
    if (isNull())
    {
        return rhs.isNull();
    }
    if (rhs.isNull())
    {
        return false;
    }
    return getRed() == rhs.getRed()
        && getGreen() == rhs.getGreen()
        && getBlue() == rhs.getBlue()
        && getAlpha() == rhs.getAlpha();
}


// Checks if two pixel objects are equivalent.
bool FBPainter::RGBAPixel::operator==(const RGBPixel& rhs) const
{
    if (isNull())
    {
        return rhs.isNull();
    }
    if (rhs.isNull() || ! isOpaque())
    {
        return false;
    }
    return getRed() == rhs.getRed()
        && getGreen() == rhs.getGreen()
        && getBlue() == rhs.getBlue();
}


// Checks if two pixel objects are not equivalent.
bool FBPainter::RGBAPixel::operator!=(const RGBAPixel& rhs) const
{
    return ! (*this == rhs);
}


// Checks if two pixel objects are not equivalent.
bool FBPainter::RGBAPixel::operator!=(const RGBPixel& rhs) const
{
    return ! (*this == rhs);
}
