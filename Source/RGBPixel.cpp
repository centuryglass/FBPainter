#include "RGBPixel.h"


// Creates a non-null RGBPixel.
FBPainter::RGBPixel::RGBPixel(uint8_t r, uint8_t g, uint8_t b) :
    nullPixel(false), red(r), green(g), blue(b) { }


// Checks if this object represents a lack of pixel data.
bool FBPainter::RGBPixel::isNull() const
{
    return nullPixel;
}


// Gets the pixel's red color component.
uint8_t FBPainter::RGBPixel::getRed() const
{
    return red;
}


// Gets the pixel's green color component.
uint8_t FBPainter::RGBPixel::getGreen() const
{
    return green;
}


// Gets the pixel's blue color component.
uint8_t FBPainter::RGBPixel::getBlue() const
{
    return blue;
}


// Checks if two pixel objects are equivalent.
bool FBPainter::RGBPixel::operator==(const RGBPixel& rhs) const
{
    if (nullPixel)
    {
        return rhs.nullPixel;
    }
    if (rhs.nullPixel)
    {
        return false;
    }
    return red == rhs.red && green == rhs.green && blue == rhs.blue;
}


// Checks if two pixel objects are not equivalent.
bool FBPainter::RGBPixel::operator!=(const RGBPixel& rhs) const
{
    return ! (*this == rhs);
}
