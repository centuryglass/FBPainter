#include "Cursor.h"

// All image colors, as an array of RGBA color components.
static const constexpr unsigned char colors [4][4] =
{
    {0, 0, 0, 255},
    {0, 255, 41, 0},
    {214, 27, 27, 153},
    {0, 0, 0, 222}
};

// All image data, stored in a string of color indices starting at index 'a'.
static const constexpr char* imageData = "aaabbbbbbbbbbb"
                                         "acdabbbbbbbbbb"
                                         "adcaaabbbbbbbb"
                                         "baaccaaabbbbbb"
                                         "bbaccccaaabbbb"
                                         "bbaacccccaaabb"
                                         "bbbacccccccaaa"
                                         "bbbaaccccccaab"
                                         "bbbbacccccaabb"
                                         "bbbbaaccccaabb"
                                         "bbbbbaccaacaab"
                                         "bbbbbaaaaaacaa"
                                         "bbbbbbaabbaaab"
                                         "bbbbbbabbbbabb";

// Gets the color of an image pixel.
FBPainter::RGBAPixel FBPainter::Cursor::getColor
(const size_t x, const size_t y)
{
    if (x >= width || y >= height)
    {
        return RGBAPixel();
    }
    size_t charIdx = y * width + x;
    size_t idx = imageData[charIdx] - 'a';
    return RGBAPixel(colors[idx][0], colors[idx][1], colors[idx][2],
            colors[idx][3]);
}