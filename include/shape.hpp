#pragma once

#include "intersection.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "vector.hpp"

class Shape
{
  private:
  public:
  virtual ~Shape(){};

  virtual bool intersect(const Ray& ray, float tmin, float tmax,
                         Intersection& intersection) const = 0;
};

class Sphere : public Shape
{
  private:
  Vec3 center;
  double radius;

  public:
  Sphere();
  Sphere(Vec3, float);
  Sphere(float, float, float, float);

  bool intersect(const Ray&, float, float, Intersection&) const override;
};

class Cylinder : public Shape
{
  private:
  Vec3 center;
  Vec3 axis;
  float radius;
  float height;

  public:
  Cylinder();
  Cylinder(Vec3, Vec3, float, float);

  bool intersect(const Ray& ray, float tmin, float tmax, Intersection& intersection) const override;
};

class Triangle : public Shape
{
  private:
  Vec3 v0;
  Vec3 v1;
  Vec3 v2;

  public:
  Triangle();
  Triangle(Vec3, Vec3, Vec3);

  bool intersect(const Ray& ray, float tmin, float tmax, Intersection& intersection) const override;
};
