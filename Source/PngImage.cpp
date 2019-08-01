#include "PngImage.h"


// Loads image data on construction.
FBPainter::PngImage::PngImage(const char* imagePath) : sourceImage(imagePath)
{ }


// Gets the width of the image.
size_t FBPainter::PngImage::getWidth() const 
{
    return sourceImage.get_width();
}


// Gets the height of the image.
size_t FBPainter::PngImage::getHeight() const 
{
    return sourceImage.get_height();
}


// Gets pixel color data at a specific image coordinate.
FBPainter::RGBPixel FBPainter::PngImage::getRGBPixel
(const size_t xPos, const size_t yPos) const 
{
    if (xPos >= getWidth() || yPos >= getHeight())
    {
        return RGBPixel();
    }
    const png::rgba_pixel px = sourceImage.get_pixel(xPos, yPos);
    return RGBPixel(px.red, px.green, px.blue);
}


// Gets pixel color data at a specific image coordinate.
FBPainter::RGBAPixel FBPainter::PngImage::getRGBAPixel
(const size_t xPos, const size_t yPos) const 
{
    if (xPos >= getWidth() || yPos >= getHeight())
    {
        return RGBAPixel();
    }
    const png::rgba_pixel px = sourceImage.get_pixel(xPos, yPos);
    return RGBAPixel(px.red, px.green, px.blue, px.alpha);
}
