#pragma once

#include "vector.hpp"

#include <fstream>
#include <iostream>
#include <vector>

class PPMColor
{
  public:
      float r;
  float g;
  float b;
  float a;

      PPMColor(){};
  PPMColor(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a){};
  PPMColor(std::vector<float> rgb): r(rgb[0]), g(rgb[1]), b(rgb[2]), a(1){};
  PPMColor(Vec3 rgb): r(rgb.x), g(rgb.y), b(rgb.z){};

  friend std::ostream& operator<<(std::ostream& out, PPMColor& v)
  {
    out << v.r * 255 << " " << v.g * 255 << " " << v.b * 255;
    return out;
  }

  PPMColor operator*(float value) const {
    return PPMColor(r * value, g * value, b * value);
  }

  PPMColor operator*(const PPMColor& color) const {
    return PPMColor(r * color.r, g * color.g, b * color.b);
  }

  PPMColor operator*(const Vec3& color) const {
    return PPMColor(r * color.x, g * color.y, b * color.z);
  }

  PPMColor operator+(const PPMColor& color) const {
    return PPMColor(r + color.r, g + color.g, b + color.b);
  }

  Vec3 to_vec() const {
    return Vec3(r, g, b);
  }

  void clamp(){
    r = std::min(1.0f, r);
    g  = std::min(1.0f, g);
    b = std::min(1.0f, b);
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
