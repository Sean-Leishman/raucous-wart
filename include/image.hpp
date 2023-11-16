#pragma once

#include "vector.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

class PPMColor
{
  public:
      float r;
  float g;
  float b;
  float a;

      PPMColor(): r(1), g(1), b(1){};
  PPMColor(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a){};
  PPMColor(std::vector<float> rgb): r(rgb[0]), g(rgb[1]), b(rgb[2]), a(1){};
  PPMColor(Vec3 rgb): r(rgb.x), g(rgb.y), b(rgb.z){};

  PPMColor(PPMColor& t){
    r = t.r;
    b = t.b;
    g = t.g;
  };
  PPMColor(const PPMColor& t): r(t.r), b(t.b), g(t.g){};

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

  ~PPMImage()=default;
  PPMImage(PPMImage &t)
  {
    width = t.width;
    height = t.height;
    max_color_value = t.max_color_value;

    for (auto it=t.data.begin(); it!=t.data.end(); ++it){
      auto i = std::distance(t.data.begin(), it);

      data[i] = std::vector<PPMColor>(height);
      for (auto it2=t.data[i].begin(); it2!=t.data[i].end(); ++it2){
        auto j = std::distance(t.data[i].begin(), it2);
        data[i][j] = t.data[i][j];
        }
    }
  }

  PPMImage(int width, int height, int max_color_value);

  void set_pixel(int x, int y, int r, int g, int b);
  void set_pixel(int x, int y, PPMColor color);
  PPMColor get_pixel(float u, float v) const;
  bool save_to_file(const std::string& filename);
  bool read_from_file(const std::string& filename);

  void set(int _width, int _height, int _max_color_value){
    width=_width;
    height=_height;
    max_color_value=_max_color_value;
    data.resize(width, std::vector<PPMColor>(height));
  }
};
