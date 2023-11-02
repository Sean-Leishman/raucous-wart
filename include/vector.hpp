#pragma once

#include <vector>

class Vec3
{
  private:
  float x;
  float y;
  float z;

  public:
  Vec3();
  Vec3(std::vector<float>);
  Vec3(float, float, float);
};
