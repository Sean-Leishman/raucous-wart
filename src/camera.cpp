#include "camera.hpp"
#include "ray.hpp"
#include "vector.hpp"

#include <numbers>

PRenderHole::PRenderHole()
{
  width = 0;
  height = 0;
  position = Vec3();
  lookAt = Vec3();
  upVector = Vec3();
  fov = 0;
  exposure = 0;

  right = Vec3();
  up = Vec3();
  forward = Vec3();
};

PRenderHole::PRenderHole(int width, int height, Vec3 position, Vec3 lookAt,
                         Vec3 upVector, float fov, float exposure)
    : width(width), height(height), position(position), lookAt(lookAt),
      upVector(upVector), fov(fov), exposure(exposure)
{
  forward = Vec3::normalize(position - lookAt);
  right = Vec3::normalize(upVector.cross(forward));
  up = forward.cross(right);

  aspect = width / (float)height;

  float theta = fov * M_PI / 180.0f;
  half_width = std::tan(theta / 2.0f);
  half_height = aspect * half_height;
};

Ray PRenderHole::compute_ray(float s, float t)
{
  s = (s / (float)width) * 2 - 1;
  t = (t / (float)height) * 2 - 1;

  s *= aspect;

  Vec3 ray_direction = right * half_width* s + up * half_width * t - forward;

  ray_direction = Vec3::normalize(ray_direction);

  return Ray{position, ray_direction};
};
