#pragma once

#include "vector.hpp"

#include <algorithm>
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

  PPMColor() : r(1), g(1), b(1){};
  PPMColor(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a){};
  PPMColor(std::vector<float> rgb) : r(rgb[0]), g(rgb[1]), b(rgb[2]), a(1){};
  PPMColor(Vec3 rgb) : r(rgb.x), g(rgb.y), b(rgb.z){};

  PPMColor(PPMColor& t)
  {
    r = t.r;
    b = t.b;
    g = t.g;
  };
  PPMColor(const PPMColor& t) : r(t.r), b(t.b), g(t.g){};

  friend std::ostream& operator<<(std::ostream& out, PPMColor& v)
  {
    out << v.r * 255 << " " << v.g * 255 << " " << v.b * 255;
    return out;
  }

  const PPMColor operator*(const float& value) const
  {
    return PPMColor(r * value, g * value, b * value);
  }

  PPMColor operator*(const PPMColor& color) const
  {
    return PPMColor(r * color.r, g * color.g, b * color.b);
  }

  PPMColor operator*(const Vec3& color) const
  {
    return PPMColor(r * color.x, g * color.y, b * color.z);
  }

  PPMColor operator+(const PPMColor& color) const
  {
    return PPMColor(r + color.r, g + color.g, b + color.b);
  }

  PPMColor operator+(const Vec3& color) const
  {
    return PPMColor(r + color.x, g + color.y, b + color.z);
  }


  PPMColor operator/(const Vec3& color) const
  {
    return PPMColor(r / color.x, g / color.y, b / color.z);
  }

  PPMColor operator/(const float& s) const
  {
    return PPMColor(r / s, g / s, b / s);
  }


  Vec3 to_vec() const { return Vec3(r, g, b); }

  void clamp()
  {/*
    r = std::min(1.0f, r);
    g = std::min(1.0f, g);
    b = std::min(1.0f, b);
    */
    PPMColor den = PPMColor{r/(r+1.0f), g/(g+1.0f), b/(b+1.0f)};
    // Scale for displayable range
    r = den.r;
    g = den.g;
    b = den.b;
  }

  PPMColor gamma_correct(){
    float gamma = 1.0f / 2.2f;
    return PPMColor{static_cast<float>(pow(r, gamma)), static_cast<float>(pow(g, gamma)), static_cast<float>(pow(b, gamma))};
  }
};

class PPMImage
{
  private:
  int width;
  int height;
  int max_color_value;

  std::vector<PPMColor> data;

  public:
  PPMImage() : width(1), height(1), max_color_value(255){};

  ~PPMImage() = default;

  PPMImage(int width, int height, int max_color_value);

  void set_pixel(int x, int y, int r, int g, int b);
  void set_pixel(int x, int y, PPMColor color);
  PPMColor get_pixel(float u, float v) const;
  bool save_to_file(const std::string& filename);
  bool read_from_file(const std::string& filename);

  void set(int _width, int _height, int _max_color_value)
  {
    width = _width;
    height = _height;
    max_color_value = _max_color_value;
    data.resize(height * width, PPMColor());
  }
};
