#pragma once

#include <fstream>
#include <iostream>
#include <vector>

class PPMColor
{
  private:
  int r;
  int g;
  int b;
  int a;

  public:
      PPMColor(){};
  PPMColor(int r, int g, int b, int a = 255) : r(r), g(g), b(b), a(a){};

  friend std::ostream& operator<<(std::ostream& out, PPMColor& v)
  {
    out << v.r << " " << v.g << " " << v.b;
    return out;
  }
};

class PPMImage
{
  private:
  int width;
  int height;
  int max_color_value;

  std::vector<std::vector<PPMColor>> data;

  public:
  PPMImage(){};
  PPMImage(int width, int height, int max_color_value);
  void set_pixel(int x, int y, int r, int g, int b);
  void set_pixel(int x, int y, PPMColor color);
  bool save_to_file(const std::string& filename);
};
