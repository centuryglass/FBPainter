/**
 * @file  Pixel.h
 *
 * @brief  Provides definitions for RGBPixel and RGBAPixel data structures.
 */
#pragma once

// If images are enabled, use libpng++ pixel data structures. Otherwise, define
// basic replacements here.

#ifdef USE_PNG
#include <png++/png.hpp>
namespace FBPainter
{
    typedef png::rgb_pixel RGBPixel;
    typedef png::rgba_pixel RGBAPixel;
}

#else
#include <stdint.h>

namespace FBPainter
{
    struct RGBPixel
    {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;

        RGBPixel(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g),
                blue(b) { }
    };

    struct RGBAPixel
    {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
        uint8_t alpha = 0;

        RGBAPixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : red(r), green(g),
                blue(b), alpha(a) { }
    };
}
#endif
