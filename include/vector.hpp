#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <vector>

class Vec3
{
  public:
  double x;
  double y;
  double z;

  Vec3();
  Vec3(std::vector<float>);
  Vec3(float, float, float);

  Vec3 operator-(const Vec3& v) const
  {
    return Vec3(x - v.x, y - v.y, z - v.z);
  }

  Vec3 operator*(const float s) const { return Vec3(x * s, y * s, z * s); }

  Vec3 operator+(const Vec3& v) const
  {
    return Vec3(x + v.x, y + v.y, z + v.z);
  }

  Vec3 cross(const Vec3& v) const
  {
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
  }

  void normalize()
  {
    float len = length();
    x /= len;
    y /= len;
    z /= len;
  }

  float length() const { return std::sqrt(x * x + y * y + z * z); }

  static Vec3 normalize(const Vec3& v)
  {
    Vec3 vec = v;
    vec.normalize();
    return vec;
  }
};

class Mat3
{
  public:
  std::array<std::array<float, 3>, 3> m;

  Mat3()
  {
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        m[i][j] = (i == j) ? 1.0f : 0.0f;
      }
    }
  }

  // Multiply a Vector3 by this matrix
  Vec3 operator*(const Vec3& vec) const
  {
    return Vec3(m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z,
                m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z,
                m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z);
  }
};
