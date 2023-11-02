#pragma once

#include "vector.hpp"

class Camera
{
  private:
};

class PRenderHole : public Camera
{
  private:
  int width;
  int height;
  Vec3 position;
  Vec3 lookAt;
  Vec3 upVector;
  float fov;
  float exposure;

  public:
  PRenderHole();
  PRenderHole(int, int, Vec3, Vec3, Vec3, float, float);
};
