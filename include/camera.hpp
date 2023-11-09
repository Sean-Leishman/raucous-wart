#pragma once

#include "ray.hpp"
#include "vector.hpp"

class Camera
{
  public:
  virtual Vec3 get_position() = 0;
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

  Vec3 right;
  Vec3 up;
  Vec3 forward;

  float aspect;

  float half_height;
  float half_width;

  public:
  PRenderHole();
  PRenderHole(int, int, Vec3, Vec3, Vec3, float, float);

  Ray compute_ray(float, float);

  Vec3 get_position() override {return position;};

};
