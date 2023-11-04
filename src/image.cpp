#include "image.hpp"

#include <fstream>
#include <iostream>
#include <vector>

PPMImage::PPMImage(int width, int height, int max_color_value)
    : width(width), height(height), max_color_value(max_color_value)
{
  data.resize(height, std::vector<PPMColor>(width));
}

void PPMImage::set_pixel(int x, int y, int r, int g, int b)
{
  PPMColor color = PPMColor{r, g, b};
  set_pixel(x,y , color);
}

void PPMImage::set_pixel(int x, int y, PPMColor color){
  if (x < 0 || x >= width || y < 0 || y >= height)
    return;
  data[y][x] = color;
}

bool PPMImage::save_to_file(const std::string& filename)
{
  std::ofstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Failed to open file for writing." << std::endl;
    return false;
  }

  // Write header
  file << "P3\n";
  file << width << " " << height << "\n";
  file << max_color_value << "\n";

  // Write pixel data
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      file << data[y][x] << "\n";
    }
  }

  file.close();
  return true;
}
