/**
 * @file  ImageEncoder.cpp
 *
 * @brief  Encodes .png image data into C++ source code.
 */

#include <png++/png.hpp>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <iostream>

typedef png::rgba_pixel Pixel;
typedef png::image<Pixel, png::solid_pixel_buffer<Pixel>> Image;

/**
 * @brief  Creates a source and header file for a single .png image.
 *
 * The created files will share the name and path of the image, with file
 * extensions changed appropriately.
 *
 * @param imgPath  The path to a .png image file.
 *
 * @return         Whether the image was encoded successfully.
 */
bool testEncode(const std::string& imgPath)
{
    using std::string;
    // Load image data:
    Image src;
    try
    {
        src.read(imgPath);
    }
    catch (const png::std_error& e)
    {
        std::cerr << "Error reading \"" << imgPath << "\":" << e.what()
                << "\n";
        return false;
    }
    const size_t width = src.get_width();
    const size_t height = src.get_height();

    // Find and store all unique image pixel colors:
    std::vector<Pixel> colorList;
    const auto colorIndex = [&colorList](const Pixel& color)
    {
        for (int i = 0; i < colorList.size(); i++)
        {
            if (colorList[i].red == color.red
                    && colorList[i].green == color.green
                    && colorList[i].blue == color.blue
                    && colorList[i].alpha == color.alpha)
            {
                return i;
            }
        }
        return -1;
    };

    // Map image colors and stringify image data:
    std::string imageData;
    imageData.reserve(width * height + 1);
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            const Pixel pixelColor = src.get_pixel(x, y);
            int cIndex = colorIndex(pixelColor);
            if (cIndex == -1)
            {
                cIndex = colorList.size();
                colorList.push_back(pixelColor);
            }
            imageData += ('a' + cIndex);
        }
    }

    // Generate output files:
    const auto writeFile = [](const string& filePath, const auto writeAction)
    {
        std::ofstream outFile(filePath);
        if (! outFile.is_open())
        {
            std::cerr << "Couldn't open \"" << filePath << "\" for writing.\n";
            return false;
        }
        try
        {
            writeAction(outFile);
        }
        catch(const std::ofstream::failure& e)
        {
            std::cerr << "Error when writing to \"" << filePath << "\":"
                    << e.what() << "\n";
            outFile.close();
            return false;
        }
        outFile.close();
        std::cout << "Finished writing to \"" << filePath << "\"\n";
        return true;
    };

    // Find file and class names:
    const size_t extensionIdx = imgPath.rfind(".");
    string baseName = imgPath;
    if (extensionIdx != std::string::npos)
    {
        baseName.erase(extensionIdx);
    }
    const string sourcePath = baseName + ".cpp";
    const string headerPath = baseName + ".h";
    const size_t pathIdx = baseName.rfind("/");
    if (pathIdx != std::string::npos)
    {
        baseName.erase(0, pathIdx + 1);
    }

    // Write image header:
    const auto writeHeader = [&imageData, &colorList, &baseName, width, height]
    (std::ofstream& header)
    {
        string indent = "    ";
        const int size = colorList.size();
        header << "/**\n"
                << " * @file " << baseName << ".h\n"
                << " *\n"
                << " * @brief  Image data from " << baseName << ".png"
                << " encoded into a static class.\n */\n\n"
                << "#pragma once\n"
                << "#include \"../FBPainter.hpp\"\n"
                << "#include <cstddef>\n"
                << "#include <limits>\n\n"
                << "namespace FBPainter\n{\n"
                << indent << "class " << baseName << "\n" << indent
                << "{\n" << indent << "public:\n";
        indent += "    ";
        header << indent << "// Represents an invalid index:\n"
                << indent << "static const constexpr size_t npos = "
                << "std::numeric_limits<size_t>::max();\n\n"
                << indent << "// Number of distinct image colors:\n"
                << indent << "static const constexpr size_t numColors = "
                << size << ";\n\n"
                << indent << "// Image width in pixels:\n"
                << indent << "static const constexpr size_t width = " << width
                << ";\n\n"
                << indent << "// Image height in pixels:\n"
                << indent << "static const constexpr size_t height = " << height
                << ";\n\n"
                << indent << "/**\n"
                << indent << " * @brief  Gets the color of an image pixel.\n"
                << indent << " *\n"
                << indent << " * @param x  The pixel's x-coordinate.\n"
                << indent << " *\n"
                << indent << " * @param y  The pixel's y-coordinate.\n"
                << indent << " *\n"
                << indent << " * @return   The color of that pixel, or a null "
                << "RGBAPixel if the\n"
                << indent << " *           coordinates are invalid.\n"
                << indent << " */\n"
                << indent << "static RGBAPixel getColor(const size_t x,"
                << "const size_t y);\n";
        indent.erase(indent.length() / 2);
        header << indent << "};\n}";
    };

    // Write image source:
    const auto writeSrc = [&imageData, &colorList, &baseName, width, height]
    (std::ofstream& source)
    {
        const int size = colorList.size();
        source << "#include \"" << baseName << ".h\"\n\n"
                << "// All image colors, as an array of RGBA color "
                << "components.\n"
                << "static const constexpr unsigned char colors ["
                << size << "][4] =\n{";
        for (int i = 0; i < size; i++)
        {
            if (i > 0)
            {
                source << ',';
            }
            source << "\n    {" <<  (int) colorList[i].red << ", "
                    << (int) colorList[i].green << ", "
                    << (int) colorList[i].blue << ", "
                    << (int) colorList[i].alpha << "}";
        }
        source << "\n};\n\n// All image data, stored in a string of color"
                << " indices starting at index 'a'.\n";
        const std::string dataDeclaration
                = "static const constexpr char* imageData = ";
        const std::string indent(dataDeclaration.length(), ' ');
        source << dataDeclaration;
        for (int i = 0; i < imageData.length(); i += width)
        {
            if (i > 0)
            {
                source << "\n" << indent;
            }
            const std::string dataRow = imageData.substr(i, width);
            source << "\"" << dataRow << "\"";
        }
        source << ";\n\n// Gets the color of an image pixel.\n"
                << "FBPainter::RGBAPixel FBPainter::" << baseName
                << "::getColor\n"
                << "(const size_t x, const size_t y)\n{\n"
                << "    if (x >= width || y >= height)\n    {\n"
                << "        return RGBAPixel();\n    }\n"
                << "    size_t charIdx = y * width + x;\n"
                << "    size_t idx = imageData[charIdx] - 'a';\n"
                << "    return RGBAPixel(colors[idx][0], colors[idx][1], "
                << "colors[idx][2],\n            colors[idx][3]);\n}";
    };

    if (! writeFile(headerPath, writeHeader))
    {
        return false;
    }
    if (! writeFile(sourcePath, writeSrc))
    {
        return false;
    }
    return true;
}


// Converts a single image, passed in as a command line argument.
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "No image given!\n";
        return 1;
    }
    const std::string imagePath(argv[1]);

    if (testEncode(imagePath))
    {
        std::cout << "Encoded image \"" << imagePath << "\"\n";
        return 0;
    }
    std::cout << "Encoding \"" << imagePath << "\" failed\n";
    return 1;
}
