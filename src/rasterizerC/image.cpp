#include <sstream>

#include "image.h"

PPMImage::
PPMImage(size_t width, size_t height, const Color & backgroundColor):
width(width), height(height), pixels(width*height, backgroundColor)
{
}

PPMImage::
PPMImage(std::fstream & fileIn)
{
    char _cdump;
    int _idump;
    fileIn >> _cdump >> _cdump; // magic words, "P6" or "P3"
    fileIn >> width >> height; //TODO: non-square
    fileIn >> _idump; // max color value
    fileIn.get(_cdump);
    pixels = std::vector<Color>(width*height, Color());
    for (int y = height - 1; y > -1; y--) 
    {
        for (size_t x = 0; x < width; x++) 
        {
            unsigned char rgbBuff[3];
            float r, g, b;
            //in >> rc >> gc >> bc;
            fileIn.read((char *)rgbBuff, 3);
            r = (float)rgbBuff[0] / 255.0F;
            g = (float)rgbBuff[1] / 255.0F;
            b = (float)rgbBuff[2] / 255.0F;
            InitColor(&pixels[y * width + x], r, g, b);
            //if (fileIn.eof()) 
            //{
                //in.close();
                //exit(1);
            //}
        }
    }
}

AABox PPMImage:: 
Bounds() const
{
    return AABox(Vector(0.0, 0.0), Vector(1.0, 1.0));	
}

void PPMImage::
WritePPM(std::fstream & fileOut)
{
    std::ostringstream oss;
    oss << "P6 " << width << ' '
        << height << " 255\n";
    fileOut << oss.str();
    for (int y = height - 1; y > -1; y--) 
    {
        for (size_t x = 0; x < width; x++) 
        {
            std::string str;
            unsigned char buf[3];
            /*
            if (pixels[y * resolution + x].transparent && bg != nullptr) {
                str = bg->pixels[y * resolution + x].as_ppm();
            }
            else {
                str = pixels[y * resolution + x].as_ppm();
            }
            out << str;
            */
            AsPPM(&pixels[y * width + x], buf);
            //out.put(buf[0]);
            //out.put(buf[1]);
            //out.put(buf[2]);
            fileOut.write((char *)buf, 3);
        }
    }
}
