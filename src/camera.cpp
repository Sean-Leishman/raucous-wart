#include "camera.hpp"
#include "vector.hpp"

PRenderHole::PRenderHole()
{
  width = 0;
  height = 0;
  position = Vec3();
  lookAt = Vec3();
  upVector = Vec3();
  fov = 0;
  exposure = 0;
};

PRenderHole::PRenderHole(int width, int height, Vec3 position, Vec3 lookAt,
                         Vec3 upVector, float fov, float exposure)
    : width(width), height(height), position(position), lookAt(lookAt),
      upVector(upVector), fov(fov), exposure(exposure){
    forward = (eye-lookAt).normalize();
    right = upVector.cross(w).normalize();
    up = w.cross(right);
      };
