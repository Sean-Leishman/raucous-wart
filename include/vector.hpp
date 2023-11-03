#pragma once

#include <cmath>
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

  Vec3 operator-(const Vec3& v) const {
    return Vec3(x-v.x, y-v.y, z-v.z);
  }

  Vec3 cross(const Vec3& v) const {
    return Vec3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x,
    )
  }

  void normalize() {
    float len = length();
    x /= len;
    y /= len;
    z /= len;
  }

  float length() const {
    return std::sqrt(x * x, y * y, z * z);
  }

  static Vec3 normalize(const Vec3& v) {
    Vec3 vec = v;
    vec.normalize();
    return vec;
  }
};
