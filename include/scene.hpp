#pragma once

#include "shape.hpp"

#include <vector>

struct BackgroundColor
{
  float r;
  float g;
  float b;
};

class Scene
{
  private:
  BackgroundColor bg_color;
  std::vector<Shape> shapes;

  public:
  Scene();
  void add_shape(Shape shape);
};
