#pragma once

#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "vector.hpp"

class Intersection;

class Shape
{
  private:
  public:
  virtual ~Shape()= default;


  virtual bool intersect(const Ray& ray, float tmin, float tmax,
                         Intersection& intersection) const = 0;
};

class Sphere : public Shape
{
  private:
  Material material;
  Vec3 center;
  double radius;

  public:
  Sphere();
  Sphere(Vec3, float, Material);
  Sphere(float, float, float, float);

  template <typename T>
  std::shared_ptr<Shape> shared_ptr() const {
    return std::make_shared<T>();
  }

  bool intersect(const Ray&, float, float, Intersection&) const override;

};

class Cylinder : public Shape
{
  private:
  Vec3 center;
  Vec3 axis;
  float radius;
  float height;
  Material material;

  public:
  Cylinder();
  Cylinder(Vec3, Vec3, float, float, Material);

  template <typename T>
  std::shared_ptr<Shape> shared_ptr() const {
    return std::make_shared<T>();
  }
  bool intersect(const Ray& ray, float tmin, float tmax, Intersection& intersection) const override;
};

class Triangle : public Shape
{
  private:
  Vec3 v0;
  Vec3 v1;
  Vec3 v2;
  Material material;

  public:
  Triangle();
  Triangle(Vec3, Vec3, Vec3, Material);

  template <typename T>
  std::shared_ptr<Shape> shared_ptr() const {
    return std::make_shared<T>();
  }
  bool intersect(const Ray& ray, float tmin, float tmax, Intersection& intersection) const override;
};
