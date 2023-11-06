#pragma once

#include "vector.hpp"
#include "image.hpp"
#include "intersection.hpp"

enum LightType{
  POINT,
};

class Light{
  public:
      Vec3 position;
      Vec3 intensity;
      PPMColor color;

    virtual ~Light(){};

    virtual float compute_intensity(float) = 0;
};

class PointLight: public Light {
public:
    Vec3 position;
    Vec3 intensity;
    PPMColor color;

    ~PointLight() = default;
    PointLight() = default;
    PointLight(Vec3 position, Vec3 intensity);
    float compute_intensity(float) override;
};