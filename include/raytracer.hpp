#pragma once

#include "ray.hpp"
#include "image.hpp"
#include "intersection.hpp"
#include "scene.hpp"
#include "camera.hpp"

class Raytracer{
  public:
      virtual ~Raytracer() = default;

      Scene* scene;
      Camera* camera;

      Raytracer(Scene* scene, Camera* camera): scene(scene), camera(camera){};

      virtual PPMColor trace_ray(Ray& ray) = 0;
};

class BinaryRaytracer: public Raytracer{
  public:
      BinaryRaytracer(Scene* scene, Camera* camera) : Raytracer(scene ,camera){};
      PPMColor trace_ray(Ray& ray) override;
};

class PhongRaytracer: public Raytracer{
  public:
      PhongRaytracer(Scene* scene, Camera* camera) : Raytracer(scene, camera) {}
      PPMColor trace_ray(Ray& ray) override;
};
