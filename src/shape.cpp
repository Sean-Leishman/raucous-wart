#include "shape.hpp"
#include "vector.hpp"

Sphere::Sphere()
{
  center = Vec3();
  radius = 0;
}

Sphere::Sphere(Vec3 center, float radius) : center(center), radius(radius){};

Cylinder::Cylinder()
{
  center = Vec3();
  axis = Vec3();
  radius = 0;
  height = 0;
}

Cylinder::Cylinder(Vec3 center, Vec3 axis, float radius, float height)
    : center(center), axis(axis), radius(radius), height(height){};

Triangle::Triangle(Vec3 v0, Vec3 v1, Vec3 v2) : v0(v0), v1(v1), v2(v2){};
