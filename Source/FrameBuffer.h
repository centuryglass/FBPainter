/**
 * @file  FrameBuffer.h
 *
 * @brief  Draws directly to a Linux framebuffer.
 */

#pragma once
#include <png++/png.hpp>
#include <linux/fb.h>
#include <stdint.h>
#include <stddef.h>

class FrameBuffer
{
public:
    // LibPNG pixel color type:
    typedef png::rgb_pixel RGBPixel;

    /**
     * @brief  Opens and memory maps the frame buffer file.
     *
     * @param bufferPath  The path to the frame buffer file.
     */
    FrameBuffer(const char* bufferPath);

    /**
     * @brief  Closes and unmaps the frame buffer file on destruction.
     */
    ~FrameBuffer();

    /**
     * @brief  Checks if the buffer is open and ready for IO.
     *
     * @return  Whether the frame buffer is open and memory mapped.
     */
    bool isBufferOpen() const;

    /**
     * @brief  Gets the frame buffer's width.
     *
     * @return  The buffer width, measured in pixels.
     */
    size_t getWidth() const;

    /**
     * @brief  Gets the frame buffer's height.
     *
     * @return  The buffer height, measured in pixels.
     */
    size_t getHeight() const;

    /**
     * @brief  Gets the color set at a specific pixel in the buffer.
     *
     * @param xPos  The x-coordinate of the pixel to read.
     *
     * @param yPos  The y-coordinate of the pixel to read.
     *
     * @return      The color at that pixel coordinate, or RGBPixel(0, 0, 0) if
     *              the coordinate is not within the buffer's bounds.
     */
    RGBPixel getPixel(const size_t xPos, const size_t yPos);

    /**
     * @brief  Sets the color at a specific coordinate in the buffer.
     *
     * This will do nothing if the coordinate given is out of bounds.
     *
     * @param xPos   The x-coordinate of the pixel to update.
     *
     * @param yPos   The y-coordinate of the pixel to update.
     *
     * @param color  The new color value to set.
     */
    void setPixel(const size_t xPos, const size_t yPos, const RGBPixel color);

    /**
     * @brief  Unmaps the frame buffer from memory, closes the buffer file, and
     *         clears all buffer information.
     */
    void closeAndClearData();

private:
    /**
     * @brief  Gets a 32-bit frame buffer color value from RGB color values.
     *
     * @param red    The red component of the color.
     *
     * @param green  The green component of the color.
     *
     * @param blue   The blue component of the color.
     *
     * @return       A color value that can be copied directly into the buffer.
     */
    uint32_t getPixelColor(const uint8_t red, const uint8_t green,
            const uint8_t blue) const;

    /**
     * @brief  Gets a 32-bit frame buffer color value from a RGBPixel object.
     *
     * @param pixel  The pixel color data to convert.
     *
     * @return       A color value that can be copied directly into the buffer.
     */
    uint32_t getPixelColor(const RGBPixel& pixel) const;

    /**
     * @brief  Gets the address in the frame buffer memory map where a specific
     *         coordinate's pixel color is stored.
     *
     * @param xPos   The pixel x-coordinate.
     *
     * @param yPos   The pixel y-coordinate.
     *
     * @return       An address within the buffer, or nullptr if the buffer is
     *               closed or the point is out of bounds.
     */
    uint32_t* getMappedPoint(const size_t xPos, const size_t yPos);

    // Stored display info:
	struct fb_fix_screeninfo fInfo = {0};
	struct fb_var_screeninfo vInfo = {0};

    // FrameBuffer file descriptor:
	int bufferFD = 0;

    // FrameBuffer file mapped to memory with mmap:
    uint8_t* bufferData = nullptr;
    size_t bufferSize = 0;
};
