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

      Light(Vec3, Vec3, PPMColor);
      Light(Vec3, Vec3);
    virtual ~Light(){};

    virtual float compute_intensity(float) = 0;
};

class PointLight: public Light {
public:
    ~PointLight()=default;
    PointLight();
    PointLight(Vec3 position, Vec3 intensity);
    float compute_intensity(float) override;
};

class AmbientLight: public Light {
public:
    ~AmbientLight()=default;
    AmbientLight();
    AmbientLight(Vec3 intensity, PPMColor color);
    float compute_intensity(float) override;
};
