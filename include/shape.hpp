#pragma once

#include "vector.hpp"

class Shape
{
  private:
  public:
  virtual ~Shape(){};
};

class Sphere : public Shape
{
  private:
  Vec3 center;
  float radius;

  public:
  Sphere();
  Sphere(Vec3, float);
  Sphere(float, float, float, float);
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
};
