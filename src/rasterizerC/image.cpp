#include <sstream>
#include "image.h"

PPMImage::
PPMImage(size_t Width, size_t Height, const Color &BackgroundColor):
Width(Width), Height(Height), Pixels(Width*Height, BackgroundColor)
{
}

PPMImage::
PPMImage(std::fstream &FileIn)
{
  char _cdump;
  int _idump;
  FileIn >> _cdump >> _cdump; // magic words, "P6" or "P3"
  FileIn >> Width >> Height; //TODO: non-square
  FileIn >> _idump; // max color value
  FileIn.get(_cdump);
  Pixels = std::vector<Color>(Width*Height, Color());
  for (int y = Height - 1; y > -1; y--) {
    for (size_t x = 0; x < Width; x++) {
      unsigned char rgbbuf[3];
      float r, g, b;
      //in >> rc >> gc >> bc;
      FileIn.read((char *)rgbbuf, 3);
      r = (float)rgbbuf[0] / 255.0F;
      g = (float)rgbbuf[1] / 255.0F;
      b = (float)rgbbuf[2] / 255.0F;
      initColor(&Pixels[y * Width + x], r, g, b);
      if (FileIn.eof()) {
        //in.close();
        //exit(1);
      }
    }
  }
}

AABox PPMImage:: 
bounds() const
{
  return AABox(Vector(0.0, 0.0), Vector(1.0, 1.0));	
}

void PPMImage::
writePPM(std::fstream &FileOut)
{
  std::ostringstream oss;
  oss << "P6 " << Width << ' '
    << Height << " 255\n";
  FileOut << oss.str();
  for (int y = Height - 1; y > -1; y--) {
    for (size_t x = 0; x < Width; x++) {
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
      asPPM(&Pixels[y * Width + x], buf);
      //out.put(buf[0]);
      //out.put(buf[1]);
      //out.put(buf[2]);
      FileOut.write((char *)buf, 3);
    }
  }
}