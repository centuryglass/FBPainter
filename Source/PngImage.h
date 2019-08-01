/**
 * @file PngImage.h
 *
 * @brief  An image object that loads its data from .png files using libpng.
 */

#ifndef USE_PNG
    #error "FBPainter::PngImage class included, but libpng support is disabled."
#endif
#include "Image.h"
#include <png++/png.hpp>

namespace FBPainter
{
    class Image;
    class PngImage;
}

class FBPainter::PngImage : public Image
{
public:
    /**
     * @brief  Loads image data on construction.
     *
     * @param imagePath    The path to a PNG image file.
     */
    PngImage(const char* imagePath);

    virtual ~PngImage() { }

    /**
     * @brief  Gets the width of the image.
     *
     * @return  The image width in pixels.
     */
    size_t getWidth() const override;

    /**
     * @brief  Gets the height of the image.
     *
     * @return  The image height in pixels.
     */
    size_t getHeight() const override;


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
    RGBPixel getRGBPixel(const size_t xPos, const size_t yPos) const override;

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
    RGBAPixel getRGBAPixel(const size_t xPos, const size_t yPos) const override;

private:
    typedef png::rgba_pixel RGBApng;
    // Image type used to store the source image:
    typedef png::image<RGBApng, png::solid_pixel_buffer<RGBApng>> SourceImage;
    // The loaded image:
    SourceImage sourceImage;
};

