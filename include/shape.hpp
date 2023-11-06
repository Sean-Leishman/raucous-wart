#pragma once

#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "vector.hpp"

class Intersection;

class Shape: public std::enable_shared_from_this<Shape>
{
  private:
  public:
       Material material;
  virtual ~Shape()= default;

  virtual std::shared_ptr<Shape> get_shared_ptr()  = 0;
  virtual bool intersect(const Ray& ray, float tmin, float tmax,
                         Intersection* intersection) const = 0;
};

class Sphere : public Shape
{
  private:
  Vec3 center;
  double radius;

  public:
      Material material;

  Sphere();
  Sphere(Vec3, float, Material);
  Sphere(float, float, float, float);


  std::shared_ptr<Shape> get_shared_ptr() override
  {
    return shared_from_this();
  }
  bool intersect(const Ray&, float, float, Intersection*) ;
};

class Cylinder : public Shape
{
  private:
  Vec3 center;
  Vec3 axis;
  float radius;
  float height;

  public:
      Material material;

  Cylinder();
  Cylinder(Vec3, Vec3, float, float, Material);

  std::shared_ptr<Shape> get_shared_ptr() override {
    return shared_from_this();
  }
  bool intersect(const Ray& ray, float tmin, float tmax, Intersection* intersection) const override;
};

class Triangle : public Shape
{
  private:
  Vec3 v0;
  Vec3 v1;
  Vec3 v2;

  public:
      Material material;
  Triangle();
  Triangle(Vec3, Vec3, Vec3, Material);

  std::shared_ptr<Shape> get_shared_ptr() override {
    return shared_from_this();
  }

  bool intersect(const Ray& ray, float tmin, float tmax, Intersection* intersection) const override;
};
